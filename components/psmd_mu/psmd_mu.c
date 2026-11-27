#include "psmd_mu.h"

#include <esp_check.h>

#include "mu_backend.h"

static const char *TAG = "psmd_mu";

esp_err_t psmd_mu_init(void)
{
    return mu_backend_init();
}

esp_err_t psmd_mu_read(psmd_measurement_t *out)
{
    ESP_RETURN_ON_FALSE(out != NULL, ESP_ERR_INVALID_ARG, TAG, "no output");
    return mu_backend_read(out);
}
