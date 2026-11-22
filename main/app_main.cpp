/* PSMD – Matter Controller
 *
 * One endpoint, On/Off Plug-in Unit (0x010A). OnOff mirrors Output State in
 * both directions; Operation Mode, the button and the LEDs are the local
 * Power Unit simulation until the real hardware exists (M3).
 */

#include <esp_err.h>
#include <esp_log.h>
#include <nvs_flash.h>

#include <esp_matter.h>
#include <platform/CHIPDeviceLayer.h>

#include <app/server/Server.h>
#include <setup_payload/OnboardingCodesUtil.h>

#include "psmd_button.h"
#include "psmd_led.h"
#include "psmd_mode.h"
#include "psmd_output.h"

static const char *TAG = "psmd_main";

/* `Pairing Timeout`: 3 min. */
#define PSMD_PAIRING_TIMEOUT_S 180

using namespace esp_matter;
using namespace esp_matter::attribute;
using namespace chip::app::Clusters;

static uint16_t s_endpoint_id;

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
        // One endpoint: the OnOff attribute belongs to the Output.
        return psmd_output_set(val->val.b);
    }
    return ESP_OK;
}

static void publish_output_state(bool on)
{
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
            ESP_LOGI(TAG, "Commissioning window already open");
            return;
        }
        CHIP_ERROR err = manager.OpenBasicCommissioningWindow(chip::System::Clock::Seconds32(PSMD_PAIRING_TIMEOUT_S));
        if (err == CHIP_NO_ERROR) {
            ESP_LOGI(TAG, "Commissioning window opened");
        } else {
            ESP_LOGE(TAG, "Commissioning window: %" CHIP_ERROR_FORMAT, err.Format());
        }
    } else if (manager.IsCommissioningWindowOpen()) {
        manager.CloseCommissioningWindow();
        ESP_LOGI(TAG, "Commissioning window closed");
    }
}

static void app_event_cb(const ChipDeviceEvent *event, intptr_t arg)
{
    switch (event->Type) {
    case chip::DeviceLayer::DeviceEventType::kCommissioningComplete:
        ESP_LOGI(TAG, "Commissioning complete");
        psmd_mode_commissioned();
        break;
    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowOpened:
        ESP_LOGI(TAG, "Commissioning window opened");
        break;
    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowClosed:
        ESP_LOGI(TAG, "Commissioning window closed");
        psmd_mode_window_closed();
        break;
    case chip::DeviceLayer::DeviceEventType::kFabricRemoved:
        ESP_LOGI(TAG, "Fabric removed");
        psmd_mode_fabric_removed();
        break;
    default:
        break;
    }
}

static void on_output_changed(bool on)
{
    publish_output_state(on);
}

static void on_commissioning_window(bool open)
{
    CHIP_ERROR err = chip::DeviceLayer::PlatformMgr().ScheduleWork(commissioning_window_work, open ? 1 : 0);
    if (err != CHIP_NO_ERROR) {
        ESP_LOGE(TAG, "ScheduleWork: %" CHIP_ERROR_FORMAT, err.Format());
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

    // Power-loss Recovery: OnOff is non-volatile and esp-matter restores the
    // stored value at Power-up, so Output State is set to OFF again.
    esp_matter_attr_val_t off = esp_matter_bool(false);
    ESP_ERROR_CHECK(attribute::update(s_endpoint_id, OnOff::Id, OnOff::Attributes::OnOff::Id, &off));

    // Boot Mode: OPERATIONAL when the device already has a Fabric, otherwise
    // LOCAL.
    bool commissioned = chip::Server::GetInstance().GetFabricTable().FabricCount() > 0;
    psmd_mode_callbacks_t callbacks = {};
    callbacks.on_output_changed = on_output_changed;
    callbacks.on_commissioning_window = on_commissioning_window;
    ESP_ERROR_CHECK(psmd_mode_init(commissioned, &callbacks));
    ESP_ERROR_CHECK(psmd_button_init(psmd_mode_button_event));

    // The commissioning window is open only while no fabric exists, so the
    // onboarding payload is printed from the values the device uses, then.
    if (!commissioned) {
        PrintOnboardingCodes(chip::RendezvousInformationFlags(chip::RendezvousInformationFlag::kBLE));
    }
}
