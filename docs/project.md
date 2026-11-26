# Project plan – PSMD

**Power Switch and Measurement Device (PSMD)**

> Related: [`manual.md`](manual.md), [`specification.md`](specification.md),
> [`decisions.md`](decisions.md), [`glossary.md`](glossary.md)

> **Owner:** what is done when – phases, order, dependencies and exit criteria.
> **Read when needed:** [`decisions.md`](decisions.md) for the open questions
> and gaps (`TBD-NN`, `GAP-NN`), [`README.md`](README.md) for the document map.

There are no dates. The plan gives the order, what each phase needs before it
starts and what it must produce before the next one starts.

## 1. How work flows

[`manual.md`](manual.md) owns the behavior; the specification and the glossary
are derived from it. A function that is added or removed starts in the manual,
and the same change updates the documents it reaches:

1. Change [`manual.md`](manual.md) first: what the user sees and does.
2. If a value is unknown, leave the placeholder and add or update the entry in
   [`decisions.md`](decisions.md). Do not guess.
3. Derive the change in [`specification.md`](specification.md) §3–10 and fill in
   the `Manual` column, and in [`glossary.md`](glossary.md) when a term is
   involved.
4. Update requirements and acceptance criteria in `specification.md` §10 when
   the behavior changed.
5. If the manual states something the specification cannot hold yet, record a
   gap. It is filled in later, not invented now.

The manual is not finished: functions can still be added and removed. Until it
is, the specification is derived from what the manual states today, and every
change to the manual is a change to the specification.

## 2. State (2026-11-22)

| Document | State |
|---|---|
| [`manual.md`](manual.md) | Draft. Button, LEDs, `Operation Mode` and `Power-up` are settled. About 40 placeholders, in §1, §2.2–2.3, §3, §4.1, §4.4–4.5, §5.1 (connectors), §5.4, §6.2–6.3, §7, §8 (last row), §9 and §11. |
| [`specification.md`](specification.md) | Behavior derived from the manual, with a `Manual` column in §3–10. Architecture, data model and Matter device types in place. |
| [`glossary.md`](glossary.md) | Terms in the manual and internal terms are separated; terms follow the manual as it grows. |
| [`decisions.md`](decisions.md) | Open questions and gaps with IDs and sources; the IDs move to **Decided questions** as they are closed. |
| [`hardware.md`](hardware.md) | Stub. Scope is set; the values are open. |
| [`main/`](../main/), [`components/`](../components/) | M0 and M1 done: the ESP-IDF project is the repository root and builds, flashes and boots on the ESP32-C6 dev board with `esp_matter` 1.6.1; one On/Off Plug-in Unit endpoint switches the `Output` GPIO and the commissioning window opens. The device prints its onboarding code when `PAIRING` opens, and pairing into the Matter Server on the test network works (tested 2026-11-26). M2 is done: reconnect after a reboot and decommissioning, both locally and from Home Assistant, are verified. M3 is verified on the board (2026-11-26) with the notes in its increment row. |

What remains in each document is its open questions and gaps, defined in
[`decisions.md`](decisions.md); the firmware increments are in the table below.

## 3. Phases

| # | Phase | Goal | Exit criteria | Depends on |
|---|---|---|---|---|
| 1 | Steering | The manual owns the behavior and the other documents derive from it. | No document contradicts the order in §1; `scripts/check_docs.py` passes. | – (done 2026-11-16) |
| 2 | Manual v1 | The manual covers the first version of the product. | No placeholder that waits for an answer from the product owner remains in chapters 1–8 and 11. `GAP-05` closed: the UI section is removed or clarified. | Answers from K1 and K4 |
| 3 | Specification detail | The specification is implementable. | `GAP-01` and `GAP-03` closed: endpoints, clusters, attributes, units, reporting, and the data model per field. `GAP-06` filled in. | 1, and the manual sections involved |
| 4 | Hardware interface | The interface between hardware and software is fixed. | `GAP-02` closed: the MU I2C interface (`TBD-28`), pin map for the button, the LEDs and the `Output`, active level, debounce and calibration. `GAP-07` closed: connectors, ratings, environment and technical data. | MU interface specification, hardware from K4 |
| 5 | Verification | Every requirement has an acceptance test. | `GAP-04` closed: non-functional requirements and one concrete test per requirement. | 3 |
| 6 | Firmware, simulated | PSMD behaves as specified on an ESP32-C6 dev board, with a simulated `PU` and `MU`. | M0–M6 are done: it builds and flashes, is commissioned in Home Assistant, the behavior in `manual.md` §4–§7 is reproducible on the dev board, and the simulated measurement is visible in Home Assistant. | M0–M2: none. The rest: the part of the specification detail and verification that the increment touches. |
| 7 | Real hardware | The simulated `PU` and `MU` are replaced by the real ones without changing application logic. | M7 is done: the same behavior and the same acceptance tests, with only the `mu_i2c` backend and the pin map changed. | 4, 6 |

