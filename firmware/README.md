# Firmware – PSMD

ESP-IDF-projektet för PSMD (ESP32, ESP-IDF 6.1 + ESP-Matter). Projektet har
inte startats än. Byggmiljö och versionsbeslut finns i
[`../docs/specification.md`](../docs/specification.md) §14.

## Bygg och flash

- Byggs och flashas via `esp-idf`-MCP:n (`build_project`, `flash`, `monitor`),
  inte för hand.
- Hårdvarutarget och byggkonfiguration är inte fastställda.

## Loggning

- Logga med ESP-IDF-makrona `ESP_LOGI`, `ESP_LOGW`, `ESP_LOGE`, `ESP_LOGD`
  och `ESP_LOGV` – inte `printf`.
- Varje modul som loggar definierar `static const char *TAG`. `TAG` börjar
  alltid med `psmd_` följt av modulnamnet, t.ex. `psmd_meas`, så att PSMD:s
  egna loggar kan filtreras fram från ESP-IDF- och ESP-Matter-bibliotekens.

## Felsökning

- Filtrera loggar på prefixet `psmd_`.
- Espressif-dokumentation och sökvägar: [`../docs/lookup/esp-idf.md`](../docs/lookup/esp-idf.md).
- Matter-endpoints och kluster dokumenteras i ESP-Matter, inte ESP-IDF:
  `$ESP_MATTER_PATH/docs/en/`.