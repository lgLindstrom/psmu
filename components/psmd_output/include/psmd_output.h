/* PSMD – Output State driver
 *
 * Drives the GPIO that stands in for the relay until the real Power Unit
 * exists (TBD-C18), and the Output State LED when the board has one.
 */

#pragma once

#include <stdbool.h>

#include <esp_err.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Configure the Output GPIO as an output and set Output State to OFF. */
esp_err_t psmd_output_init(void);

/** Set Output State; the Output State LED follows it. */
esp_err_t psmd_output_set(bool on);

/** Read Output State. */
bool psmd_output_get(void);

/** Toggle Output State. */
esp_err_t psmd_output_toggle(void);

#ifdef __cplusplus
}
#endif
