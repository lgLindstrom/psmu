#include "psmd_output.h"

#include <sdkconfig.h>

#include <driver/gpio.h>
#include <esp_check.h>
#include <esp_log.h>

static const char *TAG = "psmd_output";

esp_err_t psmd_output_init(void)
{
    gpio_reset_pin(CONFIG_PSMD_OUTPUT_GPIO);
    ESP_RETURN_ON_ERROR(gpio_set_direction(CONFIG_PSMD_OUTPUT_GPIO, GPIO_MODE_OUTPUT), TAG,
                        "GPIO %d: output mode failed", CONFIG_PSMD_OUTPUT_GPIO);
    return psmd_output_set(false);
}

esp_err_t psmd_output_set(bool on)
{
    ESP_RETURN_ON_ERROR(gpio_set_level(CONFIG_PSMD_OUTPUT_GPIO, on), TAG,
                        "GPIO %d: set level failed", CONFIG_PSMD_OUTPUT_GPIO);
    ESP_LOGI(TAG, "Output State = %s", on ? "ON" : "OFF");
    return ESP_OK;
}
