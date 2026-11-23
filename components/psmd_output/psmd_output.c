#include "psmd_output.h"

#include <driver/gpio.h>
#include <esp_check.h>
#include <esp_log.h>

#include "psmd_board.h"

static const char *TAG = "psmd_output";

static bool s_on;

static void set_output_led(bool on)
{
    if (PSMD_OUTPUT_LED_GPIO >= 0) {
        gpio_set_level(PSMD_OUTPUT_LED_GPIO, on ? PSMD_OUTPUT_LED_ON_LEVEL : !PSMD_OUTPUT_LED_ON_LEVEL);
    }
}

esp_err_t psmd_output_init(void)
{
    if (PSMD_OUTPUT_GPIO >= 0) {
        gpio_reset_pin(PSMD_OUTPUT_GPIO);
        ESP_RETURN_ON_ERROR(gpio_set_direction(PSMD_OUTPUT_GPIO, GPIO_MODE_OUTPUT), TAG,
                            "GPIO %d: output mode failed", PSMD_OUTPUT_GPIO);
    }
    if (PSMD_OUTPUT_LED_GPIO >= 0) {
        gpio_reset_pin(PSMD_OUTPUT_LED_GPIO);
        ESP_RETURN_ON_ERROR(gpio_set_direction(PSMD_OUTPUT_LED_GPIO, GPIO_MODE_OUTPUT), TAG,
                            "GPIO %d: output mode failed", PSMD_OUTPUT_LED_GPIO);
        set_output_led(false);
    }
    return psmd_output_set(false);
}

esp_err_t psmd_output_set(bool on)
{
    s_on = on;
    if (PSMD_OUTPUT_GPIO >= 0) {
        ESP_RETURN_ON_ERROR(gpio_set_level(PSMD_OUTPUT_GPIO, on ? PSMD_OUTPUT_ON_LEVEL : !PSMD_OUTPUT_ON_LEVEL),
                            TAG, "GPIO %d: set level failed", PSMD_OUTPUT_GPIO);
    }
    set_output_led(on);
    ESP_LOGI(TAG, "Output State = %s", on ? "ON" : "OFF");
    return ESP_OK;
}

bool psmd_output_get(void)
{
    return s_on;
}

esp_err_t psmd_output_toggle(void)
{
    return psmd_output_set(!s_on);
}
