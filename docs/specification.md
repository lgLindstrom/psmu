# Specification – PSMD

**Power Switch and Measurement Device (PSMD)**

> Related: [`decisions.md`](decisions.md), [`manual.md`](manual.md),
> [`glossary.md`](glossary.md)

> **Owner:** operation modes, button, LEDs, `Power-up`, measurement,
> requirements, architecture, data model, Matter integration and build
> environment.
> **Read when needed:** [`decisions.md`](decisions.md) for open questions and
> unknown values, and
> [`lookup/matter-1.6.1/INDEX.md`](lookup/matter-1.6.1/INDEX.md) for the Matter
> extracts (search there, do not read the whole text files).

## 1. Purpose

This document specifies PSMD: software behavior (§3–9), requirements with
acceptance criteria (§10) and the overall structure of the software (§2,
§11–15) – components, responsibilities, data flows, data model and the
integration with Matter and Home Assistant.

The document describes the firmware for the **Matter Controller (MC)** and the
interface to the **Measurement Unit (MU)**.

[`manual.md`](manual.md) owns the behavior: it describes what PSMD does from the
user's point of view, and this document is derived from it. The specification
adds what the manual does not describe – implementation, data model and the
Matter integration. Behavior that the manual states and this document lacks is a
gap: it is recorded in [`decisions.md`](decisions.md) and added here. The
`Manual` column in the behavior tables shows which part of the manual each
behavior comes from.

The document describes the **intended architecture**. Exact implementation,
software version and hardware target are not decided yet. Electrical safety,
mains connectors, environment and technical data are not specified here; they
are in [`hardware.md`](hardware.md).

## 2. System overview

PSMD is a Matter device that switches an `Output` and measures the connected
`Load`. The device can be used standalone in `LOCAL` or through a `Fabric` in
Home Assistant.

PSMD consists of three functional units:

| Unit | Abbreviation | Task |
|---|---|---|
| **Matter Controller** | `MC` | Handles the interface to Home Assistant through the Matter protocol. |
| **Measurement Unit** | `MU` | Performs the measurement. |
| **Power Unit** | `PU` | Is the interface to the input and output signals. |

`MC` and `MU` communicate over an I2C channel and over direct signals.

```mermaid
flowchart LR
    Load[Load] --- PSMD[PSMD]
    PSMD -- Matter --> MS[Matter Server]
    MS --- HA[Home Assistant]
```

PSMD is responsible for:

- switching `Output State` (`ON`/`OFF`),
- measuring voltage, current and power in `OPERATIONAL`,
- handling `Operation Mode` and local operation,
- storing the configuration and state that `Power-up` requires,
- working without a cloud service or an internet connection in `LOCAL`, sending
  no data outside the local network (`TBD-C15`).

Home Assistant and the `Matter Server` are outside PSMD and are responsible for
`Commissioning`, automations and the user interface.

## 3. Operation Modes

| `Operation Mode` | Description | `Operation Mode LED` | Manual |
|---|---|---|---|
| **`LOCAL`** | Default mode at the first `Power-up`. No connection to Home Assistant. `Short Press` toggles `Output State`. | Fast blink for 5 s at `Power-up`, then off. | [manual.md §5.3](manual.md) |
| **`PAIRING`** | The device is available for Matter `Commissioning`. `Short Press` has no effect. | Slow blink. | [manual.md §5.3](manual.md) |
| **`OPERATIONAL`** | The device belongs to a `Fabric` and can be controlled from Home Assistant. `Short Press` toggles `Output State`. | Fast blink for 5 s at `Power-up`, then solid. | [manual.md §5.3](manual.md) |

## 4. Transitions

| From | Event | To | Comment | Manual |
|---|---|---|---|---|
| – | `Power-up`, first time | `LOCAL` | Default mode. | [manual.md §5.3](manual.md) |
| – | `Power-up`, previously connected | `OPERATIONAL` | Behavior when Home Assistant is unreachable is handled by ESP-Matter (`TBD-C13`). | [manual.md §5.3](manual.md) |
| `LOCAL` | `Long Press` | `PAIRING` | | [manual.md §5.3](manual.md) |
| `PAIRING` | Successful `Commissioning` | `OPERATIONAL` | The device is added to a `Fabric`. | [manual.md §5.3](manual.md) |
| `PAIRING` | `Pairing Timeout` or failed connection | `LOCAL` | `Pairing Timeout` is 3 min; verification against ESP-Matter/Matter is open (`TBD-C13`). | [manual.md §4.4](manual.md) |
| `PAIRING` | `Long Press` | `LOCAL` | `Decommissioning` per ESP-Matter (`TBD-C13`). | [manual.md §5.3](manual.md) |
| `OPERATIONAL` | `Long Press` | `PAIRING` | | [manual.md §5.3](manual.md) |

