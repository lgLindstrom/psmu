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
    /** Slow Blink: 0.5 s on / 0.5 s off (1 Hz). */
    PSMD_LED_PATTERN_SLOW_BLINK,
} psmd_led_pattern_t;

/** Configure the Operation Mode LED and start its driver. It starts off. */
esp_err_t psmd_led_init(void);

/** Set the Operation Mode LED pattern. */
esp_err_t psmd_led_set_pattern(psmd_led_pattern_t pattern);

#ifdef __cplusplus
}
#endif
