# Decisions, open questions and gaps – PSMD

Collected decisions, open questions and known gaps, with **where they come
from**. The document is working material: open questions move to **Decided
questions** when a decision is made, and the affected documents are updated.

> Related: [`manual.md`](manual.md), [`glossary.md`](glossary.md),
> [`specification.md`](specification.md)

> **Owner:** decisions, open questions and known gaps with source (`K1`–`K6`).
> **Read when needed:** the document the question or gap concerns – search on
> the ID or the topic instead of reading the whole file.

## Statuses

| Status | Meaning |
|---|---|
| **Open** | The question is identified but not started. |
| **To investigate** | Someone needs to establish the facts, e.g. in the Matter specification. |
| **Awaiting decision** | Material exists or is needed, but a decision has to be made. |
| **Closed** | Decided; the outcome is applied in the relevant document. |

## Sources (where the questions come from)

| ID | Source | Description |
|---|---|---|
| **K1** | The product owner | Answers and requirements from the project discussion (2026-11-04). |
| **K2** | The Matter specification | The Matter specification and its behavioral requirements. |
| **K3** | Home Assistant | Home Assistant's Matter Server and integration flow. |
| **K4** | Product requirements / electrical safety | Requirements on function, ratings and safety. |
| **K5** | Document drafts | Questions that arose while writing the manual. |
| **K6** | Terminology | Term choices that need confirmation. |

## Open questions

### Connectivity and Matter

| ID | Question | Source | Affects | Status / next step |
|---|---|---|---|---|
| TBD-24 | Where does the commissioning code/QR come from, and when can it be printed in the manual? | K1 | `manual.md` §4.4, `specification.md` §12 | Awaiting decision |
| TBD-29 | Which Home Assistant and Matter Server versions run on the test network? | K1, K3 | `specification.md` §12–13 | To investigate at M2 |

### Measurement

| ID | Question | Source | Affects | Status / next step |
|---|---|---|---|---|
| TBD-10 | Which of the eight measured values are exposed: voltage, current, apparent power, active power, reactive power, phase delay, frequency, energy? | K4 | Manual 6.2, Matter clusters | Awaiting decision |
| TBD-12 | Should energy measurement accumulate across a power loss (`Persisted State`)? | K4 | Energy measurement | Awaiting decision |
| TBD-28 | Where is the MU interface specification, and does it cover bus, address, register or command protocol, sample format, units, cycle and error handling? | K1 | `specification.md` §2, §8, §11, `GAP-02` | Awaiting the specification from the product owner |

### Hardware and electrical safety

| ID | Question | Source | Affects | Status / next step |
|---|---|---|---|---|
| TBD-13 | What type of connector do input and output have? | K1 (deferred), K4 | `manual.md` §4.2, §5.1, `hardware.md` | Awaiting decision |
| TBD-14 | Maximum load (A/W) and electrical safety rating? | K4 | `manual.md` §3, §11, `hardware.md` | Awaiting decision |
| TBD-15 | Should the device be intended for indoor use only? | K4 | `manual.md` §3, `hardware.md` | Awaiting decision |
| TBD-23 | How is the plug-in unit mounted? `TBD-C9` refers here. | K4 | `hardware.md` | Awaiting decision |
| TBD-25 | Should the device require a protective earth (grounded outlet)? | K4 | `manual.md` §3, `hardware.md` | Awaiting decision |
| TBD-26 | Which environment must the device tolerate: humidity, heat, storage and operating temperature? | K4 | `manual.md` §3, §7, §11, `hardware.md` | Awaiting decision |
| TBD-27 | Technical data: mains supply, dimensions and weight? | K4 | `manual.md` §11, `hardware.md` | Awaiting decision |

### Terminology

| ID | Question | Source | Affects | Status / next step |
|---|---|---|---|---|
| TBD-16 | Should `Output State` be used, or `Relay State`/`Power State`? | K6 | Glossary | Awaiting decision |
| TBD-17 | Should `Persisted State` be used as the term for stored state? | K6 | Glossary | Awaiting decision |
| TBD-18 | Should `Relay` be used in user documentation, or only internally? | K6 | Glossary, manual | Awaiting decision |

### Architecture and data model

| ID | Question | Source | Affects | Status / next step |
|---|---|---|---|---|
| TBD-19 | What do `PSMD`, `MDCB`, `MDB` and `DPS` stand for, and where are their responsibilities? | K5 | `specification.md` §11, data model | To investigate |
| TBD-20 | Should the data model's fields, types and scaling (e.g. `int64` for measured values) be confirmed and documented per field? | K5 | `specification.md` §11–12, Matter reporting | To investigate |