`Short Press` never changes `Operation Mode`; it only toggles `Output State` in
`LOCAL` and `OPERATIONAL`.

## 5. Button

| Event | Definition | Behavior | Manual |
|---|---|---|---|
| **`Short Press`** | Press ≤ 3 s. | Toggles `Output State` in `LOCAL` and `OPERATIONAL`. No effect in `PAIRING`. | [manual.md §5.2](manual.md) |
| **`Long Press`** | Press > 3 s. | Changes `Operation Mode` according to the table above. | [manual.md §5.2](manual.md) |
| **`Press Threshold`** | 3 s. | Boundary between `Short Press` and `Long Press`. | [manual.md §5.3](manual.md) |

Debounce time and the exact reading of the button are not decided.

## 6. LEDs

| LED | Behavior | Manual |
|---|---|---|
| **`Operation Mode LED`** (`LED1`) | Shows `Operation Mode`: fast blink for 5 s at `Power-up` (both `LOCAL` and `OPERATIONAL`), off in `LOCAL`, slow blink in `PAIRING`, solid in `OPERATIONAL`. | [manual.md §5.2](manual.md) |
| **`Output State LED`** (`LED2`) | Mirrors `Output State`: on when `ON`, off when `OFF`, independent of `Operation Mode`. | [manual.md §5.2](manual.md) |

| Pattern | On / off | Frequency |
|---|---|---|
| **`Fast Blink`** | 0.25 s / 0.25 s | 2 Hz, for 5 s at `Power-up`. |
| **`Slow Blink`** | 0.5 s / 0.5 s | 1 Hz. |
| **`Solid`** | On | – |
| **`Off`** | Off | – |

Both LEDs are off when PSMD has no power; the fast blink at `Power-up` is the
only indication that the device is powered.

Source: [manual.md §5.2](manual.md), [manual.md §8](manual.md).

## 7. Power-up and persistence

- `Output State` is **always set to `OFF`** at `Power-up` (`Power-loss Recovery`).
- `Operation Mode` at `Power-up` follows `Boot Mode`: `LOCAL` the first time,
  otherwise `OPERATIONAL`.
- Which state is otherwise stored and restored is not decided (`TBD-12`).
- `Factory Reset` is handled by ESP-Matter (`TBD-C13`).

Source: [manual.md §4.2](manual.md), [manual.md §5.3](manual.md), [manual.md §6.1](manual.md).

## 8. Measurement

Measurement runs **only in `OPERATIONAL`**. Nothing is measured in `LOCAL` or
`PAIRING`. Measurement is active also when `Output State` is `OFF`. The values
are computed and reported also when no `Load` is connected; they are then 0
(`TBD-C16`).

| Quantity | Status |
|---|---|
| Voltage | Decided as a measured value. |
| Current | Decided as a measured value. |
| Apparent power | Decided as a measured value. |
| Active power | Decided as a measured value. |
| Reactive power | Decided as a measured value. |
| Phase delay (`phase_delay`) | Decided as a measured value; reported as `PowerFactor` in percent100ths (`TBD-C10`, `TBD-C19`). |
| Frequency | Decided as a measured value; whether it is exposed is decided in `TBD-10`. |
| Energy | Decided as a measured value, but accumulation across `Power-up` is not decided (`TBD-12`). |

The measurement is performed by **MU** at a fixed interval of **5 s**. All 8
measured values are synchronous (the same measurement cycle). `MC` sends one
value at a time to Home Assistant. `MC` reaches `MU` over I2C; the interface is
`TBD-28`.

A **measured value** is produced by the `MU`. `MC` never measures or derives
one: it reads the values from `MU` and republishes them as Matter attributes.

`Measurement Interval` is therefore decided at 5 s (`TBD-C11`), but which values
are reported to Matter is decided in `TBD-10`. Field names and scaling are in
§11.

Measured values are reported as **standard attributes** (one value per Matter
attribute) so that Home Assistant can use its standard mapping. No custom
struct with all values is used (`TBD-C12`).

`phase_delay` is reported as `PowerFactor` in percent100ths (`TBD-C10`,
`TBD-C19`). The mapping between the data model's fields and the Matter
attributes is in §11.

Source: [manual.md §6.2](manual.md).

