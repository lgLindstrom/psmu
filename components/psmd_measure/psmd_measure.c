#include "psmd_measure.h"

#include <inttypes.h>
#include <stdbool.h>

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "psmd_mode.h"
#include "psmd_mu.h"

static const char *TAG = "psmd_measure";

#define MEASURE_INTERVAL_MS 5000
#define MEASURE_TASK_STACK  4096
#define MEASURE_TASK_PRIO   5

static void measure_task(void *arg)
{
    (void)arg;
    bool read_error = false;

    for (;;) {
        if (psmd_mode_get() == PSMD_MODE_OPERATIONAL) {
            psmd_measurement_t m;
            esp_err_t err = psmd_mu_read(&m);

            if (err == ESP_OK) {
                if (read_error) {
                    ESP_LOGI(TAG, "MU available again");
                    read_error = false;
                }
                ESP_LOGI(TAG,
                         "V=%" PRId32 " mV I=%" PRId32 " mA S=%" PRId32 " mVA P=%" PRId32
                         " mW Q=%" PRId32 " mVAR phase=%" PRId32 " F=%" PRId32 " mHz E=%" PRId64
                         " mWh",
                         m.voltage_mv, m.current_ma, m.apparent_power_mva, m.active_power_mw,
                         m.reactive_power_mvar, m.phase_delay_pct100, m.frequency_mhz, m.energy_mwh);
            } else if (!read_error) {
                ESP_LOGW(TAG, "MU read failed: %s", esp_err_to_name(err));
                read_error = true;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(MEASURE_INTERVAL_MS));
    }
}

esp_err_t psmd_measure_start(void)
{
    return xTaskCreate(measure_task, "psmd_measure", MEASURE_TASK_STACK, NULL, MEASURE_TASK_PRIO,
                       NULL) == pdPASS
               ? ESP_OK
               : ESP_ERR_NO_MEM;
}