## Decided questions (closed)

| ID | Question | Decision | Source |
|---|---|---|---|
| TBD-C1 | Should measurement run in `LOCAL`? | No, no measurement in `LOCAL` or `PAIRING`. | K1 |
| TBD-C2 | Should measurement be active when `Output State = OFF`? | Yes. | K1 |
| TBD-C3 | `Slow Blink` frequency? | 0.5 s on / 0.5 s off (1 Hz). | K1 |
| TBD-C4 | `Fast Blink` frequency? | 0.25 s on / 0.25 s off (2 Hz), 5 s at `Power-up`. | K1 |
| TBD-C5 | Should `Long Press` go to `PAIRING` from `OPERATIONAL` as well? | Yes. | K1 |
| TBD-C6 | Should `Long Press` in `PAIRING` return to `LOCAL`? | Yes. | K1 |
| TBD-C7 | `Output State` at `Power-up`? | Always `OFF`. | K1 |
| TBD-C8 | Which ESP-IDF version is the firmware built against? | ESP-IDF 6.1.0, with Matter 1.6.1 from the component `espressif/esp_matter` 1.6.1 in the ESP Component Registry. See [`lookup/esp-idf.md`](lookup/esp-idf.md). | K1 |
| TBD-C9 | Which Matter device types are used? | On/Off Plug-in Unit (`0x010A`) and Electrical Sensor (`0x0510`) on the same endpoint. See [`specification.md`](specification.md) §12.1. The mounting is examined in `TBD-23`. | K4 |
| TBD-C10 | How is `phase_delay` reported? | As `PowerFactor` in Electrical Power Measurement. Unit and scaling are decided in `TBD-C19`. | K4 |
| TBD-C11 | Which `Measurement Interval` should be used? | 5 s, driven by the MU measurement cycle; all 8 measured values are synchronous and MC reports one value at a time to HA. | K4 |
| TBD-C12 | Should measured values be reported as one struct with all values, or as standard attributes? | Standard attributes (one value per Matter attribute). Keeps Home Assistant's standard mapping; no custom struct. | K2, K3 |
| TBD-C13 | Matter, Home Assistant and provisioning (`TBD-01`–`TBD-09`): Pairing/Commissioning Timeout, Decommissioning, Factory Reset, Wi-Fi provisioning, behavior when Home Assistant is unreachable, Reconnect, multiple Fabrics and HA/Matter Server version. | Handled by ESP-Matter according to the Matter specification; PSMD uses ESP-Matter's implementation/standard behavior and assumes compatibility with the Matter specification. | K2, K3 |
| TBD-C14 | Should PSMD be a plug-in or permanently mounted unit? | Plug-in (On/Off Plug-in Unit, `0x010A`). Mounted On/Off Control (`0x010F`) is not needed. | K4 |
| TBD-C15 | Does `LOCAL` require a cloud service or an internet connection? | No. `LOCAL` switches the `Output` without a cloud service or an internet connection, and no data leaves the local network. | K1 |
| TBD-C16 | Are measured values computed when no `Load` is connected? | Yes, always. They are then 0 and are reported if the device is in `OPERATIONAL`. | K1 |
| TBD-C17 | Which hardware is the firmware developed and verified on? | An ESP32-C6 dev board. The product's own board follows the hardware (`GAP-07`). | K1 |
| TBD-C18 | How is PSMD developed before the hardware exists? | A simulated `PU` and `MU` behind the same interfaces as the real ones. Everything works simulated before the real MU replaces the simulated backend, and the simulator stays as a backend for tests. | K1 |
| TBD-C19 | Which unit and scaling should the `PowerFactor` reporting of `phase_delay` use? | Percent100ths, the unit `PowerFactor` has in Electrical Power Measurement (`0x0011`, −10000…10000). `phase_delay` is not reported in degrees. | K2 |
| TBD-21 | ESP-Matter recommends ESP-IDF v6.0.2, but PSMD builds on 6.1 (`TBD-C8`). Does the combination work, or should the IDF version be lowered? | The combination works: `espressif/esp_matter` 1.6.1 builds and links on ESP-IDF 6.1.0 for `esp32c6`, and the image fits the application partition. 6.1 is kept; it is rechecked when the component is updated. | K2 |