## 9. Local and external control

- **`LOCAL`:** only `Short Press` controls `Output State`.
- **`OPERATIONAL`:** Home Assistant and `Short Press` can both change
  `Output State`. `Output State LED` always follows the current state.
- **`PAIRING`:** `Output State` keeps its current value; `Short Press` has no
  effect.

Source: [manual.md §6.1](manual.md).

## 10. Requirements and acceptance criteria

Requirements with ID, source, acceptance criterion and traceability to the
specification in this document.

| ID | Requirement | Source | Acceptance criterion | Spec | Manual | Test | Status |
|---|---|---|---|---|---|---|---|
| REQ-F-001 | `Short Press` shall toggle `Output State` in `LOCAL` and `OPERATIONAL`. | K1 | The output toggles on a press ≤ 3 s. | §5 | [manual.md §5.2](manual.md) [manual.md §6.1](manual.md) | [ ] | Confirmed |
| REQ-F-002 | `Long Press` shall change `Operation Mode` according to the specification. | K1 | `LOCAL`/`OPERATIONAL` → `PAIRING`; `PAIRING` → `LOCAL`. | §4–5 | [manual.md §5.2](manual.md) [manual.md §5.3](manual.md) | [ ] | Confirmed |
| REQ-F-003 | `Output State` shall always be `OFF` at `Power-up`. | K1 | The output is `OFF` after every power-up. | §7 | [manual.md §4.2](manual.md) [manual.md §5.3](manual.md) | [ ] | Confirmed |
| REQ-F-004 | Measurement shall run only in `OPERATIONAL`. | K1 | No measured values are updated in `LOCAL` or `PAIRING`. | §8 | [manual.md §6.2](manual.md) | [ ] | Confirmed |
| REQ-F-005 | `Operation Mode LED` and `Output State LED` shall follow the specified patterns. | K1 | Blink and light patterns match the specification. | §6 | [manual.md §5.2](manual.md) | [ ] | Confirmed |
| REQ-F-006 | `LOCAL` shall work without a cloud service or an internet connection, and without sending data outside the local network. | K1 | With the network disabled, `Short Press` still toggles `Output State`. | §2, §9 | [manual.md §2.1](manual.md) [manual.md §4.3](manual.md) | [ ] | Confirmed |
| REQ-F-007 | Measured values shall be computed and reported also when no `Load` is connected. | K1 | Values are reported as 0 with no load in `OPERATIONAL`. | §8 | [manual.md §6.2](manual.md) [manual.md §8](manual.md) | [ ] | Confirmed |

Non-functional requirements and concrete acceptance tests tied to each
requirement are still missing; see the gap in [`decisions.md`](decisions.md).

### Test strategy

- [ ] Unit tests of software modules.
- [ ] Integration and HIL tests of the button, LEDs, relay and measurement.
- [ ] Matter `Commissioning` against Home Assistant.
- [ ] Acceptance tests against the requirements above.

## 11. Data model

The table is the preliminary data model. Field names, types and scaling need to
be confirmed (`TBD-20`).

| Name | Editable | Matter attribute | Matter unit |
|---|---|---|---|
| mac_address | no | – | – |
| onoff | yes | `OnOff` (On/Off) | bool |
| vac | no | `Voltage` or `RMSVoltage` (Electrical Power Measurement) | mV |
| iac | no | `ActiveCurrent` or `RMSCurrent` | mA |
| papp | no | `ApparentPower` | mVA |
| pact | no | `ActivePower` | mW |
| preact | no | `ReactivePower` | mVAR |
| phase_delay | no | `PowerFactor` | percent100ths |
| frequency | no | `Frequency` | mHz |
| energy | no | `CumulativeEnergyImported` (Electrical Energy Measurement) | mWh |
| friendly_name | yes | – | – |
| mdb_firmware | no | – | – |
| mdcb_firmware | no | – | – |
| location | yes | – | – |
| geolocation | yes | – | – |
| mdb_app_id | no | – | – |

`Editable = yes` means that the value can be changed through the interface.

The table is a proposal. Which measured values are actually exposed is decided
in `TBD-10`. That `phase_delay` is reported as `PowerFactor` in percent100ths is
decided (`TBD-C10`, `TBD-C19`).

Matter scales measured values in mV, mA, mW and mHz, while the data model
specifies `int64` without a unit. The scaling has to be decided (`TBD-20`).
Whether energy measurement accumulates across `Power-up` is open (`TBD-12`).

## 12. Matter and Home Assistant

