# PSMD – firmware

The ESP-IDF project for PSMD (ESP-IDF 6.1 + ESP-Matter 1.6.1). It builds and
flashes on an ESP32-C6 dev board, which is used only to verify the build and
flash environment; the product's own board is described in the hardware
documentation. The project root is the repository root: `CMakeLists.txt`,
`main/`, `components/`, `sdkconfig.defaults*` and `partitions.csv`. The
documentation index is in [`docs/README.md`](docs/README.md).

## ESP-IDF project standard

- The project root is the repository root and is the directory with the
  top-level `CMakeLists.txt`. Do not add another project level.
- `main/` only connects: application entry point, dependencies and Kconfig. Own
  modules live in `components/psmd_*` with `idf_component_register()` and
  explicit `REQUIRES`/`PRIV_REQUIRES`. Component names and log `TAG`s use the
  `psmd_` prefix.
- Create projects with `idf.py create-project` and add dependencies with
  `idf.py add-dependency`; both go through the `esp-idf` MCP (`create_project`,
  `add_dependency`). Do not hand-write manifests or scaffold files.
- `sdkconfig` is generated. Intentions go in `sdkconfig.defaults`, and
  target-specific values in `sdkconfig.defaults.<target>` (which requires the
  base file). `set-target` is destructive and is run after approval.
- `managed_components/` and `dependencies.lock` are never edited by hand;
  `dependencies.lock` is committed so that the build is reproducible.
- Build, flash and monitor through the `esp-idf` MCP (`build_project`,
  `flash_project`, `monitor_open`), never by hand. Do not source the EIM
  activation script without `-e` and do not wrap commands in `eim run`: both
  write a banner to stdout, which breaks the MCP protocol.

## Build and flash

- Built and flashed through the `esp-idf` MCP, never by hand.
- Target: `esp32c6` (`TBD-C17`).

## Simulation

- `PU` and `MU` sit behind interfaces with a simulated backend, so the firmware
  runs without the real hardware (`TBD-C18`). The increments are in
  [`docs/project.md`](docs/project.md) §4.
- Until the real `PU` exists, `Output State` is the GPIO in
  `CONFIG_PSMD_OUTPUT_GPIO` (default 2). The final pin map is owned by `GAP-02`.
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
- Espressif documentation and paths: [`docs/lookup/esp-idf.md`](docs/lookup/esp-idf.md).
- Matter endpoints and clusters: the local API source is
  `managed_components/espressif__esp_matter/` after the first build; the
  conceptual documentation is in the ESP-Matter Programming Guide.