## Gaps and next steps

Known gaps in the documentation and what has to be produced. It is a work list
– not established requirements. Decisions and open questions are handled in the
sections above.

| ID | Prio | Area | What is missing | Document |
|---|---|---|---|---|
| GAP-01 | High | Matter | Device type, endpoints, clusters, attributes, units and reporting intervals. | [`specification.md`](specification.md) §12 |
| GAP-02 | High | Hardware/software | Pin map for the button, the LEDs and the `Output`, active level, debounce, the MU I2C interface (`TBD-28`), measurement ranges and calibration. | [`specification.md`](specification.md) §14 |
| GAP-03 | High | Data model | Units, scaling, intervals, `timestamp`, energy at `Power-up`/overflow and which fields are `Persisted State`. | [`specification.md`](specification.md) §11 |
| GAP-04 | Medium | Verification | Non-functional requirements and concrete acceptance tests tied to each requirement. | [`specification.md`](specification.md) §10 |
| GAP-05 | Medium | User manual | Section 5.4 assumes a local UI with views and screenshots. Clarify whether PSMD has any interface of its own beyond the button, the LEDs and Home Assistant. | [`manual.md`](manual.md) |
| GAP-06 | Medium | Manual → specification | The network requirements in the manual (`manual.md` §2.3: Wi-Fi band, PSMD and Home Assistant in the same network, Home Assistant version with a Matter Server) have no equivalent in the specification. The band follows the hardware implementation. | [`specification.md`](specification.md) §13 |
| GAP-07 | Medium | Hardware document | [`hardware.md`](hardware.md) is a stub: connectors, electrical safety, environment and technical data are open (`TBD-13`–`TBD-15`, `TBD-23`, `TBD-25`–`TBD-27`). | [`hardware.md`](hardware.md) |

### GAP-01 – Matter interface

`specification.md` §12 says that endpoints and clusters are TBD. It needs:

- device type and number of endpoints,
- which clusters are used, e.g. On/Off and the measurement clusters,
- attributes, units and reporting intervals,
- which values are reported when `Output State = OFF`.

Without this, the firmware, the Home Assistant integration and the acceptance
tests cannot be specified.

### GAP-02 – Hardware/software

A description of the interface between hardware and software is missing:

- pin map for the button, `Operation Mode LED`, `Output State LED` and the `Output`,
- active level, pull-up/pull-down and debounce,
- the I2C bus and interface to the `MU` (`TBD-28`), calibration and measurement
  ranges.

### GAP-03 – Semantics of the data model

The data model in `specification.md` §11 has field names and types, but lacks:

- units and scaling for `vac`, `iac`, `pact` and `energy`,
- interval and sign convention for `preact` and `phase_delay`,
- what `timestamp` represents,
- how `energy` is handled at `Power-up` and overflow,
- which fields are actually `Persisted State`.

### GAP-04 – Verification

`specification.md` §10 has five functional requirements. Missing:

- non-functional requirements within performance, memory, robustness and power consumption,
- electrical safety and EMC, if they are to be handled in this repository,
- concrete acceptance tests for each requirement.

### GAP-05 – The UI section of the user manual

Section 5.4 of `manual.md` describes "main views, menus and key functions –
preferably with screenshots". That assumes a local interface that does not
appear anywhere else. PSMD appears to have a button, two LEDs and Home
Assistant – no screen of its own. The section should be removed or clarified.

### GAP-06 – Network requirements in the manual

`manual.md` §2.3 states network requirements – Wi-Fi band, PSMD and Home
Assistant in the same network, a minimum Home Assistant version with a Matter
Server – that the network section of `specification.md` does not have. The band
(2.4 or 5 GHz) follows the hardware implementation and is open until it is
decided ([`hardware.md`](hardware.md)).

### GAP-07 – The hardware document

[`hardware.md`](hardware.md) owns what the specification does not: connectors,
electrical safety, environment and technical data. The values are open; the
questions with IDs are in the tables above.

### Order of work

The order, the phases and the exit criteria are in [`project.md`](project.md).
This document only defines the IDs.

## How the document is used

1. Every new open question gets an ID (`TBD-NN`) and a source.
2. When the question is decided it moves to **Decided questions** and the
   relevant document (`manual.md` / `glossary.md` / the specification) is
   updated.
3. External sources (Matter, HA) should preferably be completed with a link or
   a version when they are verified.
