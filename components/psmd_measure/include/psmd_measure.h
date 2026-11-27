/* PSMD – measurement loop
 *
 * Reads the MU every 5 s and reports the sample only in `OPERATIONAL`
 * (specification §8). Reporting to Matter is added in M5.
 */

#pragma once

#include <esp_err.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Start the measurement task. */
esp_err_t psmd_measure_start(void);

#ifdef __cplusplus
}
#endif
