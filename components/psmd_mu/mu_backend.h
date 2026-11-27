/* Private interface between the psmd_mu shell and its backends. */

#pragma once

#include "psmd_mu.h"

esp_err_t mu_backend_init(void);
esp_err_t mu_backend_read(psmd_measurement_t *out);
