# Firmware – PSMD

The ESP-IDF project for PSMD (ESP32, ESP-IDF 6.1 + ESP-Matter). The project has
not been started yet. Build environment and version decisions are in
[`../docs/specification.md`](../docs/specification.md) §14.

## Build and flash

- Built and flashed through the `esp-idf` MCP (`build_project`, `flash`,
  `monitor`), never by hand.
- Hardware target and build configuration are not decided.

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
