/* PSMD – Matter Controller
 *
 * One endpoint, On/Off Plug-in Unit (0x010A). The OnOff attribute mirrors
 * Output State; there is no PSMD logic yet (M1).
 */

#include <esp_err.h>
#include <esp_log.h>
#include <nvs_flash.h>

#include <esp_matter.h>

#include <app/server/Server.h>
#include <setup_payload/OnboardingCodesUtil.h>

#include "psmd_output.h"

static const char *TAG = "psmd_main";

using namespace esp_matter;
using namespace esp_matter::attribute;
using namespace chip::app::Clusters;

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

static void app_event_cb(const ChipDeviceEvent *event, intptr_t arg)
{
    switch (event->Type) {
    case chip::DeviceLayer::DeviceEventType::kCommissioningComplete:
        ESP_LOGI(TAG, "Commissioning complete");
        break;
    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowOpened:
        ESP_LOGI(TAG, "Commissioning window opened");
        break;
    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowClosed:
        ESP_LOGI(TAG, "Commissioning window closed");
        break;
    case chip::DeviceLayer::DeviceEventType::kFabricRemoved:
        ESP_LOGI(TAG, "Fabric removed");
        break;
    default:
        break;
    }
}

extern "C" void app_main()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(psmd_output_init());

    node::config_t node_config;
    node_t *node = node::create(&node_config, app_attribute_update_cb, app_identification_cb);
    ESP_ERROR_CHECK(node == nullptr ? ESP_FAIL : ESP_OK);

    endpoint::on_off_plug_in_unit::config_t plug_config;
    plug_config.on_off.on_off = false;

    endpoint_t *endpoint = endpoint::on_off_plug_in_unit::create(node, &plug_config, ENDPOINT_FLAG_NONE, nullptr);
    ESP_ERROR_CHECK(endpoint == nullptr ? ESP_FAIL : ESP_OK);

    ESP_LOGI(TAG, "On/Off Plug-in Unit on endpoint %u", endpoint::get_id(endpoint));

    ESP_ERROR_CHECK(esp_matter::start(app_event_cb));

    // The commissioning window is open only while no fabric exists, so the
    // onboarding payload is printed from the values the device uses, then.
    if (chip::Server::GetInstance().GetFabricTable().FabricCount() == 0) {
        PrintOnboardingCodes(chip::RendezvousInformationFlags(chip::RendezvousInformationFlag::kBLE));
    }
}
