# Manual test plan – M2 and M3

**Power Switch and Measurement Device (PSMD)**

> Related: [`../../docs/project.md`](../../docs/project.md), [`../../docs/specification.md`](../../docs/specification.md),
> [`../../docs/decisions.md`](../../docs/decisions.md), [`../../docs/manual.md`](../../docs/manual.md)

> **Owner:** the manual tests that verify M2 (pairing with Home Assistant) and
> M3 (development board, `Operation Mode`, button and LEDs) on the development
> board.
> **Read when needed:** before and while testing a build on the development
> board, and when recording the result of M2 or M3.

> **Status:** historical, kept as the M2/M3 record with its `TC-NN` IDs. The
> live plans are [`manual-power-up.md`](manual-power-up.md),
> [`manual-local-control.md`](manual-local-control.md) and
> [`manual-matter.md`](manual-matter.md); the mapping is in
> [`README.md`](README.md).

## 1. Scope

What the plan covers:

- `Power-up` behavior, `Operation Mode`, the button and the `Operation Mode LED`
  (M3).
- Matter pairing into Home Assistant, reconnect after a restart and
  decommissioning (M2).
- The Matter values Home Assistant shows for PSMD.

Not covered: measurement (`M4`–`M6`), the real `PU`/`MU` (`M7`), electrical
safety and the production board.

## 2. Preconditions

| Item | Value |
|---|---|
| Build | This repository, `CONFIG_PSMD_BOARD_DEV`, ESP-IDF 6.1, ESP-Matter 1.6.1. |
| Board | ESP32-C6 development board. |
| Button | GPIO3, active low, internal pull-up. |
| `Operation Mode LED` | GPIO1, active high. |
| `Output State` | GPIO2, active high, standing in for the relay. |
| Output measurement | Multimeter or LED on GPIO2 and GND (optional but recommended). |
| Home Assistant | Matter integration with its `Matter Server`, same 2.4 GHz network, BLE adapter in range. |
| Serial monitor | Open on the board's USB port, 115200 baud. |

## 3. Log markers

Filter the serial log on `psmd_` and on `chip[SVR]`/`chip[DL]`. The relevant
lines:

| Marker | Meaning |
|---|---|
| `psmd_main: SerialNumber = PSMD-DEV-<MAC>` | The per-unit serial number was stored. |
| `psmd_mode: Operation Mode = <mode> at Power-up` | `Boot Mode` decided. |
| `psmd_mode: Operation Mode = <mode>` | A transition happened. |
| `psmd_button: Short Press` / `Long Press` | Debounced button event (3 s `Press Threshold`). |
| `psmd_output: Output State = ON/OFF` | `Output State` changed. |
| `psmd_main: Commissioning window opened` | A commissioning window is open. |
| `psmd_main: Commissioning complete` | The device joined a `Fabric`. |
| `psmd_main: Commissioning window closed` | The window closed (timeout or abort). |
| `psmd_main: Fabric removed` | A `Fabric` was removed. |
| `psmd_main: Decommissioning` | A `Long Press` in `PAIRING` erased the Fabric. |
| `chip[SVR]: SetupQRCode: [...]` and the manual code | The onboarding payload. |

## 4. Test cases

Run them in order; each one starts from the state the previous case left.

