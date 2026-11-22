/* PSMD – Operation Mode
 *
 * LOCAL, PAIRING and OPERATIONAL (specification §3–4), the Operation Mode LED
 * and the button's effect on them. The Matter side (fabric, commissioning
 * window) is reached through the callbacks, so this module holds the state
 * machine only.
 */

#pragma once

#include <stdbool.h>

#include <esp_err.h>

#include "psmd_button.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PSMD_MODE_LOCAL,
    PSMD_MODE_PAIRING,
    PSMD_MODE_OPERATIONAL,
} psmd_mode_t;

typedef struct {
    /** Output State changed locally; publish it to Matter. */
    void (*on_output_changed)(bool on);
    /** PAIRING entered (open) or left (close) the commissioning window. */
    void (*on_commissioning_window)(bool open);
} psmd_mode_callbacks_t;

/** Start in OPERATIONAL when commissioned, otherwise in LOCAL. */
esp_err_t psmd_mode_init(bool commissioned, const psmd_mode_callbacks_t *callbacks);

psmd_mode_t psmd_mode_get(void);

/** Button events: Short Press toggles Output State, Long Press moves mode. */
void psmd_mode_button_event(psmd_button_event_t event);

/** Commissioning succeeded. */
void psmd_mode_commissioned(void);

/** The commissioning window closed without commissioning. */
void psmd_mode_window_closed(void);

/** The fabric was removed. */
void psmd_mode_fabric_removed(void);

#ifdef __cplusplus
}
#endif
