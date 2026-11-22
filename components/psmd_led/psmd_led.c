#include "psmd_led.h"

#include <stdint.h>

#include <driver/gpio.h>
#include <esp_check.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "psmd_board.h"

static const char *TAG = "psmd_led";

#define LED_POLL_INTERVAL_MS 10
#define FAST_BLINK_HALF_US   (250 * 1000)
#define SLOW_BLINK_HALF_US   (500 * 1000)

/* Written by psmd_led_set_pattern() from other tasks, read by led_task(). */
static volatile psmd_led_pattern_t s_pattern = PSMD_LED_PATTERN_OFF;

static void set_level(int gpio, int on_level, bool on)
{
    if (gpio >= 0) {
        gpio_set_level(gpio, on ? on_level : !on_level);
    }
}

static bool level_for(psmd_led_pattern_t pattern, int64_t now_us)
{
    switch (pattern) {
    case PSMD_LED_PATTERN_SOLID:
        return true;
    case PSMD_LED_PATTERN_FAST_BLINK:
        return (now_us / FAST_BLINK_HALF_US) % 2 == 0;
    case PSMD_LED_PATTERN_SLOW_BLINK:
        return (now_us / SLOW_BLINK_HALF_US) % 2 == 0;
    case PSMD_LED_PATTERN_OFF:
    default:
        return false;
    }
}

static void led_task(void *arg)
{
    for (;;) {
        set_level(PSMD_MODE_LED_GPIO, PSMD_MODE_LED_ON_LEVEL, level_for(s_pattern, esp_timer_get_time()));
        vTaskDelay(pdMS_TO_TICKS(LED_POLL_INTERVAL_MS));
    }
}

esp_err_t psmd_led_init(void)
{
    if (PSMD_MODE_LED_GPIO >= 0) {
        ESP_RETURN_ON_ERROR(gpio_reset_pin(PSMD_MODE_LED_GPIO), TAG, "GPIO %d: reset failed", PSMD_MODE_LED_GPIO);
        ESP_RETURN_ON_ERROR(gpio_set_direction(PSMD_MODE_LED_GPIO, GPIO_MODE_OUTPUT), TAG,
                            "GPIO %d: output mode failed", PSMD_MODE_LED_GPIO);
        set_level(PSMD_MODE_LED_GPIO, PSMD_MODE_LED_ON_LEVEL, false);
    }
    if (PSMD_OUTPUT_LED_GPIO >= 0) {
        ESP_RETURN_ON_ERROR(gpio_reset_pin(PSMD_OUTPUT_LED_GPIO), TAG, "GPIO %d: reset failed", PSMD_OUTPUT_LED_GPIO);
        ESP_RETURN_ON_ERROR(gpio_set_direction(PSMD_OUTPUT_LED_GPIO, GPIO_MODE_OUTPUT), TAG,
                            "GPIO %d: output mode failed", PSMD_OUTPUT_LED_GPIO);
        set_level(PSMD_OUTPUT_LED_GPIO, PSMD_OUTPUT_LED_ON_LEVEL, false);
    }
    return xTaskCreate(led_task, "psmd_led", 2048, NULL, 5, NULL) == pdPASS ? ESP_OK : ESP_ERR_NO_MEM;
}

esp_err_t psmd_led_set_pattern(psmd_led_pattern_t pattern)
{
    s_pattern = pattern;
    return ESP_OK;
}

esp_err_t psmd_led_output(bool on)
{
    if (PSMD_OUTPUT_LED_GPIO >= 0) {
        set_level(PSMD_OUTPUT_LED_GPIO, PSMD_OUTPUT_LED_ON_LEVEL, on);
    }
    return ESP_OK;
}
