#include "psmd_mode.h"

#include <esp_check.h>
#include <esp_log.h>
#include <esp_timer.h>

#include "psmd_led.h"
#include "psmd_output.h"

static const char *TAG = "psmd_mode";

#define STARTUP_BLINK_US (5 * 1000 * 1000)

static psmd_mode_t s_mode = PSMD_MODE_LOCAL;
static psmd_mode_callbacks_t s_callbacks;
static bool s_startup = false;   /* Fast Blink for 5 s at Power-up. */
static esp_timer_handle_t s_startup_timer;

static const char *mode_name(psmd_mode_t mode)
{
    switch (mode) {
    case PSMD_MODE_PAIRING:
        return "PAIRING";
    case PSMD_MODE_OPERATIONAL:
        return "OPERATIONAL";
    case PSMD_MODE_LOCAL:
    default:
        return "LOCAL";
    }
}

static void apply_pattern(void)
{
    if (s_startup) {
        psmd_led_set_pattern(PSMD_LED_PATTERN_FAST_BLINK);
        return;
    }
    switch (s_mode) {
    case PSMD_MODE_PAIRING:
        psmd_led_set_pattern(PSMD_LED_PATTERN_SLOW_BLINK);
        break;
    case PSMD_MODE_OPERATIONAL:
        psmd_led_set_pattern(PSMD_LED_PATTERN_SOLID);
        break;
    case PSMD_MODE_LOCAL:
    default:
        psmd_led_set_pattern(PSMD_LED_PATTERN_OFF);
        break;
    }
}

static void set_mode(psmd_mode_t mode)
{
    if (s_mode == mode) {
        return;
    }
    s_mode = mode;
    ESP_LOGI(TAG, "Operation Mode = %s", mode_name(mode));
    apply_pattern();
}

static void startup_done(void *arg)
{
    s_startup = false;
    apply_pattern();
}

esp_err_t psmd_mode_init(bool commissioned, const psmd_mode_callbacks_t *callbacks)
{
    if (callbacks != NULL) {
        s_callbacks = *callbacks;
    }
    s_mode = commissioned ? PSMD_MODE_OPERATIONAL : PSMD_MODE_LOCAL;
    ESP_LOGI(TAG, "Operation Mode = %s at Power-up", mode_name(s_mode));

    const esp_timer_create_args_t timer_args = {
        .callback = startup_done,
        .name = "psmd_mode_startup",
    };
    ESP_RETURN_ON_ERROR(esp_timer_create(&timer_args, &s_startup_timer), TAG, "timer create failed");

    s_startup = true;
    apply_pattern();
    return esp_timer_start_once(s_startup_timer, STARTUP_BLINK_US);
}

psmd_mode_t psmd_mode_get(void)
{
    return s_mode;
}

void psmd_mode_button_event(psmd_button_event_t event)
{
    switch (event) {
    case PSMD_BUTTON_EVENT_SHORT_PRESS:
        if (s_mode == PSMD_MODE_PAIRING) {
            return; /* No effect in PAIRING. */
        }
        ESP_ERROR_CHECK_WITHOUT_ABORT(psmd_output_toggle());
        if (s_callbacks.on_output_changed != NULL) {
            s_callbacks.on_output_changed(psmd_output_get());
        }
        break;
    case PSMD_BUTTON_EVENT_LONG_PRESS:
        if (s_mode == PSMD_MODE_PAIRING) {
            // Leaving PAIRING with Long Press is decommissioning (TBD-C21).
            set_mode(PSMD_MODE_LOCAL);
            if (s_callbacks.on_commissioning_window != NULL) {
                s_callbacks.on_commissioning_window(false);
            }
            if (s_callbacks.on_decommission != NULL) {
                s_callbacks.on_decommission();
            }
        } else {
            set_mode(PSMD_MODE_PAIRING);
            if (s_callbacks.on_commissioning_window != NULL) {
                s_callbacks.on_commissioning_window(true);
            }
        }
        break;
    }
}

void psmd_mode_commissioned(void)
{
    set_mode(PSMD_MODE_OPERATIONAL);
}

void psmd_mode_window_closed(void)
{
    if (s_mode == PSMD_MODE_PAIRING) {
        set_mode(PSMD_MODE_LOCAL);
    }
}

void psmd_mode_fabric_removed(void)
{
    set_mode(PSMD_MODE_LOCAL);
}
