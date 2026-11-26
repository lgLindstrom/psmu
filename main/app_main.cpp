/* PSMD – Matter Controller
 *
 * One endpoint, On/Off Plug-in Unit (0x010A). OnOff mirrors Output State in
 * both directions; Operation Mode, the button and the LEDs are the local
 * Power Unit simulation until the real hardware exists (M3).
 */

#include <esp_err.h>
#include <esp_log.h>
#include <esp_mac.h>
#include <nvs_flash.h>

#include <cstdio>
#include <cstring>

#include <esp_matter.h>
#include <platform/CHIPDeviceLayer.h>
#include <platform/ConfigurationManager.h>
#include <platform/DeviceInstanceInfoProvider.h>

#include <app/server/Server.h>
#include <setup_payload/OnboardingCodesUtil.h>

#include "psmd_button.h"
#include "psmd_led.h"
#include "psmd_mode.h"
#include "psmd_output.h"

static const char *TAG = "psmd_main";

/* `Pairing Timeout`: 3 min (`TBD-31`). */
#define PSMD_PAIRING_TIMEOUT_S 180

using namespace esp_matter;
using namespace esp_matter::attribute;
using namespace chip::app::Clusters;

static uint16_t s_endpoint_id;  /* The one endpoint; OnOff reports go to it. */

static void print_onboarding_codes(void)
{
    PrintOnboardingCodes(chip::RendezvousInformationFlags(chip::RendezvousInformationFlag::kBLE));
}

/* One line per Fabric, so a test can tell two entries of the same Fabric (the
 * same compressed ID) from two controllers. */
static void log_fabrics(const char *when)
{
    auto &table = chip::Server::GetInstance().GetFabricTable();
    ESP_LOGI(TAG, "%s: %u fabric(s)", when, (unsigned)table.FabricCount());
    for (const auto &fabric : table) {
        ESP_LOGI(TAG,
                 "  index %u compressed 0x%016llX fabric 0x%016llX node 0x%016llX vendor 0x%04X",
                 (unsigned)fabric.GetFabricIndex(), (unsigned long long)fabric.GetCompressedFabricId(),
                 (unsigned long long)fabric.GetFabricId(), (unsigned long long)fabric.GetNodeId(),
                 (unsigned)fabric.GetVendorId());
    }
}

static esp_err_t app_identification_cb(identification::callback_type_t type, uint16_t endpoint_id,
                                       uint8_t effect_id, uint8_t effect_variant, void *priv_data)
{
    ESP_LOGI(TAG, "Identify: type %u, effect %u, variant %u", type, effect_id, effect_variant);
    return ESP_OK;
}

static esp_err_t app_attribute_update_cb(callback_type_t type, uint16_t endpoint_id, uint32_t cluster_id,
                                         uint32_t attribute_id, esp_matter_attr_val_t *val, void *priv_data)
{
    if (type == PRE_UPDATE && cluster_id == OnOff::Id && attribute_id == OnOff::Attributes::OnOff::Id) {
        // PRE_UPDATE runs before the attribute is stored, so a failing output
        // update rejects the write. One endpoint: OnOff is the Output.
        return psmd_output_set(val->val.b);
    }
    return ESP_OK;
}

static void publish_output_state(bool on)
{
    // A local change has already set the value; the report tells subscribers
    // (Home Assistant) about it.
    esp_matter_attr_val_t val = esp_matter_bool(on);
    esp_err_t err = attribute::report(s_endpoint_id, OnOff::Id, OnOff::Attributes::OnOff::Id, &val);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "OnOff report failed: %s", esp_err_to_name(err));
    }
}

static void commissioning_window_work(intptr_t open)
{
    auto &manager = chip::Server::GetInstance().GetCommissioningWindowManager();

    if (open != 0) {
        if (manager.IsCommissioningWindowOpen()) {
            // The 900 s window from Power-up may still be open (TBD-31); do not
            // restart its timer.
            ESP_LOGI(TAG, "Commissioning window already open");
            psmd_mode_pairing();
            return;
        }
        CHIP_ERROR err = manager.OpenBasicCommissioningWindow(chip::System::Clock::Seconds32(PSMD_PAIRING_TIMEOUT_S));
        if (err == CHIP_NO_ERROR) {
            // The event callback logs "Commissioning window opened"; only the
            // codes are printed here. PAIRING follows the open, not the event,
            // so the boot window that is closed again does not show it.
            psmd_mode_pairing();
            print_onboarding_codes();
        } else {
            ESP_LOGE(TAG, "Commissioning window: %" CHIP_ERROR_FORMAT, err.Format());
        }
    } else if (manager.IsCommissioningWindowOpen()) {
        // The event callback logs "Commissioning window closed".
        manager.CloseCommissioningWindow();
    }
}

static void on_commissioning_window(bool open)
{
    // CHIP calls the event handler on its own task; the window manager has to
    // run on the platform task.
    CHIP_ERROR err = chip::DeviceLayer::PlatformMgr().ScheduleWork(commissioning_window_work, open ? 1 : 0);
    if (err != CHIP_NO_ERROR) {
        ESP_LOGE(TAG, "ScheduleWork: %" CHIP_ERROR_FORMAT, err.Format());
    }
}

