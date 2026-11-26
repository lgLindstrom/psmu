# Manual test plan – `Power-up`

**Power Switch and Measurement Device (PSMD)**

> Related: [`../../docs/project.md`](../../docs/project.md), [`../../docs/specification.md`](../../docs/specification.md),
> [`../../docs/decisions.md`](../../docs/decisions.md), [`../../docs/manual.md`](../../docs/manual.md),
> [`README.md`](README.md)

> **Owner:** the manual tests that verify what the device does at `Power-up`:
> `Boot Mode`, the startup pattern, `Output State`, the `SerialNumber` and the
> onboarding code.
> **Read when needed:** before and while testing a build on the development
> board, and when recording the result.

## 1. Scope

Covers [`../../docs/specification.md`](../../docs/specification.md) §6 and §7:

- `Boot Mode`: `LOCAL` the first time, `OPERATIONAL` when the device has a
  `Fabric`.
- The `Fast Blink` for 5 s at `Power-up`.
- `Output State = OFF` after every `Power-up` (`Power-loss Recovery`,
  `REQ-F-003`), so the `Output State LED` on the same line is off too.
- The per-unit `SerialNumber`, stored once.
- The onboarding code, printed only while the device has no `Fabric`.

Not covered: pairing itself (see [`manual-matter.md`](manual-matter.md)), the
button and the mode transitions (see
[`manual-local-control.md`](manual-local-control.md)), and measurement (M4–M6).

## 2. Preconditions

Everything is in [`README.md`](README.md) *Common preconditions*. Each case says
what the device state must be, because that is what the case varies.

## 3. Test cases

| ID | Purpose | Steps | Expected result | REQ |
|---|---|---|---|---|
| **PU-01** | First `Power-up`, no `Fabric` | Power up a device with no `Fabric` (a fresh board, or after `MT-05`). Watch the LEDs and the log for 10 s, then hold the button into `PAIRING` (`LC-05`). | `Operation Mode = LOCAL at Power-up`; `Operation Mode LED` fast blinks (0.25 s on / 0.25 s off) for about 5 s, then off; **no QR code and no commissioning window**; `SerialNumber = PSMD-DEV-<MAC>`; `Output State = OFF`, GPIO1 low and the `Output State LED` off. The code is printed when `PAIRING` opens (`TBD-33`). | `REQ-F-003` |
| **PU-02** | `Boot Mode` with a `Fabric` | Pair the device (`MT-01`), then power-cycle it. | `Operation Mode = OPERATIONAL at Power-up`; no QR code and no manual code in the log; no `Commissioning window opened` at boot; fast blink 5 s, then solid; the device is reachable from Home Assistant again (`MT-10`). | |
| **PU-03** | `Power-loss Recovery` | With `Output State = ON` (button or Home Assistant), pull the power and restore it. | GPIO1 is low, the `Output State LED` is off and Home Assistant shows `Off` after the boot, not the `On` it had before; the log has `Output State = OFF`. | `REQ-F-003` |
| **PU-04** | `SerialNumber` is stored once | Boot twice with a `Fabric` and compare the two boot logs. | `psmd_main: SerialNumber = PSMD-DEV-<MAC>` appears at the first boot only; Home Assistant shows the same serial number after the second boot; the hex part is the board's base MAC. | |
| **PU-05** | Startup `Fast Blink` duration | Time the `Fast Blink` from power on with a stopwatch, in both `LOCAL` and `OPERATIONAL`. | About 2 Hz for 5 s (±1 s), then off in `LOCAL` and solid in `OPERATIONAL`. The blink is the only sign that the device is powered. | `REQ-F-005` |

## 4. Known pitfalls

- A device without a `Fabric` keeps the 900 s commissioning window ESP-Matter
  opens at `Power-up` (`TBD-31`). That the device is still commissionable long
  after the 5 s blink is therefore correct, not a failure.
- A device that already has a `Fabric` opens no window at `Power-up`; open one
  with `Long Press` (`LC-06`).
- `SerialNumber` is written to NVS only when the stored value differs from the
  computed one, so its absence in the log on later boots is the expected result,
  not a missing line.
