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
    /** No Fabric: only Short Press controls Output State. */
    PSMD_MODE_LOCAL,
    /** Commissioning window open: Short Press has no effect. */
    PSMD_MODE_PAIRING,
    /** In a Fabric: Home Assistant and Short Press both control Output State. */
    PSMD_MODE_OPERATIONAL,
} psmd_mode_t;

/** All three callbacks are optional; a NULL member is skipped. */
typedef struct {
    /** Output State changed locally; publish it to Matter. */
    void (*on_output_changed)(bool on);
    /** PAIRING entered (open) or left (close) the commissioning window. */
    void (*on_commissioning_window)(bool open);
    /** Long Press in PAIRING: decommission the device (erase Fabric and Wi-Fi). */
    void (*on_decommission)(void);
} psmd_mode_callbacks_t;

/** Start in OPERATIONAL when commissioned, otherwise in LOCAL, and run the 5 s startup Fast Blink. Call once, before psmd_button_init: it creates the timer that ends the blink. */
esp_err_t psmd_mode_init(bool commissioned, const psmd_mode_callbacks_t *callbacks);

/** The current Operation Mode. */
psmd_mode_t psmd_mode_get(void);

/** A commissioning window is open: PAIRING, so the LED slow-blinks for as long as the device can be paired. */
void psmd_mode_pairing(void);

/** Button events: Short Press toggles Output State in LOCAL and OPERATIONAL and is ignored in PAIRING; a Long Press moves LOCAL/OPERATIONAL to PAIRING, and in PAIRING decommissions the device (`TBD-C21`). */
void psmd_mode_button_event(psmd_button_event_t event);

/** Commissioning succeeded: OPERATIONAL. */
void psmd_mode_commissioned(void);

/** The commissioning window closed without commissioning: PAIRING returns to OPERATIONAL when a `Fabric` remains, otherwise to LOCAL; other modes are unchanged. */
void psmd_mode_window_closed(bool commissioned);

/** The last Fabric was removed: LOCAL. The commissioning window that opens next turns this into PAIRING. */
void psmd_mode_fabric_removed(void);

#ifdef __cplusplus
}
#endif
