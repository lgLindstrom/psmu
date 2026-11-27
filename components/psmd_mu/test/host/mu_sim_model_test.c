#include <assert.h>
#include <stdio.h>

#include "mu_sim_model.h"

static void expect_load(const psmd_measurement_t *m)
{
    assert(m->voltage_mv == 230000);
    assert(m->current_ma == 870);
    assert(m->apparent_power_mva == 200000);
    assert(m->active_power_mw == 180000);
    assert(m->reactive_power_mvar == 87178);
    assert(m->phase_delay_pct100 == 9000);
    assert(m->frequency_mhz == 50000);
}

int main(void)
{
    psmd_measurement_t m;
    int64_t energy_mwh = 0;

    mu_sim_model_step(false, &m, &energy_mwh);
    assert(m.voltage_mv == 0);
    assert(m.current_ma == 0);
    assert(m.active_power_mw == 0);
    assert(m.energy_mwh == 0);
    assert(energy_mwh == 0);

    mu_sim_model_step(true, &m, &energy_mwh);
    expect_load(&m);
    assert(m.energy_mwh == 250);
    assert(energy_mwh == 250);

    mu_sim_model_step(true, &m, &energy_mwh);
    expect_load(&m);
    assert(m.energy_mwh == 500);
    assert(energy_mwh == 500);

    mu_sim_model_step(false, &m, &energy_mwh);
    assert(m.active_power_mw == 0);
    assert(m.energy_mwh == 500);
    assert(energy_mwh == 500);

    puts("mu_sim_model: ok");
    return 0;
}
