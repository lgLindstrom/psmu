#include "mu_sim_model.h"

/* 230 V, 200 VA, PF 0.90 -> 180 W. 180 W for 5 s = 0.25 Wh = 250 mWh. */
#define SIM_ENERGY_PER_CYCLE_MWH 250

/* shortcut: one sample per 5 s cycle; the real MU integrates continuously. */

void mu_sim_model_step(bool load, psmd_measurement_t *sample, int64_t *energy_mwh)
{
    if (load) {
        *sample = (psmd_measurement_t){
            .voltage_mv = 230000,
            .current_ma = 870,
            .apparent_power_mva = 200000,
            .active_power_mw = 180000,
            .reactive_power_mvar = 87178,
            .phase_delay_pct100 = 9000,   /* PowerFactor 0.90 */
            .frequency_mhz = 50000,
        };
        *energy_mwh += SIM_ENERGY_PER_CYCLE_MWH;
    } else {
        *sample = (psmd_measurement_t){0};
    }
    sample->energy_mwh = *energy_mwh;
}
