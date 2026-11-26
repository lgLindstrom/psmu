# Tests – PSMD

**Power Switch and Measurement Device (PSMD)**

> Related: [`../../docs/specification.md`](../../docs/specification.md),
> [`../../docs/project.md`](../../docs/project.md), [`../../docs/decisions.md`](../../docs/decisions.md)

> **Owner:** the manual test plans that verify a build on the development board.
> **Read when needed:** before testing a build on the board, and when recording
> the result of an increment.

The tests here are manual unless a plan says otherwise. The requirements and
their acceptance criteria are in [`../../docs/specification.md`](../../docs/specification.md)
§10; the increments and their exit criteria are in
[`../../docs/project.md`](../../docs/project.md) §4.

## 1. Test plans

| Plan | Covers | Test cases | State |
|---|---|---|---|
| [`manual-power-up.md`](manual-power-up.md) | `Boot Mode`, the startup `Fast Blink`, `Output State = OFF` at `Power-up`, the `SerialNumber`, the onboarding code. | `PU-01`–`PU-05` | Run 2026-11-26, pass |
| [`manual-local-control.md`](manual-local-control.md) | Button, `Press Threshold`, debounce, `Operation Mode` transitions, `Operation Mode LED`, `Output State` and its LED on GPIO1. | `LC-01`–`LC-10` | Run 2026-11-26; `LC-04`, `LC-07`, `LC-08` not run |
| [`manual-matter.md`](manual-matter.md) | `Commissioning`, the identity Home Assistant shows, control from Home Assistant, reconnect, decommissioning, the `Matter Server` node, `Pairing Timeout`, removing the device while another administrator exists. | `MT-01`–`MT-11` | Run 2026-11-26; `MT-06`, `MT-07` not run |
| [`manual-m2-m3.md`](manual-m2-m3.md) | Historical: M2 and M3 as they were written and run. | `TC-01`–`TC-15` | Kept as the M2/M3 record; superseded by the three plans above |

There is no plan for measurement yet: it arrives with the simulated `MU` (M4)
and is written when there is something to measure. The two LEDs are covered:
`Output State LED` is on the `Output State` line and follows it (`LC-09`).

The Unity test app in [`../`](../) (`test/`, an ESP-IDF project of its own) is a
bring-up check before the plans: `test/main/test_ui.c` blinks the `Operation Mode
LED` fast and `Output State` slow for 10 s. It asserts the driver calls; the
blink is checked by eye.

| Old case | Now |
|---|---|
| `TC-01` | `PU-01` |
| `TC-02` | `LC-01` |
| `TC-03` | `LC-03`, `LC-05` |
| `TC-04` | `MT-05` |
| `TC-05` | `MT-01` |
| `TC-06` | `MT-02` |
| `TC-07` | `MT-03` |
| `TC-08` | `LC-02` |
| `TC-09` | `PU-02`, `MT-10` |
| `TC-10` | `PU-03` |
| `TC-11` | `MT-06` |
| `TC-12` | `MT-07` |
| `TC-13` | `MT-09` |
| `TC-14` | `MT-04` |
| `TC-15` | `MT-08` |

## 2. Common preconditions

| Item | Value |
|---|---|
| Build | This repository, `CONFIG_PSMD_BOARD_DEV`, ESP-IDF 6.1, ESP-Matter 1.6.1. |
| Board | ESP32-C6 development board. |
| Button | GPIO3, active low, internal pull-up. |
| `Operation Mode LED` | GPIO2, active high. |
| `Output State` | GPIO1, active high, standing in for the relay; the `Output State LED` is on the same line. |
| Output measurement | Multimeter or LED on GPIO1 and GND (optional but recommended). |
| Home Assistant | Matter integration with its `Matter Server`, same 2.4 GHz network, BLE adapter in range. |
| Serial monitor | Open on the board's USB port, 115200 baud. |

## 3. Log markers

Filter the serial log on `psmd_` and on `chip[SVR]`/`chip[DL]`. The relevant
lines:

| Marker | Meaning |
|---|---|
| `psmd_main: SerialNumber = PSMD-DEV-<MAC>` | The per-unit serial number was stored. It is printed only when it changed. |
| `psmd_main: Fabrics at Power-up = <n>` | How many `Fabric`s the device had at boot; 0 means it is commissionable. |
| `psmd_main:   index <i> compressed 0x… fabric 0x… node 0x… vendor 0x…` | One line per `Fabric`, logged at `Power-up`, when a window opens, after `Commissioning complete` and after a removal. The same `compressed` value twice means two entries of the same `Fabric`; two different values mean two controllers. |
| `psmd_mode: Operation Mode = <mode> at Power-up` | `Boot Mode` decided. |
| `psmd_mode: Operation Mode = <mode>` | A transition happened. |
| `psmd_button: Short Press` / `Long Press` | Debounced button event (3 s `Press Threshold`). |
| `psmd_output: Output State = ON/OFF` | `Output State` changed. |
| `psmd_main: Commissioning window opened` | A commissioning window is open. |
| `psmd_main: Commissioning complete` | The device joined a `Fabric`. |
| `psmd_main: Commissioning window closed` | The window closed (timeout or abort). |
| `chip[SVR]: Fabric already commissioned. Disabling BLE advertisement` | The device booted with at least one `Fabric`, so BLE advertising is off. |
| `psmd_main: Fabric removed (<n> left)` | A `Fabric` was removed; `<n>` remain. Only 0 makes the device commissionable again. |
| `psmd_main: Wi-Fi credentials erased` | The last `Fabric` was removed, so the network configuration is gone too. |
| `psmd_main: Decommissioning` | A `Long Press` in `PAIRING` erased the Fabric. |
| `chip[SVR]: SetupQRCode: [...]` and the manual code | The onboarding payload. |

