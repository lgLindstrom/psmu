# Terminology – PSMD

English terms used in software, documentation and continued discussion. The
term is the name used in the firmware and in all documents; the Swedish column
holds the wording used in [`manual.md`](manual.md), which is written in Swedish
and owns the behavior.

The terminology is split in two. **Terms in the manual** are the user-visible
terms; they are derived from `manual.md` and the Swedish column is the wording
the manual uses. **Internal terms** are used in the specification and the
firmware but do not appear in the manual; they have no Swedish wording.

Status: `Confirmed` = established, `Proposed` = proposed term, `TBD` = to
investigate/decide. Open questions with source are in
[`decisions.md`](decisions.md).

`LED1` and `LED2` are the two LEDs on PSMD; the descriptive names
**`Operation Mode LED`** and **`Output State LED`** are used in text and
software. Approved abbreviations where they fit: **`Mode LED`** and
**`Output LED`**.

> **Owner:** terminology and approved terms.
> **Read when needed:** [`decisions.md`](decisions.md) for terminology
> questions. Search the tables instead of reading the whole file.

## Terms in the manual

### Operation Modes

| Term (EN) | Swedish (manual.md) | Definition | Status |
|---|---|---|---|
| **Operation Mode** | Driftläge | The overall state of the device: `LOCAL`, `PAIRING` or `OPERATIONAL`. | Confirmed |
| **`LOCAL`** (Local Mode) | Lokalt läge | Default mode at the first `Power-up`; no connection to Home Assistant. See `manual.md` §5.3. | Confirmed |
| **`PAIRING`** (Pairing Mode) | Parkopplingsläge | Mode where the device is available for Matter `Commissioning`. See `manual.md` §5.3. | Confirmed |
| **`OPERATIONAL`** (Operational Mode) | Anslutet läge | Mode after a successful Matter `Commissioning`; the device belongs to a `Fabric` and can be controlled from Home Assistant. See `manual.md` §5.3. | Confirmed |

### Controls and Indicators

| Term (EN) | Swedish (manual.md) | Definition | Status |
|---|---|---|---|
| **Button** | Knapp | The device's only physical control. | Confirmed |
| **Short Press** | Kort tryck | Press ≤ 3 s. | Confirmed |
| **Long Press** | Långt tryck | Press > 3 s. | Confirmed |
| **Press Threshold** | Trycktröskel | 3 s – the boundary between `Short Press` and `Long Press`. | Confirmed |
| **`Operation Mode LED`** | Lägeslysdiod | Shows `Operation Mode`; also called `LED1`. See `manual.md` §5.2. | Confirmed |
| **`Output State LED`** | Utgångslysdiod | Mirrors `Output State`; also called `LED2`. See `manual.md` §5.2. | Confirmed |
| **Slow Blink** | Långsamt blink | 0.5 s on / 0.5 s off (1 Hz). | Confirmed |
| **Fast Blink** | Snabbt blink | 0.25 s on / 0.25 s off (2 Hz). | Confirmed |
| **Solid** | Fast sken | An LED that is on without blinking. | Confirmed |
| **Off** | Släckt | An LED that is not lit. | Confirmed |

### Output and Load

| Term (EN) | Swedish (manual.md) | Definition | Status |
|---|---|---|---|
| **Output** | Utgång | The mains outlet that the connected equipment is supplied from. | Confirmed |
| **Output State** | Utgångstillstånd | `ON` or `OFF` for the `Output`. | Confirmed |
| **Load** | Utrustning | The equipment connected to the `Output`. | Confirmed |
| **Toggle** | Växla | Change `Output State` between `ON` and `OFF`. | Confirmed |

### Connectivity

| Term (EN) | Swedish (manual.md) | Definition | Status |
|---|---|---|---|
| **Commissioning** | Idrifttagning / parkoppling | The Matter process by which the device is added to a `Fabric`. | Confirmed |
| **Pairing Timeout** | Parkopplingstimeout | Time limit that ends `PAIRING` and returns to `LOCAL` if no connection succeeded. 3 minutes per the manual; verification against the Matter specification is open (`TBD-C13`). | TBD |
| **Fabric** | Fabric | Matter domain that a device belongs to; created by e.g. Home Assistant. | Confirmed |
| **Matter Server** | Matter Server | Home Assistant component required for the Matter connection. | Confirmed |
| **Home Assistant (HA)** | Home Assistant | The system PSMD can be integrated with. | Confirmed |
| **Decommissioning** | Avregistrering | Removing the device from its `Fabric` and from Home Assistant. Handled by ESP-Matter (`TBD-C13`); the user-facing method is to be confirmed. | TBD |

### Lifecycle and Persistence

