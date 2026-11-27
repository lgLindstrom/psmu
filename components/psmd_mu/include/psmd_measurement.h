/* PSMD – one measurement sample
 *
 * The values are in the units Matter uses, so neither MC nor the reporting
 * code has to scale them. Seven values are instantaneous; `energy_mwh` is the
 * only accumulated value and is owned by the MU (TBD-C22).
 */

#pragma once

#include <stdint.h>

typedef struct {
    int32_t voltage_mv;
    int32_t current_ma;
    int32_t apparent_power_mva;
    int32_t active_power_mw;
    int32_t reactive_power_mvar;
    int32_t phase_delay_pct100;   /* Reported as PowerFactor, percent100ths. */
    int32_t frequency_mhz;
    int64_t energy_mwh;
} psmd_measurement_t;
