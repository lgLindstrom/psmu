# ESP-IDF 6.1 and ESP-Matter – local reference

**Power Switch and Measurement Device (PSMD)**

> Related: [`matter-1.6.1/INDEX.md`](matter-1.6.1/INDEX.md),
> [`specification.md`](../specification.md) §12 and §14

> **Owner:** where the Espressif documentation lives and how it is looked up.
> **Read when needed:** [`matter-1.6.1/INDEX.md`](matter-1.6.1/INDEX.md) for
> Matter-specific clusters and attributes.

The firmware is built with **ESP-IDF 6.1** (decision `TBD-C8`) and the Matter
part with **ESP-Matter** on the branch `release/v1.6.1`. None of Espressif's
material is copied into this repository: the documentation is read from the
installed checkout, which always has the same version as the compiler.

## Where the documentation is

| Source | Path | Used for |
|---|---|---|
| ESP-IDF, source | `$IDF_PATH/docs/en/**/*.rst` | Primary source. The version is the one we build against. |
| ESP-IDF, HTML | `https://docs.espressif.com/projects/esp-idf/en/v6.1/` | When no checkout exists on the machine |
| ESP-IDF, generated HTML | `idf.py docs` in a project, then `build/docs/` | Offline, with the right target |
| ESP-Matter | `$ESP_MATTER_PATH/docs/en/**/*.rst` | Matter clusters, device types, provisioning |
| ESP-Matter, HTML | `https://docs.espressif.com/projects/esp-matter/en/latest/` | Overview; the *Getting the Repositories* section |

ESP-IDF 6.1 is installed with **ESP-IDF Installation Manager (EIM)**, not as a
git clone in the home directory:

```text
$IDF_PATH              /home/lasse/.espressif/v6.1/esp-idf          (734 MB)
$IDF_PYTHON_ENV_PATH   /home/lasse/.espressif/tools/python/v6.1/venv
documentation          $IDF_PATH/docs/en                            (6.9 MB reST)
activation             ~/.espressif/tools/activate_idf_v6.1.sh
```

The same machine also has `~/esp/esp-idf-v4.3` and `~/esp/esp-idf-v5.5` from
older installations. They are not used by PSMD.

## Activating the environment

| Need | Command |
|---|---|
| Interactive shell | `eim shell v6.1` |
| A single command | `eim run 'idf.py build' v6.1` |
| Show installed versions and the selected one | `eim list` |
| Change the selected version | `eim select v6.1` |

`eim` registers each installation in `~/.espressif/tools/eim_idf.json` and
generates an activation script per version. The script has a quiet mode,
`bash ~/.espressif/tools/activate_idf_v6.1.sh -e`, which writes only
`KEY=VALUE` lines – that is the form the MCP launcher uses.

The `esp-idf` MCP server (`.opencode/esp-idf-mcp.sh`) therefore does this
automatically at startup:

1. reads `eim_idf.json` and takes the installation that `eim select` points at
   (`idfSelectedId`), otherwise the highest version; `EIM_IDF_VERSION` overrides,
2. runs the activation script's `-e` and exports all variables, including
   `OPENOCD_SCRIPTS`, `ESP_ROM_ELF_DIR` and the esp-clang paths,
3. falls back to `~/esp/esp-idf-v*` (highest version) when EIM is missing.

Verified: the launcher reports `ESP-IDF v6.1`, and with `eim_idf.json` removed
it falls back to `ESP-IDF v5.5`.

**Two things that must not be done:** sourcing `activate_idf_v6.1.sh` without
`-e`, and running the MCP server through `eim run`/`eim shell`. All three write
a banner to stdout – the MCP protocol runs over stdout and would be broken.
`eim run` is unsuitable as a general wrapper for agent commands for the same
reason.

**ESP-Matter is not fetched yet** on the machine: `$ESP_MATTER_PATH` is not set
and there is no `esp-matter` checkout. The branch `release/v1.6.1` is fetched
according to the *Getting the Repositories* section of ESP-Matter's Programming
Guide when the firmware work begins.

## Where in the documentation PSMD needs to look

The section names below are verified against the API Reference for v6.1. Search
the directory instead of guessing file names; Sphinx moves files between
versions.

| PSMD part | ESP-IDF area | Directory under `$IDF_PATH/docs/en/` |
|---|---|---|
| `Output State`, `Button`, `Operation Mode LED`, `Output State LED` | Peripherals API → GPIO & RTC GPIO | `api-reference/peripherals/` |
| Blink patterns (`Slow Blink`, `Fast Blink`) | Peripherals API → LED Control (LEDC) | `api-reference/peripherals/` |
| Timing, `Pairing Timeout`, `Reconnect` | System API → ESP Timer, System Time | `api-reference/system/` |
| Measurement (`TBD-10`, `TBD-C11`) | Peripherals API → ADC, or an external measurement circuit over I2C/SPI/UART | `api-reference/peripherals/` |
| Stored `Output State`, `Power-up` | Storage API → Non-Volatile Storage Library | `api-reference/storage/` |
| Wi-Fi and provisioning (`TBD-C13`) | Networking APIs → Wi-Fi; Provisioning API | `api-reference/network/`, `api-reference/provisioning/` |
| Matter endpoints and clusters | ESP-Matter, not ESP-IDF | `$ESP_MATTER_PATH/docs/en/` |
| OTA (`TBD-12`) | System API → ESP HTTPS OTA, Over The Air Updates | `api-reference/system/` |
| Logging and troubleshooting | System API → Logging library, Console, Watchdogs | `api-reference/system/` |
| Partition table and app format | API Guides → Partition Tables; System API → App Image Format | `api-guides/`, `api-reference/system/` |

## Searching

```sh
# a type or a field across the whole API reference
rg -n "adc_continuous_config_t" "$IDF_PATH/docs/en/api-reference"

# a section, to jump to the right place
rg -n "^LED Control" "$IDF_PATH/docs/en/api-reference/peripherals"

# Kconfig options (CONFIG_*) are in the config reference
rg -n "CONFIG_ESP_TASK_WDT" "$IDF_PATH/docs/en/api-reference"
```

The same rule as for Matter: **never read a whole tree**. `docs/en` is 6.9 MB
of reST and all of `$IDF_PATH` is 734 MB – never include that path in a
codegraph index or in a read without `rg` and line ranges.

A heading is not normative in itself: check signatures in
`$IDF_PATH/components/*/include/*.h`, which is the code that is actually
compiled.

## Versions and compatibility

| Component | Version | Source |
|---|---|---|
| ESP-IDF | **6.1** | Decision `TBD-C8` |
| ESP-Matter | `release/v1.6.1` (Matter 1.6.1) | [`matter-1.6.1/INDEX.md`](matter-1.6.1/INDEX.md) |
| Matter specification | 1.6.1 | [`matter-1.6.1/INDEX.md`](matter-1.6.1/INDEX.md) |

**Known deviation:** ESP-Matter's README for both `release/v1.6.1` and `main`
recommends ESP-IDF **v6.0.2**, not 6.1. We are thus one step ahead of what
Espressif tests against. The question is registered as `TBD-21` in
[`decisions.md`](../decisions.md).

## Files here

| File | Tracked | Contents |
|---|---|---|
| `esp-idf.md` | yes | This map |

No PDFs, no text extracts and no copy of Espressif's documentation are stored in
the repository. The ESP-IDF documentation is CC-BY-SA 4.0 and may be reproduced
with attribution, but a copy would still become an outdated second-hand source.
