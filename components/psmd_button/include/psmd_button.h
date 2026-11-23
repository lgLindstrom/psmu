/* PSMD – Button
 *
 * Short Press (<= 3 s) and Long Press (> 3 s) on the board's button.
 */

#pragma once

#include <esp_err.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PSMD_BUTTON_EVENT_SHORT_PRESS,
    PSMD_BUTTON_EVENT_LONG_PRESS,
} psmd_button_event_t;

typedef void (*psmd_button_callback_t)(psmd_button_event_t event);

/** Configure the button and start reading it. The callback runs on a task. */
esp_err_t psmd_button_init(psmd_button_callback_t callback);

#ifdef __cplusplus
}
#endif
