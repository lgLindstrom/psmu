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
| TBD-24 | Where does the commissioning code/QR come from, and when can it be printed in the manual? | K1 | `manual.md` §4.4, `specification.md` §12 | Where it comes from is established: `main/app_main.cpp` prints the onboarding payload (QR and manual code) whenever a commissioning window opens: with `Long Press` into `PAIRING`, and after the last `Fabric` is removed. It does not print at `Power-up`, because no window is open then (`TBD-33`). Still open: what a shipped unit carries. Every unit uses chip's test discriminator (3840) and passcode (20202021), so all units share one code and cannot be told apart while unprovisioned; the manual shows the dev board's code as an example only. A per-unit pair needs a custom commissionable data provider, which is still open. |
| TBD-29 | Which Home Assistant and Matter Server versions run on the test network? | K1, K3 | `specification.md` §12–13 | To investigate at M2 |
| TBD-32 | Is it a relevant case for PSMD that another controller or service has also commissioned the device (a second `Fabric`), and how should PSMD behave when one `Fabric` is removed while another remains? | K1, K3 | `specification.md` §4, §9, §12, `test/manual/manual-matter.md`, `main/app_main.cpp` | Awaiting decision. Matter allows several administrators and any of them can open a commissioning window, while the manual and `REQ-F-006` name only Home Assistant, so the case is possible even if it is not a supported use. Seen 2026-11-25: a delete in Home Assistant removed one `Fabric`, the device kept another (`Fabrics at Power-up = 1`, `Fabric already commissioned`) and was not commissionable again. `main/app_main.cpp` now keeps `OPERATIONAL` until the last `Fabric` is removed, then erases the Wi-Fi credentials, opens a window and enters `PAIRING`, and it logs `Fabric removed (<n> left)`; whether that is the behavior the product should have, and whether multi-admin should be allowed at all, is undecided. After a full factory reset (`Fabrics at Power-up = 0`) in `PAIRING`, Home Assistant still showed the old device and would not continue, so that block is in the `Matter Server` node list, not on the device. Home Assistant's own issue `home-assistant/core#174891` (HA 2026.6, Matter Server 1.1.1) documents that delete under Settings, Devices and Services, Devices leaves the node in the fabric, and that a factory reset before the node is removed leaves an orphan node; the correct path is the Matter integration's device list, and an orphan is removed in the `Matter Server` web UI. Seen 2026-11-25: one pairing produced two `Commissioning completed successfully` lines and a new window 1.3 s after the first, so the device ended with two `Fabric`s; the first attempt had failed on a failsafe timeout before that. The user pressed "Add device" once, and the second window could only have been opened by the administrator that the first commissioning had just created, so the double commissioning is the controller's; the third session also wrote a different regulatory country code (`XX`) than the first two (`SE`), which suggests a different commissioner client rather than a plain retry. Pairing cannot be done from the Home Assistant web UI, because commissioning needs Bluetooth; the Companion app was used, and its flow may add the phone's ecosystem as a second administrator, which would make two different fabrics expected. `main/app_main.cpp` now logs one line per `Fabric` (index, compressed fabric ID, fabric ID, node ID, vendor ID) at `Power-up`, when a window opens, after `Commissioning complete` and after a removal, so two entries of the same `Fabric` can be told from two controllers. Read 2026-11-26: the two entries are different controllers – index 1 with vendor `0x6006` (Google, compressed `0x3C423E297FB0A2BB`) and index 2 with vendor `0xFFF1` (Home Assistant's `Matter Server`, fabric 1, compressed `0x4B2AED2DFF0D1A26`). The Android Companion app commissions through Google Home because Android's Matter commissioning is provided by Google Play Services (the Home Mobile SDK) and the app cannot do BLE itself; Google Home gets the first `Fabric` and then shares the device with Home Assistant as a second administrator, so two `Fabric`s are expected for that flow and no duplicate cleanup applies. If the compressed IDs turn out identical and both appeared in one window, removing the older entry would be a candidate; which entry to keep must be decided first, and nothing is removed today. Matter requires the device to support at least five fabrics (`SupportedFabrics` in Operational Credentials is mandatory with the range 5–254), so a single-admin device, or a device that deletes a fabric it did not create, is not an option; the extra fabric can only be removed by its administrator (`RemoveFabric`) or by a factory reset. |
| TBD-35 | From the user's side, how is PSMD removed completely – including a `Fabric` created by the Android Companion app's Google Home flow – and how is the factory reset made discoverable? | K1 | `manual.md` §4.4, §5.3, §8, `test/manual/manual-matter.md` | Awaiting decision. Removing PSMD in Home Assistant removes only Home Assistant's `Fabric`; the Google Home `Fabric` from the Android flow stays and the device remains `OPERATIONAL` and commissioned, which the user cannot see. Home Assistant's own documentation gives a path inside Home Assistant: Settings > Connectivity > Matter > Devices > the device > ⋮ > Manage fabrics lists the other controller (for example Google LLC) and removes it, and ⋮ > Delete then removes Home Assistant's own; the Android flow and the delete dialog do not point this out. The only total removal without Home Assistant is `MT-05`/`Decommissioning` (`Long Press` in `PAIRING`). Whether a dedicated reset gesture, a visible indication of remaining `Fabric`s, or only manual wording is the answer is open. |
| TBD-37 | Should a Matter `Identify` request make the device do something visible, and with which pattern? | K1 | `manual.md` §5.2, `specification.md` §6, `test/manual/manual-matter.md`, `main/app_main.cpp` | Awaiting decision. Home Assistant sends `Identify` (seen 2026-11-26: `Identify: type 0` then `type 1` after 22 s), but `app_identification_cb` only logs, so nothing happens on the device and `MT-04` says no LED change is required. Suggestion: `Fast Blink` while Identify is active, except in `PAIRING` where the slow blink must stay, and restore the mode's pattern when it stops. |

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
| TBD-C3 | `Slow Blink` frequency? | 1 s on / 1 s off (0.5 Hz). | K1 |
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
| TBD-C20 | Which pins does the development board use for the button, the `Operation Mode LED` and `Output State`? | Button GPIO3, active low, with the button's internal pull-up; `Operation Mode LED` GPIO2, active high; `Output State` on GPIO1, active high, standing in for the relay. The `Output State LED` is on the `Output State` line and has no pin of its own. The board is chosen with `CONFIG_PSMD_BOARD_DEV`; the production board's pin map is still open (`GAP-02`). | K1 |
| TBD-C21 | How is decommissioning done, and what happens after the last `Fabric` is removed? | `Long Press` in `PAIRING` closes the commissioning window and factory-resets the device: Fabric and Wi-Fi credentials are erased and it restarts in `LOCAL`. When an administrator removes the last `Fabric`, the device opens a commissioning window again, shows `PAIRING` and prints the onboarding code, so it can be paired without a restart. See [`specification.md`](specification.md) §4. | K1 |
| TBD-31 | Which `Pairing Timeout` applies: the 3 min the manual states, or the 900 s commissioning window ESP-Matter opens at `Power-up` while the device has no `Fabric`? | The window ESP-Matter opens at `Power-up` is closed again (`TBD-33`), so only the 3 min window from `Long Press` or from the last `Fabric` removal applies. | K1 |
| TBD-33 | At `Power-up` without a `Fabric`, should PSMD be in `PAIRING` with a slow blink, or should the 900 s window wait for `Long Press` so that `LOCAL` (LED off) always means "not commissionable"? | A commissioning window is only open in `PAIRING`: it is opened by a `Long Press` (3 min) or when the last `Fabric` is removed, and the window ESP-Matter opens at `Power-up` is closed again. `LOCAL` therefore always means "not pairable", `PAIRING` always slow-blinks, and an unpaired device starts in `LOCAL`. | K1 |
| TBD-34 | What should `Operation Mode` and the `Operation Mode LED` show between the commissioning window closing (PASE rendezvous complete) and `CommissioningComplete` setting `OPERATIONAL`? | Stay in `PAIRING` from the start of commissioning until the outcome: `CommissioningComplete` gives `OPERATIONAL`, and a window close without an armed fail-safe gives `OPERATIONAL` when a `Fabric` remains, otherwise `LOCAL`. The close at PASE alone does not change the mode. | K1 |
| TBD-36 | Should PSMD erase the Wi-Fi credentials when the last `Fabric` is removed, or keep them as Matter does? | Erase the Wi-Fi credentials when the last `Fabric` is removed, so the device leaves the network together with its last administrator. | K1 |
| TBD-30 | Which values do the mandatory Basic Information attributes carry? | Development values until the product has its own: `VendorName` `Consius AB`, `ProductName` `PSMD`, `VendorID` `0xFFF1` and `ProductID` `0x8000` (the CSA test IDs the test attestation carries), `HardwareVersion` 1/`1.0`, `SoftwareVersion` 1/`0.1.0`, and a per-unit `SerialNumber`, `PSMD-DEV-<base MAC>`, stored in NVS at first boot; `NodeLabel`/`Location` keep their defaults. Real IDs and a matching attestation are required before shipping; they are set in `CMakeLists.txt` and `sdkconfig.defaults`. | K1 |

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
