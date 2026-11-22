#include "psmd_button.h"

#include <stdbool.h>
#include <stdint.h>

#include <driver/gpio.h>
#include <esp_check.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "psmd_board.h"

static const char *TAG = "psmd_button";

#define BUTTON_POLL_INTERVAL_MS 10
#define BUTTON_DEBOUNCE_US      (30 * 1000)
#define BUTTON_PRESS_THRESHOLD_US (3 * 1000 * 1000)

static psmd_button_callback_t s_callback;

static bool is_pressed(void)
{
    return gpio_get_level(PSMD_BUTTON_GPIO) == PSMD_BUTTON_PRESSED_LEVEL;
}

static void button_task(void *arg)
{
    bool raw = false;              /* last reading */
    bool pressed = false;          /* debounced state */
    int64_t raw_since = esp_timer_get_time();
    int64_t pressed_at = 0;
    bool long_press_sent = false;

    for (;;) {
        int64_t now = esp_timer_get_time();
        bool reading = is_pressed();

        if (reading != raw) {
            raw = reading;
            raw_since = now;
        }
        if (raw != pressed && now - raw_since >= BUTTON_DEBOUNCE_US) {
            pressed = raw;
            if (pressed) {
                pressed_at = now;
                long_press_sent = false;
            } else if (!long_press_sent) {
                ESP_LOGI(TAG, "Short Press");
                s_callback(PSMD_BUTTON_EVENT_SHORT_PRESS);
            }
        }
        if (pressed && !long_press_sent && now - pressed_at >= BUTTON_PRESS_THRESHOLD_US) {
            long_press_sent = true;
            ESP_LOGI(TAG, "Long Press");
            s_callback(PSMD_BUTTON_EVENT_LONG_PRESS);
        }
        vTaskDelay(pdMS_TO_TICKS(BUTTON_POLL_INTERVAL_MS));
    }
}

esp_err_t psmd_button_init(psmd_button_callback_t callback)
{
    ESP_RETURN_ON_FALSE(callback != NULL, ESP_ERR_INVALID_ARG, TAG, "no callback");
    s_callback = callback;

    ESP_RETURN_ON_ERROR(gpio_reset_pin(PSMD_BUTTON_GPIO), TAG, "GPIO %d: reset failed", PSMD_BUTTON_GPIO);
    ESP_RETURN_ON_ERROR(gpio_set_direction(PSMD_BUTTON_GPIO, GPIO_MODE_INPUT), TAG,
                        "GPIO %d: input mode failed", PSMD_BUTTON_GPIO);
    ESP_RETURN_ON_ERROR(gpio_set_pull_mode(PSMD_BUTTON_GPIO, GPIO_PULLUP_ONLY), TAG,
                        "GPIO %d: pull-up failed", PSMD_BUTTON_GPIO);

    return xTaskCreate(button_task, "psmd_button", 2048, NULL, 5, NULL) == pdPASS ? ESP_OK : ESP_ERR_NO_MEM;
}
