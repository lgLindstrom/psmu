/* PSMD – Button
 *
 * Short Press (<= 3 s) and Long Press (> 3 s) on the board's button. The button
 * is polled in a task, debounced, and reported as one event per press: Long
 * Press is sent once, about 3 s into the press, and the release that follows it
 * is not reported as a Short Press.
 */

#pragma once

#include <esp_err.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    /** Released before the 3 s Press Threshold. */
    PSMD_BUTTON_EVENT_SHORT_PRESS,
    /** Held past the 3 s Press Threshold; sent once, while still held. */
    PSMD_BUTTON_EVENT_LONG_PRESS,
} psmd_button_event_t;

/** Runs on the button's task, not in an ISR. */
typedef void (*psmd_button_callback_t)(psmd_button_event_t event);

/** Configure the button and start its task (priority 5, 2 KB stack). Call once; there is no deinit. */
esp_err_t psmd_button_init(psmd_button_callback_t callback);

#ifdef __cplusplus
}
#endif