Phase 6 is where the firmware work starts. Phases 2–5 continue in parallel and
are closed per increment: an increment that touches the Matter interface updates
the part of `GAP-01` it uses, an increment that measures updates the part of
`GAP-03` it uses, and so on. `GAP-07` and the electrical limits do not block
phase 6, because the dev board and the simulation have no mains side.

## 4. Firmware increments

This is the order inside phase 6. Every increment ends with something observable
on the dev board or in Home Assistant, and leaves the documents it touches
updated.

| # | Increment | What | Exit criteria |
|---|---|---|---|
| **M0** (done 2026-11-19) | Environment | ESP-IDF 6.1 with target `esp32c6`; ESP-Matter 1.6.1 as the component `espressif/esp_matter` from the ESP Component Registry; `TBD-21` settled. | The `psmd` project builds via the `esp-idf` MCP. |
| **M1** (done 2026-11-19) | Matter on/off | ESP-IDF project at the repository root: one endpoint, On/Off Plug-in Unit (`0x010A`), `OnOff`, `Output State` on a GPIO. No PSMD logic yet. | Builds and flashes; the device boots and opens the commissioning window. |
| **M2** (done 2026-11-26) | Pairing with Home Assistant | `Commissioning` over BLE into the Matter Server on the test network (`TBD-29`). | Home Assistant switches `Output State`; the device reconnects after a reboot; decommissioning works. |
| **M3** (verified 2026-11-26) | Development board and local control | Board variants (`CONFIG_PSMD_BOARD`): the development board (`CONFIG_PSMD_BOARD_DEV`) has the button on GPIO3 (active low), the `Operation Mode LED` on GPIO2 (active high), and `Output State` on GPIO1 (active high) with the `Output State LED` on the same line; the production board is a placeholder whose pin map is open. Matter's device identity and mandatory attribute values (`TBD-30`), `Operation Mode`, transitions, button, LEDs and `Power-up`. No measurement in this increment (`M5`–`M6`). | Home Assistant shows PSMD's vendor, product and version attributes, and `Output State` follows the button and Home Assistant; the LED patterns and the 3 s `Press Threshold` in `manual.md` §5.2–§5.3 are reproducible on the dev board; `Output State = OFF` at `Power-up`. Decommissioning works as in [`../test/manual/manual-matter.md`](../test/manual/manual-matter.md), and the boot, button and LED behavior as in [`../test/manual/manual-power-up.md`](../test/manual/manual-power-up.md) and [`../test/manual/manual-local-control.md`](../test/manual/manual-local-control.md). Run 2026-11-26: 21 pass, 5 not run (`LC-04`, `LC-07`, `LC-08`, `MT-06`, `MT-07`); the vendor name needs a re-interview (`TBD-30`) and Identify has no visible reaction yet (`TBD-37`). See [`../test/manual/README.md`](../test/manual/README.md). |
| **M4** | Simulated `MU` | One interface for the `MU` (`mu_init`, `mu_read`) and a simulated backend (`CONFIG_PSMD_MU_SIM`): 5 s cycle, deterministic load, 0 with no `Load`, measurement only in `OPERATIONAL`. | The simulated values appear in the log; a host test covers scaling and energy accumulation. |
| **M5** | Measurement in Matter | Electrical Sensor endpoint, Electrical Power Measurement (voltage, current, active power) and Power Topology. | Home Assistant shows the values, they follow the simulator and are reported every 5 s. |
| **M6** | Energy | Electrical Energy Measurement, `CumulativeEnergyImported`, `TBD-12`. | Home Assistant shows the energy increasing, and the behavior across `Power-up` follows the decision. |
| **M7** | Real hardware | The `mu_i2c` backend against the MU interface (`TBD-28`), the pin map and the calibration (`GAP-02`), and the real `PU` replace the simulation. | The same tests pass on the real device; only the backend and the pin map changed. |

The simulator is not deleted in M7: it stays as a backend for tests.

## 5. Work items

The gap IDs (`GAP-NN`) are defined in [`decisions.md`](decisions.md); the phase
each one is closed in is given by its exit criterion in the phase table above.
Open questions (`TBD-NN`) are worked in the phase of the document they affect
and are defined in [`decisions.md`](decisions.md) too; they are not repeated
here.
