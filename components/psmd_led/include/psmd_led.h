/* PSMD – LEDs
 *
 * The Operation Mode LED pattern and the Output State LED. Both are plain
 * GPIOs; the board decides whether they exist (psmd_board.h).
 */

#pragma once

#include <stdbool.h>

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

/** Configure the LEDs and start their driver. Both start off. */
esp_err_t psmd_led_init(void);

/** Set the Operation Mode LED pattern. */
esp_err_t psmd_led_set_pattern(psmd_led_pattern_t pattern);

/** Set the Output State LED; it follows Output State. A no-op without one. */
esp_err_t psmd_led_output(bool on);

#ifdef __cplusplus
}
#endif
