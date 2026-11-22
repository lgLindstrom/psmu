# Decisions, open questions and gaps – PSMD

Collected decisions, open questions and known gaps, with **where they come
from**. The document is working material: every new open question gets an ID
(`TBD-NN`) and a source, and moves to **Decided questions** when a decision is
made – the affected documents are updated at the same time. External sources
(Matter, Home Assistant) are completed with a link or a version when they are
verified.

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
| TBD-24 | Where does the commissioning code/QR come from, and when can it be printed in the manual? | K1 | `manual.md` §4.4, `specification.md` §12 | Where it comes from is established: `main/app_main.cpp` prints the onboarding payload (QR and manual code) at every `Power-up` while the device has no `Fabric`. Still open: what a shipped unit carries. Every unit uses chip's test discriminator (3840) and passcode (20202021), so all units share one code and cannot be told apart while unprovisioned; the manual shows the dev board's code as an example only. A per-unit pair needs a custom commissionable data provider, which is still open. |
| TBD-29 | Which Home Assistant and Matter Server versions run on the test network? | K1, K3 | `specification.md` §12–13 | To investigate at M2 |
| TBD-31 | Which `Pairing Timeout` applies: the 3 min the manual states, or the 900 s commissioning window ESP-Matter opens at `Power-up` while the device has no `Fabric`? | K1 | `manual.md` §4.4, `specification.md` §12, `main/app_main.cpp` | Awaiting decision. `PAIRING` entered with `Long Press` opens a window of 3 min; the window that ESP-Matter opens at `Power-up` keeps its 900 s and `PAIRING` follows it when it is already open. |

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
| TBD-C17 | Which hardware is the firmware developed and verified on? | An ESP32-C6 dev board, chosen only to verify the build and flash environment. Nothing about the product's hardware follows from it; the product's own board follows [`hardware.md`](hardware.md) (`GAP-07`). | K1 |
| TBD-C18 | How is PSMD developed before the hardware exists? | A simulated `PU` and `MU` behind the same interfaces as the real ones. Everything works simulated before the real MU replaces the simulated backend, and the simulator stays as a backend for tests. | K1 |
| TBD-C19 | Which unit and scaling should the `PowerFactor` reporting of `phase_delay` use? | Percent100ths, the unit `PowerFactor` has in Electrical Power Measurement (`0x0011`, −10000…10000). `phase_delay` is not reported in degrees. | K2 |
| TBD-21 | ESP-Matter recommends ESP-IDF v6.0.2, but PSMD builds on 6.1 (`TBD-C8`). Does the combination work, or should the IDF version be lowered? | The combination works: `espressif/esp_matter` 1.6.1 builds and links on ESP-IDF 6.1.0 for `esp32c6`, and the image fits the application partition. 6.1 is kept; it is rechecked when the component is updated. | K2 |
| TBD-C20 | Which pins does the development board use for the button, the `Operation Mode LED` and `Output State`? | Button GPIO3 and `Operation Mode LED` GPIO2, both active low, with the button's internal pull-up; `Output State` on GPIO10, active high, standing in for the relay; no `Output State LED`. The board is chosen with `CONFIG_PSMD_BOARD_DEV`; the production board's pin map is still open (`GAP-02`). | K1 |
| TBD-30 | Which values do the mandatory Basic Information attributes carry? | Development values until the product has its own: `VendorName`/`ProductName` `PSMD`, `VendorID` `0xFFF1` and `ProductID` `0x8000` (the CSA test IDs the test attestation carries), `HardwareVersion` 1/`1.0`, `SoftwareVersion` 1/`0.1.0`, `SerialNumber` `PSMD-DEV`, `NodeLabel`/`Location` left at their defaults. Real IDs and a matching attestation are required before shipping; they are set in `CMakeLists.txt` and `sdkconfig.defaults`. | K1 |

## Gaps and next steps

Known gaps in the documentation and what has to be produced. It is a work list
– not established requirements. Decisions and open questions are handled in the
sections above.

| ID | Prio | Area | What is missing | Document |
|---|---|---|---|---|
| GAP-01 | High | Matter | Device type and number of endpoints, clusters, attributes, units, reporting intervals, and which values are reported when `Output State = OFF`. | [`specification.md`](specification.md) §12 |
| GAP-02 | High | Hardware/software | Pin map for the button, the LEDs and the `Output`, active level, pull-up/pull-down, debounce, the MU I2C interface (`TBD-28`), measurement ranges and calibration. The development board's pins and active levels are decided (`TBD-C20`); the production board's pin map and the debounce for the real button are still open. | [`specification.md`](specification.md) §14 |
| GAP-03 | High | Data model | Units and scaling per field, intervals, `timestamp`, sign convention for `preact` and `phase_delay`, energy at `Power-up`/overflow and which fields are `Persisted State`. | [`specification.md`](specification.md) §11 |
| GAP-04 | Medium | Verification | Non-functional requirements (performance, memory, robustness, power; electrical safety and EMC if they belong in this repository) and concrete acceptance tests tied to each requirement. | [`specification.md`](specification.md) §10 |
| GAP-05 | Medium | User manual | Section 5.4 assumes a local UI with views and screenshots. Clarify whether PSMD has any interface of its own beyond the button, the LEDs and Home Assistant. | [`manual.md`](manual.md) |
| GAP-06 | Medium | Manual → specification | The network requirements in the manual (`manual.md` §2.3: Wi-Fi band, PSMD and Home Assistant in the same network, Home Assistant version with a Matter Server) have no equivalent in the specification. The band follows the hardware implementation. | [`specification.md`](specification.md) §13 |
| GAP-07 | Medium | Hardware document | [`hardware.md`](hardware.md) is a stub: connectors, electrical safety, environment and technical data are open (`TBD-13`–`TBD-15`, `TBD-23`, `TBD-25`–`TBD-27`). | [`hardware.md`](hardware.md) |

The order the gaps are worked in, with phases and exit criteria, is in
[`project.md`](project.md). This document only defines the IDs.
