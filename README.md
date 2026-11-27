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

## Board variants

- The board is chosen with `CONFIG_PSMD_BOARD` (`components/psmd_board/Kconfig`); the
  pins and active levels of each board are in
  `components/psmd_board/include/psmd_board.h`.
- `CONFIG_PSMD_BOARD_DEV` (default) is the ESP32-C6 development board: button
  GPIO3 (active low), `Operation Mode LED` GPIO2 (active high) and `Output
  State` on GPIO1 (active high). The `Output State LED` is on the `Output State`
  line, so there is no separate LED pin.
- `CONFIG_PSMD_BOARD_PRODUCTION` is a placeholder: its pin map is open
  (`GAP-02`), so selecting it stops the build with a message instead of
  inventing pins.

## Simulation

- `PU` and `MU` sit behind interfaces with a simulated backend, so the firmware
  runs without the real hardware (`TBD-C18`). The increments are in
  [`docs/project.md`](docs/project.md) §4.
- Until the real `PU` exists, `Output State` is the board's output line: on
  the development board GPIO1.
- `CONFIG_PSMD_MU_SIM` selects the simulated `MU` (M4); `CONFIG_PSMD_MU_I2C`
  selects the real backend and stops the build until the interface in `TBD-28`
  exists. The simulated `MU` measures on its own 5 s cycle, independent of
  `Operation Mode`; `MC` reads it only in `OPERATIONAL`. The tests keep using
  the simulator after M7.
- The simulated `MU` models a fixed 180 W load while `Output State` is `ON`;
  `OFF` gives 0 instantaneous values and the accumulated energy stays.
- The simulated accumulator is in RAM, so a dev-board power cycle resets it.
  The real `MU` owns the value across `MC` power-up (`TBD-12`).
- `components/psmd_mu/test/host/run.sh` checks the simulated values and energy
  accumulation on the host.

## Logging

- Log with the ESP-IDF macros `ESP_LOGI`, `ESP_LOGW`, `ESP_LOGE`, `ESP_LOGD`
  and `ESP_LOGV` – not `printf`.
- Every module that logs defines `static const char *TAG`. `TAG` always starts
  with `psmd_` followed by the module name, e.g. `psmd_meas`, so that PSMD's own
  logs can be filtered out of the ESP-IDF and ESP-Matter libraries' output.

## Code documentation

- The headers are the API: every `include/*.h` opens with a short comment saying
  what the module is and why it exists, and every exported function, type and
  enum value has a `/** ... */` comment stating its contract – what it does,
  what it returns, units, ranges and caller constraints (task, ISR). It does not
  restate the signature.
- The code is self-documenting: small functions and names from
  [`docs/glossary.md`](docs/glossary.md) and the specification (`Output State`,
  not `led1`). A comment that repeats the code is a rename waiting to happen.
- A comment in a `.c` file says only what the code cannot: why, hardware
  behaviour, timing, an error contract. One line, in English.
- No commented-out code, and no `TODO` without a decision ID from
  [`docs/decisions.md`](docs/decisions.md).

## Troubleshooting

- Filter logs on the prefix `psmd_`.
- Espressif documentation and paths: [`docs/lookup/esp-idf.md`](docs/lookup/esp-idf.md).
- Matter endpoints and clusters: the local API source is
  `managed_components/espressif__esp_matter/` after the first build; the
  conceptual documentation is in the ESP-Matter Programming Guide.