- `Commissioning` puts PSMD in a `Fabric` and makes the device controllable from
  Home Assistant.
- The commissioning code and its QR come from ESP-Matter and are printed in the
  manual ([manual.md §4.4](manual.md)) once they exist (`TBD-24`).
- `Operation Mode` and transitions are specified in §3–4.
- Measurement runs only in `OPERATIONAL`. Which measured values are exposed is
  partly decided (`TBD-10`).
- `MC` uses the standard attributes in Electrical Power/Energy Measurement;
  Home Assistant's standard mapping is used. Correlation between values from
  the same sample happens at application level, not through a struct
  (`TBD-C12`).
- `Pairing`/`Commissioning` Timeout, `Decommissioning`, `Factory Reset`,
  Wi-Fi provisioning, behavior when Home Assistant is unreachable, `Reconnect`,
  handling of multiple `Fabric` and the HA/Matter Server version are handled by
  ESP-Matter according to the Matter specification (`TBD-C13`).

### 12.1 Device types

PSMD is exposed as two device types on the same endpoint (`TBD-C9`):

| Device type | ID | Class | Role |
|---|---|---|---|
| **On/Off Plug-in Unit** | `0x010A` | Simple | Switches the `Output`. |
| **Electrical Sensor** | `0x0510` | Utility | Measures the connected `Load`. |

Core §9.2 allows several device types in `DeviceTypeList` on the same endpoint.
A cluster that is common to several device types on the endpoint shall exist as
one shared instance.

PSMD is implemented as a **plug-in unit** (On/Off Plug-in Unit, `0x010A`);
Mounted On/Off Control (`0x010F`) is not used (`TBD-C14`).

### 12.2 Clusters

| Cluster | ID | Required by | Conformance | Use |
|---|---|---|---|---|
| Identify | `0x0003` | On/Off Plug-in Unit | M | Identification during `Commissioning`. |
| Groups | `0x0004` | On/Off Plug-in Unit | M | Group control. |
| On/Off | `0x0006` | On/Off Plug-in Unit | M | `OnOff` mirrors `Output State`. |
| Level Control | `0x0008` | On/Off Plug-in Unit | O | Recommended for group control; must not affect `Output` beyond on/off. |
| Scenes Management | `0x0062` | On/Off Plug-in Unit | M | Scenes in Home Assistant. |
| Occupancy Sensing | `0x0406` | On/Off Plug-in Unit | O (client) | Not used by PSMD. |
| Electrical Power Measurement | `0x0090` | Electrical Sensor | O.a+ | Instantaneous measured values. |
| Electrical Energy Measurement | `0x0091` | Electrical Sensor | O.a+ | Energy measurement. |
| Power Topology | `0x009C` | Electrical Sensor | M | States which endpoints the measurement concerns (Core §11.8). |

The Electrical Sensor requires at least one of Electrical Power Measurement and
Electrical Energy Measurement (`O.a+`); PSMD uses both.

## 13. Network and provisioning

Wi-Fi provisioning, reconnection and behavior when Home Assistant is
unreachable are handled by ESP-Matter according to the Matter specification
(`TBD-C13`).

## 14. Build environment and runtime

- The software is built with **ESP-IDF 6.1** (`TBD-C8`) and **ESP-Matter**
  1.6.1 for Matter 1.6.1, taken as the component `espressif/esp_matter` from the
  ESP Component Registry. The ESP-IDF project is the repository root
  (`CMakeLists.txt`, `main/`, `components/`) and is built and flashed through
  the `esp-idf` MCP; the project standard is in [`../README.md`](../README.md).
- Where the Espressif documentation lives and how to look it up:
  [`lookup/esp-idf.md`](lookup/esp-idf.md).
- The development and verification target is an ESP32-C6 dev board
  (`TBD-C17`). The product's own board follows the hardware (`GAP-07`).
- `PU` and `MU` sit behind interfaces with a simulated backend, so the firmware
  runs without the real hardware (`TBD-C18`). The `MU` is simulated until the
  interface in `TBD-28` is implemented behind the same interface; the product
  build uses the I2C backend.
- Practical build, flash and debug steps, logging conventions and the ESP-IDF
  project standard are in [`../README.md`](../README.md).
- The Matter component recommends ESP-IDF v6.0.2; 6.1 is built and fits the
  partition, so the deviation is kept (`TBD-21`).
- Firmware and application versions are stored according to the data model
  above.

## 15. Security and updates

Matter attestation, key management, secure storage and possible OTA updates are
not investigated yet. No requirements or solutions are decided.
