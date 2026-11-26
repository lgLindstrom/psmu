# Manual test plan – local control

**Power Switch and Measurement Device (PSMD)**

> Related: [`../../docs/project.md`](../../docs/project.md), [`../../docs/specification.md`](../../docs/specification.md),
> [`../../docs/decisions.md`](../../docs/decisions.md), [`../../docs/manual.md`](../../docs/manual.md),
> [`README.md`](README.md)

> **Owner:** the manual tests that verify the button, `Operation Mode`, the
> `Operation Mode LED` and `Output State` without Home Assistant in the loop.
> **Read when needed:** before and while testing a build on the development
> board, and when recording the result.

## 1. Scope

Covers [`../../docs/specification.md`](../../docs/specification.md) §3–6 and §9:

- `Short Press` (≤ 3 s) and `Long Press` (> 3 s), the `Press Threshold` and the
  debounce.
- The `Operation Mode` transitions that the button drives.
- The `Operation Mode LED` pattern for each mode.
- `Output State` on GPIO1, driven locally, and its LED on the same line.

Not covered: pairing and the Home Assistant side (see
[`manual-matter.md`](manual-matter.md)), the behavior at boot (see
[`manual-power-up.md`](manual-power-up.md)), and measurement (M4–M6).

## 2. Preconditions

Everything is in [`README.md`](README.md) *Common preconditions*. `LC-01` and
`LC-05` run on a device with no `Fabric`; the others say so where it matters.
A stopwatch helps for `LC-04`, `LC-08` and `LC-09`.

## 3. Test cases

| ID | Purpose | Steps | Expected result | REQ |
|---|---|---|---|---|
| **LC-01** | `Short Press` in `LOCAL` | On a device with no `Fabric`, press the button for less than 1 s; press again. | `psmd_button: Short Press` and `Output State = ON` then `OFF`; GPIO1 and the `Output State LED` follow; the `Operation Mode LED` is off; nothing else in the log. | `REQ-F-001`, `REQ-F-006` |
| **LC-02** | `Short Press` in `OPERATIONAL` | On a paired device, turn the entity on in Home Assistant, then press the button shortly; press again. | `Output State` toggles locally each time, GPIO1 and the `Output State LED` follow, Home Assistant shows the new state within about 2 s, the `Operation Mode LED` stays solid. | `REQ-F-001` |
| **LC-03** | `Short Press` in `PAIRING` has no effect | With `Output State = ON`, enter `PAIRING` (`LC-05`), then press the button shortly. | `psmd_button: Short Press` in the log, but no `Output State` line, GPIO1 stays high, the mode stays `PAIRING`. | `REQ-F-002` |
| **LC-04** | `Press Threshold` | Press for about 2 s and release. Then press for about 4 s and release. | 2 s gives `Short Press` only; 4 s gives `Long Press` only, about 3 s into the press, and no `Short Press` on release. | `REQ-F-002` |
| **LC-05** | `Long Press` in `LOCAL` | Hold the button until the LED starts blinking slowly (about 3 s), then release. | `Long Press`; `Operation Mode = PAIRING`; slow blink (0.5 s on / 0.5 s off); `Commissioning window opened` and the onboarding code in the log. Releasing adds nothing. | `REQ-F-002` |
| **LC-06** | `Long Press` in `OPERATIONAL` | On a paired device, hold the button until the LED blinks slowly, then release. | Same as `LC-05`: `Operation Mode = PAIRING`, slow blink, `Commissioning window opened` with the 3 min window (`TBD-31`). Home Assistant may show the device as unavailable, which is expected. | `REQ-F-002` |
| **LC-07** | `Long Press` held sends one event | Hold the button for 10 s, then release. | Exactly one `Long Press` and one `Operation Mode = PAIRING` in the log; the release sends no `Short Press`. | |
| **LC-08** | Debounce | Tap the button as briefly as possible several times in a row; then press normally once. | The brief taps give no event (or at most one complete press each); the normal press gives exactly one `Short Press`. Contact bounce cannot be reproduced by hand, so this case checks the short tap, not the 30 ms filter (`TBD-C20`). | |
| **LC-09** | LED pattern per mode | Measure the pattern in each mode with a stopwatch: `LOCAL` after the startup blink, `PAIRING`, `OPERATIONAL`. Then toggle `Output State` in each mode. | `Operation Mode LED`: off in `LOCAL`, slow blink 1 s / 1 s (0.5 Hz) in `PAIRING`, solid in `OPERATIONAL`; the startup fast blink reaching 2 Hz is in `PU-05`. `Output State LED`: lit while `Output State = ON`, off while `OFF`, in every mode. | `REQ-F-005` |
| **LC-10** | `Output State` keeps its value in `PAIRING` | Set `Output State = ON` in `LOCAL`, then `Long Press` into `PAIRING`. | GPIO1 stays high and `Output State` is unchanged while in `PAIRING`; a `Short Press` changes nothing (`LC-03`). Leaving `PAIRING` with `Long Press` is decommissioning and restarts the device (`MT-05`), so the case ends there. | |

## 4. Known pitfalls

- The button is polled every 10 ms and must be stable for 30 ms, so a press that
  feels normal but lasts a few milliseconds can be lost. That is expected, not a
  failure.
- `Long Press` is sent once, about 3 s into the press, while the button is still
  held. The mode change therefore happens before the release, and the release
  must not be counted as a `Short Press`.
- `Short Press` is logged by the button module even in `PAIRING`, where it has
  no effect. Read the mode, not the absence of the line.