## 4. Conventions

- A test case ID is `<plan prefix>-NN` and unique in the library: `PU` for
  `Power-up`, `LC` for local control, `MT` for Matter. `TC-NN` is used only by
  the historical plan, where it is unique within that plan.
- A case names the requirement it verifies when one exists
  ([`../../docs/specification.md`](../../docs/specification.md) §10).
- A plan records the firmware commit, the board's MAC and pass/fail per case.
- The result of an increment goes into [`../../docs/project.md`](../../docs/project.md) §4.
- A failure that needs a decision is recorded in
  [`../../docs/decisions.md`](../../docs/decisions.md); a missing requirement is a gap there.

## 5. Record the result

| Field | Value |
|---|---|
| Date | |
| Firmware | commit and build time |
| Board | MAC printed at boot |
| Plan | `PU`, `LC` or `MT`, and the cases run |
| Result | pass / fail / not run, with a note per failure |

### Run 2026-11-26

| Field | Value |
|---|---|
| Date | 2026-11-26 |
| Firmware | the commit of 2026-11-26, built via the `esp-idf` MCP; ESP-IDF 6.1, ESP-Matter 1.6.1, `esp32c6` |
| Board | ESP32-C6 development board, MAC `40:4C:CA:57:EC:7C` (`PSMD-DEV-404CCA57EC7C`) |
| Plan | `PU-01`–`PU-05`, `LC-01`–`LC-10`, `MT-01`–`MT-11` |
| Result | 21 pass, 5 not run; notes below |

| Case | Result | Note |
|---|---|---|
| `PU-01` | pass | `LOCAL`, fast blink 5 s, no QR or window, `Output State = OFF` |
| `PU-02` | pass | Boot with two `Fabric`s: `OPERATIONAL`, fast blink then solid, no QR |
| `PU-03` | pass | Entity `ON` before the power cycle, `OFF` after |
| `PU-04` | pass | `SerialNumber` written once; not rewritten after the factory reset |
| `PU-05` | pass | Fast blink about 5 s in both boots |
| `LC-01` | pass | Short Press toggles `Output State`, mode unchanged |
| `LC-02` | pass | Short Press in `OPERATIONAL` toggles and Home Assistant follows |
| `LC-03` | pass | Short Press in `PAIRING` toggles nothing |
| `LC-04` | not run | 2 s / 4 s boundary not measured |
| `LC-05` | pass | Long Press in `LOCAL` gives `PAIRING`, slow blink, window and code |
| `LC-06` | pass | Long Press in `OPERATIONAL` gives `PAIRING` |
| `LC-07` | not run | Held press not measured |
| `LC-08` | not run | Debounce not measured |
| `LC-09` | pass | Slow blink measured at 1 Hz; solid in `OPERATIONAL`; off in `LOCAL` |
| `LC-10` | pass | `Output State` kept `ON` in `PAIRING` |
| `MT-01` | pass | Two `Fabric`s: Google Home (`0x6006`) and the HA `Matter Server` (`0xFFF1`); no `LOCAL` during commissioning |
| `MT-02` | pass with note | Vendor still showed `PSMD` in Home Assistant; the source now sets `Consius AB`, which needs a re-interview |
| `MT-03` | pass | Control from Home Assistant follows |
| `MT-04` | pass with note | Identify is received and logged; no visible reaction, open in `TBD-37` |
| `MT-05` | pass | `Decommissioning`, `reboot-count = 1`, `Fabrics at Power-up = 0`, `LOCAL`, no Wi-Fi |
| `MT-06` | not run | Needs exactly one `Fabric`; the Android app leaves two. `MT-11` covers the same delete |
| `MT-07` | not run | The second Home Assistant delete path was not run |
| `MT-08` | pass | No PSMD node left in the `Matter Server` |
| `MT-09` | pass | Window closed after 3 min; back to `OPERATIONAL`, LED solid, HA still works |
| `MT-10` | pass | Reachable from Home Assistant after a power cycle |
| `MT-11` | pass with note | Google fabric removed, then Delete: `0` fabrics, Wi-Fi erased, window, `LOCAL`; the removal lines were not captured because the monitor was opened later |

A failed case is recorded as an open question or gap in
[`../../docs/decisions.md`](../../docs/decisions.md) when it needs a decision, and the result of
the increment is recorded in the firmware increment table in
[`../../docs/project.md`](../../docs/project.md) §4.
