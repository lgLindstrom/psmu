# Firmware – PSMD

The ESP-IDF project for PSMD (ESP32-C6 dev board, ESP-IDF 6.1 + ESP-Matter).
The project has not been started yet. Build environment and version decisions
are in [`../docs/specification.md`](../docs/specification.md) §14.

## Build and flash

- Built and flashed through the `esp-idf` MCP (`build_project`, `flash_project`,
  `monitor_open`), never by hand.
- Target: `esp32c6` (`TBD-C17`).

## Simulation

- `PU` and `MU` sit behind interfaces with a simulated backend, so the firmware
  runs without the real hardware (`TBD-C18`). The increments are in
  [`../docs/project.md`](../docs/project.md) §4.
- `CONFIG_PSMD_MU_SIM` selects the simulated `MU` (M4); the tests keep using it
  after the real `MU` is connected (M7).

## Logging

- Log with the ESP-IDF macros `ESP_LOGI`, `ESP_LOGW`, `ESP_LOGE`, `ESP_LOGD`
  and `ESP_LOGV` – not `printf`.
- Every module that logs defines `static const char *TAG`. `TAG` always starts
  with `psmd_` followed by the module name, e.g. `psmd_meas`, so that PSMD's own
  logs can be filtered out of the ESP-IDF and ESP-Matter libraries' output.

## Troubleshooting

- Filter logs on the prefix `psmd_`.
- Espressif documentation and paths: [`../docs/lookup/esp-idf.md`](../docs/lookup/esp-idf.md).
- Matter endpoints and clusters are documented in ESP-Matter, not ESP-IDF:
  `$ESP_MATTER_PATH/docs/en/`.
