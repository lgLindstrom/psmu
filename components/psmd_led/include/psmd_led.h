/* PSMD – Operation Mode LED
 *
 * The Operation Mode LED only: its pattern shows `Operation Mode` (specification
 * §6). The Output State LED, on a board that has one, belongs to psmd_output.
 */

#pragma once

#include <esp_err.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PSMD_LED_PATTERN_OFF,
    PSMD_LED_PATTERN_SOLID,
    /** Fast Blink: 0.25 s on / 0.25 s off (2 Hz). */
    PSMD_LED_PATTERN_FAST_BLINK,
    /** Slow Blink: 1 s on / 1 s off (0.5 Hz). */
    PSMD_LED_PATTERN_SLOW_BLINK,
} psmd_led_pattern_t;

/** Configure the Operation Mode LED and start its task (priority 5, 2 KB stack). It starts off. Call once; there is no deinit. */
esp_err_t psmd_led_init(void);

/** Set the pattern: any task may call it, and the task draws it within 10 ms. Returns ESP_OK. */
esp_err_t psmd_led_set_pattern(psmd_led_pattern_t pattern);

#ifdef __cplusplus
}
#endif
