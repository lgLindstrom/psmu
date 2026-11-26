/* PSMD – Output State driver
 *
 * Drives the Output State line: the relay on/off, which stands in for the real
 * Power Unit until it exists (TBD-C18). The Output State LED is on the same
 * line, so there is no LED pin to drive.
 */

#pragma once

#include <stdbool.h>

#include <esp_err.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Configure the Output GPIO as an output and set Output State to OFF (`Power-loss Recovery`). */
esp_err_t psmd_output_init(void);

/** Set Output State: drives the line, so the relay and the Output State LED follow. The last call wins: the button task and the Matter attribute callback both call it. */
esp_err_t psmd_output_set(bool on);

/** The value last set, not a read of the pin. */
bool psmd_output_get(void);

/** Toggle the value last set. */
esp_err_t psmd_output_toggle(void);

#ifdef __cplusplus
}
#endif