| Term (EN) | Swedish (manual.md) | Definition | Status |
|---|---|---|---|
| **Power-up** | `Power-up` (spänningssättning) | When the device is supplied with power from the wall outlet. | Confirmed |
| **Power-loss Recovery** | Återstart efter strömavbrott | `Output State` is **always set to `OFF`** at `Power-up`, regardless of the previous state. | Confirmed |
| **Factory Reset** | Fabriksåterställning | Reset of configuration and `Fabric` membership. Handled by ESP-Matter (`TBD-C13`); how it is described in the manual is open. | TBD |

### Measurement

The behavior is in `manual.md` §6.2.

| Term (EN) | Swedish (manual.md) | Definition | Status |
|---|---|---|---|
| **Measurement** | Mätning | Collection of electrical quantities from the `Load`. | Confirmed |
| **Measured Value** | Mätvärde | A quantity produced by the `MU` and republished by `MC` as a Matter attribute. `MC` never measures or derives one. | Confirmed |
| **Voltage** | Spänning | Measured voltage across the `Load` (V). | Proposed |
| **Current** | Ström | Measured current through the `Load` (A). | Proposed |
| **Apparent Power** | Skenbar effekt | Voltage times current (VA). | Proposed |
| **Active Power** | Aktiv effekt | Measured active power (W). | Proposed |
| **Reactive Power** | Reaktiv effekt | Power that oscillates between source and `Load` (var). | Proposed |
| **Phase Delay** | Fasförskjutning | Phase shift between voltage and current. Reported as `PowerFactor` in percent100ths (`TBD-C10`, `TBD-C19`). | Proposed |
| **Frequency** | Frekvens | Mains frequency (Hz). | Proposed |
| **Energy** | Energiförbrukning | Accumulated consumption over time (kWh). Accumulation across `Power-up` is open (`TBD-12`). | Proposed |

## Internal terms

Used in the specification and the firmware; not in the manual, so no Swedish
wording.

### Functional units

| Term (EN) | Definition | Status |
|---|---|---|
| **Matter Controller** | Functional unit that handles the interface to Home Assistant through the Matter protocol. | Confirmed |
| **MC** | Abbreviation for `Matter Controller`. | Confirmed |
| **Measurement Unit** | Functional unit that performs the measurement and produces the measured values. | Confirmed |
| **MU** | Abbreviation for `Measurement Unit`. | Confirmed |
| **Power Unit** | Functional unit that is the interface to the input and output signals. | Confirmed |
| **PU** | Abbreviation for `Power Unit`. | Confirmed |
| **I2C** | Communication channel between `MC` and `MU` (together with direct signals). | Confirmed |

### Modeling and protocol

| Term (EN) | Definition | Status |
|---|---|---|
| **Transition** | Change of `Operation Mode`, e.g. `LOCAL → PAIRING`. | Proposed |
| **Relay** | The physical component that closes/opens the `Output`. | Proposed |
| **Boot Mode** | The `Operation Mode` the device starts in at `Power-up`: `LOCAL` the first time, otherwise `OPERATIONAL` if the device was previously connected. | Confirmed |
| **Persisted State** | State that is stored in non-volatile memory and restored at `Power-up`. | Proposed |
| **Pairing Window** | Period during `PAIRING` when `Commissioning` is allowed. Closed by `Pairing Timeout`. | Proposed |
| **Commissioning Timeout** | Time limit for the `Commissioning` process itself. To be verified against the Matter specification. | TBD |
| **Fallback** | Automatic change to `LOCAL` when `PAIRING` fails or `Pairing Timeout` expires. | Proposed |
| **Reconnect** | The device's attempt to resume contact with its `Fabric` after a restart. Behavior TBD per the Matter specification. | TBD |
| **Wi-Fi Provisioning** | How PSMD gets its Wi-Fi credentials: follows the method prescribed by Matter and Home Assistant. Exact flow TBD. | TBD |
| **Power Factor** | The ratio between active and apparent power. Not measured; `phase_delay` is reported as `PowerFactor` (`TBD-C10`, `TBD-C19`). | Confirmed |
| **Measurement Interval** | How often values are updated: 5 s, one value at a time to Home Assistant. | Confirmed |

## Avoid ambiguous words

| Avoid | Use instead | Why |
|---|---|---|
| "status" on its own | `Output State`, `Operation Mode` or `Connection Status` | "Status" can mean several different things. |
| "mode" on its own | `Operation Mode` plus the value (`LOCAL`/`PAIRING`/`OPERATIONAL`) | Makes clear which kind of mode is meant. |
| "on/off" in technical text | `ON` / `OFF` | Consistent with the software nomenclature. |
| "lamp" | `Operation Mode LED` / `Output State LED` | Descriptive names; `LED1`/`LED2` name the same LEDs. |
