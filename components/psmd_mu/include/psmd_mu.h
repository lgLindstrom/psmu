/* PSMD – Measurement Unit (MU)
 *
 * One interface for the MU. The MU measures on its own 5 s cycle and has no
 * knowledge of `Operation Mode`; MC reads the latest sample and reports it only
 * in `OPERATIONAL`. The backend is chosen at build time: simulated
 * (`CONFIG_PSMD_MU_SIM`) or the real MU over I2C (`CONFIG_PSMD_MU_I2C`, TBD-28).
 */

#pragma once

#include <esp_err.h>

#include "psmd_measurement.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Start the selected MU backend. A missing MU is not an init failure. */
esp_err_t psmd_mu_init(void);

/** Read the latest sample. `out` is valid only when ESP_OK is returned. */
esp_err_t psmd_mu_read(psmd_measurement_t *out);

#ifdef __cplusplus
}
#endif