static void app_event_cb(const ChipDeviceEvent *event, intptr_t arg)
{
    switch (event->Type) {
    case chip::DeviceLayer::DeviceEventType::kCommissioningComplete:
        ESP_LOGI(TAG, "Commissioning complete");
        psmd_mode_commissioned();
        log_fabrics("Fabrics after commissioning");
        break;
    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowOpened:
        ESP_LOGI(TAG, "Commissioning window opened");
        log_fabrics("Fabrics at window open");
        break;
    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowClosed:
        ESP_LOGI(TAG, "Commissioning window closed");
        // CHIP also stops advertising when PASE is established, so the event
        // alone does not mean the attempt failed: an armed fail-safe means the
        // commissioning is still running.
        if (!chip::Server::GetInstance().GetFailSafeContext().IsFailSafeArmed()) {
            psmd_mode_window_closed(chip::Server::GetInstance().GetFabricTable().FabricCount() > 0);
        }
        break;
    case chip::DeviceLayer::DeviceEventType::kFabricRemoved: {
        // Removing one Fabric does not make the device commissionable: another
        // controller may still own one (multi-admin), so the mode and the window
        // wait for the last.
        uint8_t fabrics_left = chip::Server::GetInstance().GetFabricTable().FabricCount();
        ESP_LOGI(TAG, "Fabric removed (%u left)", (unsigned)fabrics_left);
        log_fabrics("Fabrics after removal");
        if (fabrics_left == 0) {
            psmd_mode_fabric_removed();
            // No administrator left: the device must not stay on the network.
            chip::DeviceLayer::ConnectivityMgr().ClearWiFiStationProvision();
            ESP_LOGI(TAG, "Wi-Fi credentials erased");
            on_commissioning_window(true);
        }
        break;
    }
    default:
        break;
    }
}

static void on_output_changed(bool on)
{
    publish_output_state(on);
}

static void on_decommission(void)
{
    ESP_LOGW(TAG, "Decommissioning");
    esp_err_t err = esp_matter::factory_reset();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Factory reset failed: %s", esp_err_to_name(err));
    }
}

/* Per-unit `SerialNumber` (`TBD-30`): the board's base MAC, stored once. */
static void store_serial_number(void)
{
    uint8_t mac[6];
    esp_err_t mac_err = esp_read_mac(mac, ESP_MAC_BASE);
    if (mac_err != ESP_OK) {
        ESP_LOGE(TAG, "Base MAC: %s", esp_err_to_name(mac_err));
        return;
    }
    char serial[32];
    snprintf(serial, sizeof(serial), "PSMD-DEV-%02X%02X%02X%02X%02X%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    char stored[32] = {};
    CHIP_ERROR err = chip::DeviceLayer::GetDeviceInstanceInfoProvider()->GetSerialNumber(stored, sizeof(stored));
    // Skip the NVS write when the stored value already matches: it is written
    // once per unit, not once per boot.
    if (err == CHIP_NO_ERROR && strcmp(stored, serial) == 0) {
        return;
    }
    err = chip::DeviceLayer::ConfigurationMgr().StoreSerialNumber(serial, strlen(serial));
    if (err == CHIP_NO_ERROR) {
        ESP_LOGI(TAG, "SerialNumber = %s", serial);
    } else {
        ESP_LOGE(TAG, "StoreSerialNumber: %" CHIP_ERROR_FORMAT, err.Format());
    }
}

extern "C" void app_main()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(psmd_led_init());
    ESP_ERROR_CHECK(psmd_output_init());

    node::config_t node_config;
    node_t *node = node::create(&node_config, app_attribute_update_cb, app_identification_cb);
    ESP_ERROR_CHECK(node == nullptr ? ESP_FAIL : ESP_OK);

    endpoint::on_off_plug_in_unit::config_t plug_config;
    plug_config.on_off.on_off = false;
    // Power-loss Recovery: Output State is OFF at Power-up.
    plug_config.on_off_lighting.start_up_on_off = nullable<uint8_t>(0);

    endpoint_t *endpoint = endpoint::on_off_plug_in_unit::create(node, &plug_config, ENDPOINT_FLAG_NONE, nullptr);
    ESP_ERROR_CHECK(endpoint == nullptr ? ESP_FAIL : ESP_OK);
    s_endpoint_id = endpoint::get_id(endpoint);

    ESP_LOGI(TAG, "On/Off Plug-in Unit on endpoint %u", s_endpoint_id);

    ESP_ERROR_CHECK(esp_matter::start(app_event_cb));
    store_serial_number();

    // Power-loss Recovery: OnOff is non-volatile and esp-matter restores the
    // stored value at Power-up, so Output State is set to OFF again.
    esp_matter_attr_val_t off = esp_matter_bool(false);
    ESP_ERROR_CHECK(attribute::update(s_endpoint_id, OnOff::Id, OnOff::Attributes::OnOff::Id, &off));

    // Boot Mode: OPERATIONAL when the device already has a Fabric, otherwise
    // LOCAL. The count is logged so a test can tell how many are present.
    uint8_t fabrics = chip::Server::GetInstance().GetFabricTable().FabricCount();
    ESP_LOGI(TAG, "Fabrics at Power-up = %u", (unsigned)fabrics);
    log_fabrics("Fabrics at Power-up");
    bool commissioned = fabrics > 0;
    psmd_mode_callbacks_t callbacks = {};
    callbacks.on_output_changed = on_output_changed;
    callbacks.on_commissioning_window = on_commissioning_window;
    callbacks.on_decommission = on_decommission;
    ESP_ERROR_CHECK(psmd_mode_init(commissioned, &callbacks));
    ESP_ERROR_CHECK(psmd_button_init(psmd_mode_button_event));

    // Pair only on Long Press: close the window ESP-Matter opens at Power-up, so
    // the device is never pairable outside PAIRING and its slow blink.
    if (!commissioned) {
        on_commissioning_window(false);
    }
}