| ID | Purpose | Steps | Expected result |
|---|---|---|---|
| **TC-01** | First `Power-up`, no `Fabric` | Power up a device with no `Fabric`. Watch the LED and the log for 10 s. | `Operation Mode = LOCAL at Power-up`; `Operation Mode LED` blinks fast (0.25 s on / 0.25 s off) for 5 s, then off; QR code and manual code in the log; `SerialNumber = PSMD-DEV-<MAC>`; `Output State = OFF` and GPIO2 low. |
| **TC-02** | `Short Press` in `LOCAL` | Press the button for less than 1 s; press again. | `psmd_button: Short Press`; `Output State = ON` then `OFF`; GPIO2 follows; the LED is unaffected. |
| **TC-03** | `Long Press` to `PAIRING` | Hold the button. Release after the LED starts blinking slowly (about 3 s). Then press shortly once. | `Long Press` after about 3 s; `Operation Mode = PAIRING`; slow blink (0.5 s on / 0.5 s off); `Commissioning window opened` and the onboarding code; the following `Short Press` changes nothing (`Output State` keeps its value, no log line). |
| **TC-04** | `Long Press` in `PAIRING` = decommissioning | Hold the button in `PAIRING`. | `Operation Mode = LOCAL`, `Commissioning window closed`, `Decommissioning`, then the device restarts (Fabric and Wi-Fi credentials erased) and runs TC-01 again: fast blink 5 s, off, QR in the log, `Output State = OFF`. |
| **TC-05** | Pair from Home Assistant | In Home Assistant: Matter, add a device, enter the printed code. Watch the log and the LED. | `Commissioning complete`; `Operation Mode = OPERATIONAL`; the LED is solid within about 2 s; the device appears in Home Assistant. |
| **TC-06** | Matter values in Home Assistant | Open the device's page in Home Assistant. | Vendor `PSMD`, product `PSMD`, hardware `1.0`, software `0.1.0`, serial number `PSMD-DEV-<12 hex>`, where the hex is the board's base MAC; the switch entity is `Off`. |
| **TC-07** | Control from Home Assistant | Switch the entity on, wait 10 s, switch it off. | GPIO2 follows both changes; `Output State = ON` then `OFF` in the log; the entity keeps the new state. |
| **TC-08** | `Short Press` in `OPERATIONAL` | Press the button shortly while the entity is on and while it is off. | `Output State` toggles locally; GPIO2 follows; Home Assistant shows the new state within about 2 s (subscription report); the LED stays solid. |
| **TC-09** | Reconnect after a restart (M2) | Switch the entity on, then power-cycle the board. | No QR code (the device has a `Fabric`); `Operation Mode = OPERATIONAL at Power-up`; fast blink 5 s then solid; the device is reachable from Home Assistant within 60 s; `Output State = OFF` (see TC-10). |
| **TC-10** | `Power-loss Recovery` (REQ-F-003) | After TC-09: check GPIO2 and the entity. | GPIO2 is low and the entity shows `Off`, not the `On` it had before the power cycle. |
| **TC-11** | Decommission from Home Assistant (M2) | In Home Assistant: Settings, Devices and Services, Matter, the device, three-dot menu, Delete. Watch the log, then pair again with TC-05. | `Fabric removed`, `Commissioning window opened`, onboarding code in the log; `Operation Mode = LOCAL`; `Output State = OFF`; pairing again works without a restart. |
| **TC-12** | Decommission when Home Assistant cannot reach the device | Power the board off. Delete the device in Home Assistant (the other path, Settings, Devices and Services, Devices, Delete). Power the board on. Then hold the button into `PAIRING` and hold it again (TC-04), and pair again. | The device keeps its `Fabric` (no QR at boot, pairing fails), which is the case the local decommission exists for. After TC-04 the device is commissionable and pairing succeeds. Record which path left the `Fabric` behind. |
| **TC-13** | `Pairing Timeout` (open question TBD-31) | In `OPERATIONAL`, hold the button into `PAIRING` and wait. | After 3 min: `Commissioning window closed`, `Operation Mode = LOCAL`, LED off. Note: if the window that opened at `Power-up` is still the open one (900 s), the timeout is longer; record what happened. |
| **TC-14** | Identify (optional) | In Home Assistant, identify the device. | `psmd_main: Identify: type 0, effect ...` in the log; no LED change is required. |
| **TC-15** | No orphan node (M2) | Open the `Matter Server` web UI (port 5580). | Exactly one node for PSMD; Home Assistant shows one device, no unavailable duplicates. |

## 5. Record the result

| Field | Value |
|---|---|
| Date | |
| Firmware | commit and build time |
| Board | MAC printed at boot |
| TC-01 – TC-15 | pass / fail / not run, with a note per failure |

A failed case is recorded as an open question or gap in
[`../../docs/decisions.md`](../../docs/decisions.md) when it needs a decision, and the result of
M2 and M3 is recorded in the firmware increment table in
[`../../docs/project.md`](../../docs/project.md) §4.

## 6. Known pitfalls

- Home Assistant has two delete paths. Deleting the device under Settings,
  Devices and Services, Devices removes Home Assistant's view but can leave the
  node in the `Matter Server` fabric; the correct path is the Matter
  integration's device list, three-dot menu, Delete. TC-11 uses the correct
  path, TC-12 the other one.
- Home Assistant merges devices that report the same serial number. All
  development builds use the same `VendorID`/`ProductID`, so the device entry
  depends on the per-unit serial number (`TBD-30`).
- Pairing over BLE needs the window that is open at `Power-up` or via
  `Long Press`; a device that still has a `Fabric` opens no window at
  `Power-up`.
- Measurement is not part of this plan; the Electrical Sensor endpoint arrives
  in `M5`.
