# Manual test plan – Matter and Home Assistant

**Power Switch and Measurement Device (PSMD)**

> Related: [`../../docs/project.md`](../../docs/project.md), [`../../docs/specification.md`](../../docs/specification.md),
> [`../../docs/decisions.md`](../../docs/decisions.md), [`../../docs/manual.md`](../../docs/manual.md),
> [`README.md`](README.md)

> **Owner:** the manual tests that verify `Commissioning`, the device Home
> Assistant sees, control from Home Assistant and decommissioning.
> **Read when needed:** before and while testing a build on the development
> board, and when recording the result.

## 1. Scope

Covers [`../../docs/specification.md`](../../docs/specification.md) §4 and §12–13:

- `Commissioning` over BLE into the `Matter Server` on the test network.
- The identity and the mandatory attributes Home Assistant shows.
- `Output State` controlled from Home Assistant.
- Reconnect after a restart.
- Decommissioning, both the local path and the two paths in Home Assistant.
- The `Pairing Timeout` and Identify.

Not covered: boot behavior without a `Fabric` (see
[`manual-power-up.md`](manual-power-up.md)), the button and the local
transitions (see [`manual-local-control.md`](manual-local-control.md)),
measurement (M4–M6), and the production board (`GAP-02`).

## 2. Preconditions

Everything is in [`README.md`](README.md) *Common preconditions*. The
onboarding code is read from the serial log. `MT-01` starts from a device with
no `Fabric`; the later cases say what state they need.

## 3. Test cases

| ID | Purpose | Steps | Expected result |
|---|---|---|---|
| **MT-01** | Pair from Home Assistant | On a device with no `Fabric`, hold the button into `PAIRING` (`LC-05`). In the Home Assistant Companion app (the web UI cannot commission over BLE; a host with Bluetooth can), add a device via Matter and enter the code printed at `Power-up` (`PU-01`) or the one printed when `PAIRING` opened. Watch the log and the LED. | `Commissioning complete`; `Operation Mode = OPERATIONAL`; the LED is solid within about 2 s; the device appears in Home Assistant. After a reboot, `Fabrics at Power-up = 1`; `2` means the controller commissioned twice – compare the logged `compressed` values (`TBD-32`). |
| **MT-02** | Identity in Home Assistant | Open the device's page. | Vendor `Consius AB`, product `PSMD`, hardware `1.0`, software `0.1.0`, serial number `PSMD-DEV-<12 hex>`, where the hex is the board's base MAC (`TBD-30`); the switch entity is `Off`. |
| **MT-03** | Control from Home Assistant | Switch the entity on, wait 10 s, switch it off. | GPIO1 follows both changes; `Output State = ON` then `OFF` in the log; the entity keeps the new state. |
| **MT-04** | Identify | In Home Assistant, identify the device. | `psmd_main: Identify: type 0, effect ...` in the log; no LED change is required. |
| **MT-05** | Decommission with `Long Press` | In `PAIRING`, hold the button for a `Long Press`. | `Operation Mode = LOCAL`, `Commissioning window closed`, `Decommissioning`, then the device restarts (Fabric and Wi-Fi credentials erased) and runs `PU-01` again: `Operation Mode = LOCAL at Power-up`, fast blink 5 s, off, no QR code, `Output State = OFF`, `reboot-count = 1`, `Fabrics at Power-up = 0` and no Wi-Fi. The `SerialNumber` is not rewritten: it survives the reset on this board (`PU-04`). Pair again with `MT-01`. |
| **MT-06** | Decommission from Home Assistant | Start from `PU-01` with exactly one `Fabric` (`MT-05` first if the device has more) and no PSMD node left in the `Matter Server`. In Home Assistant: Settings, Devices and Services, Matter, the device, three-dot menu, Delete. Watch the log, then pair again with `MT-01`. | `Fabric removed (0 left)`, `Commissioning window opened` without a `Long Press`, onboarding code in the log, `Operation Mode = PAIRING` with a slow blink, `Wi-Fi credentials erased`; `Output State = OFF`; pairing again works without a restart. |
| **MT-07** | Decommission when Home Assistant cannot reach the device | Power the board off. Delete the device in Home Assistant (the other path: Settings, Devices and Services, Devices, Delete). Power the board on. Then hold the button into `PAIRING` and hold it again (`MT-05`), and pair again. | The device keeps its `Fabric` (no QR at boot, pairing fails), which is the case the local decommission exists for. After `MT-05` the device is commissionable and pairing succeeds. Record which path left the `Fabric` behind. |
| **MT-08** | No orphan node | Open the `Matter Server` web UI (port 5580). | Exactly one node for PSMD; Home Assistant shows one device, no unavailable duplicates. |
| **MT-09** | `Pairing Timeout` (`TBD-31`) | In `OPERATIONAL`, hold the button into `PAIRING` and wait. | After 3 min: `Commissioning window closed` and `Operation Mode = OPERATIONAL` (a `Fabric` remains), LED solid; on a device with no `Fabric`, `LOCAL` with the LED off. Record what happened. |
| **MT-10** | Reconnect after a restart | With the device paired and the entity on, power-cycle the board. | No QR code; `Operation Mode = OPERATIONAL at Power-up`; fast blink 5 s then solid; the device is reachable from Home Assistant within 60 s; `Output State = OFF` (see `PU-03`). |
| **MT-11** | Remove when another administrator exists | Pair with the Android Companion app, which leaves Google Home and Home Assistant as two `Fabric`s (`TBD-32`). In Home Assistant: Settings > Connectivity > Matter > Devices > PSMD > ⋮ > Manage fabrics > remove **Google LLC**, then ⋮ > Delete. | After removing Google: `Fabric removed (1 left)`, no commissioning window, `Operation Mode` stays `OPERATIONAL`; after Delete: `Fabric removed (0 left)`, `Commissioning window opened`, `Operation Mode = PAIRING` with a slow blink and `Wi-Fi credentials erased`. The device is fully removed without the Google Home app (`TBD-35`, `TBD-36`). |

