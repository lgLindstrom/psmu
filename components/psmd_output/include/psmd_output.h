/* PSMD – Output State driver
 *
 * Drives the GPIO that stands in for the relay until the real Power Unit
 * exists (TBD-C18).
 */

#pragma once

#include <stdbool.h>

#include <esp_err.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Configure CONFIG_PSMD_OUTPUT_GPIO as an output and set it to OFF. */
esp_err_t psmd_output_init(void);

/** Set Output State. */
esp_err_t psmd_output_set(bool on);

#ifdef __cplusplus
}
#endif
