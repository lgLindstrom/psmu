#include <sdkconfig.h>

#if CONFIG_PSMD_MU_SIM

#include "mu_backend.h"

#include <esp_check.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>

#include "mu_sim_model.h"
#include "psmd_output.h"

static const char *TAG = "psmd_mu";

#define SIM_INTERVAL_US (5 * 1000 * 1000)

/* Written by the timer callback, read by psmd_mu_read(). */
static psmd_measurement_t s_sample;
/* shortcut: the simulated accumulator is in RAM, so an MC power cycle resets it;
 * the real MU owns the value across MC power-up (TBD-12, M7). */
static int64_t s_energy_mwh;
static portMUX_TYPE s_lock = portMUX_INITIALIZER_UNLOCKED;
static esp_timer_handle_t s_timer;

static void sim_tick(void *arg)
{
    (void)arg;
    bool load = psmd_output_get();
    psmd_measurement_t sample;
    mu_sim_model_step(load, &sample, &s_energy_mwh);

    taskENTER_CRITICAL(&s_lock);
    s_sample = sample;
    taskEXIT_CRITICAL(&s_lock);
}

esp_err_t mu_backend_init(void)
{
    const esp_timer_create_args_t args = {
        .callback = sim_tick,
        .name = "psmd_mu_sim",
    };
    ESP_RETURN_ON_ERROR(esp_timer_create(&args, &s_timer), TAG, "timer create failed");
    ESP_RETURN_ON_ERROR(esp_timer_start_periodic(s_timer, SIM_INTERVAL_US), TAG, "timer start failed");

    ESP_LOGI(TAG, "Simulated MU on a 5 s cycle");
    return ESP_OK;
}

esp_err_t mu_backend_read(psmd_measurement_t *out)
{
    taskENTER_CRITICAL(&s_lock);
    *out = s_sample;
    taskEXIT_CRITICAL(&s_lock);
    return ESP_OK;
}

#endif