## 4. Known pitfalls

- Home Assistant has two delete paths. Deleting the device under Settings,
  Devices and Services, Devices removes Home Assistant's view but can leave the
  node in the `Matter Server` fabric; the correct path is the Matter
  integration's device list, three-dot menu, Delete. `MT-06` uses the correct
  path, `MT-07` the other one.
- Home Assistant merges devices that report the same serial number. All
  development builds use the same `VendorID`/`ProductID`, so the device entry
  depends on the per-unit serial number (`TBD-30`).
- Pairing over BLE needs the window that is open at `Power-up` or the one a
  `Long Press` opens; a device that still has a `Fabric` opens no window at
  `Power-up`.
- A `Long Press` in `PAIRING` factory-resets a paired device. To leave `PAIRING`
  without decommissioning, wait for the `Pairing Timeout` (`MT-09`).
- If another controller or service holds a `Fabric` (multi-admin), deleting the
  device in Home Assistant removes only Home Assistant's: the log says
  `Fabric removed (1 left)` or more, no commissioning window opens, and PSMD
  stays `OPERATIONAL` and is still commissioned. Record the log and stop the
  case; whether PSMD should support this case at all, and how it should behave,
  is open (`TBD-32`).
- Home Assistant can keep the old node in the `Matter Server` after a device is
  deleted. The add-device flow then shows the previous device and does not
  continue, even when PSMD is clean (`Fabrics at Power-up = 0`) and blinking
  slowly in `PAIRING`. Remove PSMD from the `Matter Server` web UI (port 5580)
  or restart the server before pairing; the device cannot clear it (`TBD-32`).
- Delete the device in Home Assistant **before** a factory reset or a
  `Decommissioning`. A factory reset first leaves an orphan node in the
  `Matter Server` (Home Assistant issue `home-assistant/core#174891`), and the
  stale node then blocks the next pairing until it is removed in the server or
  the server is restarted.
- A `Long Press` while the window from `Power-up` is still open logs
  `Commissioning window already open` instead of opening a new one; the 3 min
  `PAIRING` timeout then does not apply (`TBD-31`).
- One pairing can produce two `Commissioning completed successfully` lines and
  two `Fabric`s, because the controller opened a second window right after the
  first completion – seen with a single "Add device" press, so it is the
  controller, not PSMD (`TBD-32`). Check `Fabrics at Power-up` after a reboot;
  `2` means the controller did it, not PSMD. Matter requires at least five
  `Fabric`s, so starting from one is a test precondition for determinism, not a
  product limit, and PSMD must not remove the extra one. A different
  `chip-config/country-code` in the last session (`XX` where the earlier ones
  had `SE`) points to a different commissioner client.
- Pairing needs Bluetooth (`MT-01`), so the Home Assistant web UI cannot
  commission a device; the Companion app can, and its flow may add the phone's
  ecosystem as a second administrator. Compare the logged `compressed` values:
  different values are two controllers (normal multi-admin), the same value
  twice is a duplicate of one `Fabric` (`TBD-32`).
- Seen 2026-11-26: the two `Fabric`s are Google Home (`vendor 0x6006`) and
  Home Assistant's `Matter Server` (`vendor 0xFFF1`); the Android Companion app
  commissions through Google Home and shares the device with Home Assistant, so
  two are expected. To run `MT-06` from one `Fabric`, remove the device from
  Google Home first (or factory reset and pair without the app).
- Between `Device completed Rendezvous process` and `Commissioning complete` the
  log shows `Commissioning window closed` followed by
  `Operation Mode = LOCAL`, because CHIP closes the window as soon as the PASE
  session is established. The device is still being provisioned and the LED is
  off for some seconds; whether the mode should stay `PAIRING` until the outcome
  is open (`TBD-34`).
