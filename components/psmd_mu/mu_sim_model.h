/* The pure part of the simulated MU: one 5 s measurement cycle. Kept free of
 * IDF calls so the host test can run it. */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "psmd_measurement.h"

/** Advance the simulated MU one 5 s cycle. `energy_mwh` is its accumulator. */
void mu_sim_model_step(bool load, psmd_measurement_t *sample, int64_t *energy_mwh);
