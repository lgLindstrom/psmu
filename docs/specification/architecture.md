# Architecture – PSMU

**Power Switch and Measurement Unit (PSMU)**

> Status: Utkast
> Version: 0.1
> Ansvarig: [ ]
> Senast uppdaterad: 2026-11-13
> Relaterade: [`specification.md`](specification.md),
> [`decisions.md`](../project/decisions.md),
> [`glossary.md`](../reference/glossary.md)

> **Äger:** systemöversikt, komponenter, datamodell, Matter-integration och
> byggmiljö.
> **Läs vid behov:** [`specification.md`](specification.md) för beteende och
> [`../reference/external/matter-1.6.1/INDEX.md`](../reference/external/matter-1.6.1/INDEX.md)
> för Matter-utdragen (sök där, läs inte hela textfilerna).

## 1. Syfte

Det här dokumentet beskriver mjukvarans övergripande uppbyggnad: komponenter,
ansvarsgränser, dataflöden och integrationen med Matter och Home Assistant.
Detaljerat beteende – driftlägen, knapp, lysdioder och mätning – finns i
[`specification.md`](specification.md).

Dokumentet beskriver **avsedd arkitektur**. Exakt implementation, mjukvaruversion
och hårdvarutarget är inte fastställda ännu.

## 2. Systemöversikt

PSMU är en Matter-enhet som styr en `Output` och mäter den anslutna `Load`.
Enheten kan användas fristående i `LOCAL` eller via en `Fabric` i Home Assistant.

```mermaid
flowchart LR
    Load[Load] --- PSMU[PSMU]
    PSMU -- Matter --> MS[Matter Server]
    MS --- HA[Home Assistant]
```

PSMU ansvarar för:

- att växla `Output State` (`ON`/`OFF`),
- att mäta spänning, ström och effekt i `OPERATIONAL`,
- att hantera `Operation Mode` och lokal manövrering,
- att lagra den konfiguration och det tillstånd som krävs vid `Power-up`.

Home Assistant och `Matter Server` ligger utanför PSMU och ansvarar för
`Commissioning`, automationer och användargränssnitt.

## 3. Komponenter

Datamodellen nedan använder fyra komponentbeteckningar. Deras fullständiga namn
och ansvarsgränser är inte fastställda (`TBD-19`); tabellen är en preliminär
tolkning utifrån vilka data respektive komponent äger.

| Komponent | Preliminärt ansvar | Äger data |
|---|---|---|
| **`PSMD`** | Tidsstämplar och sampeldata från mätningen. | `timestamp`, `nsamp` |
| **`MDCB`** | Enhetsidentitet, konfiguration och metadata. | `mac_address`, `friendly_name`, `location`, `geolocation`, `mdcb_firmware` |
| **`MDB`** | Utgångstillstånd och beräknade mätvärden. | `ac_switch`, `vac`, `iac`, `papp`, `pact`, `preact`, `phase_delay`, `frequency`, `energy`, `mdb_firmware`, `mdb_app_id` |
| **`DPS`** | Persistent lagring av data som ska överleva `Power-up`. | `mdb_firmware` |

> **Att undersöka:** komponenternas ansvar och gränssnitt behöver bekräftas innan
> de kan användas som arkitekturbegrepp i krav och tester (`TBD-19`).

## 4. Datamodell

Tabellen är den preliminära datamodellen. Fältnamn, typer och skalning behöver
bekräftas (`TBD-20`).

| Name | Editable | Origin | Storage | Data type |
|---|---|---|---|---|
| timestamp | no | PSMD | PSMD | int |
| mac_address | no | MDCB | MDCB | string |
| ac_switch | yes | MDB | MDB | bool |
| nsamp | no | PSMD | PSMD | int64 |
| vac | no | MDB | MDB Calculated | int64 |
| iac | no | MDB | MDB Calculated | int64 |
| papp | no | MDB | MDB Calculated | int64 |
| pact | no | MDB | MDB Calculated | int64 |
| preact | no | MDB | MDB Calculated | int64 |
| phase_delay | no | MDB | MDB Calculated | int64 |
| frequency | no | MDB | MDB Calculated | int64 |
| energy | no | MDB | MDB Calculated | int64 |
| friendly_name | yes | MDCB | MDCB | String |
| mdb_firmware | no | MDB | DPS | `byte[8]`, `a.b.c => uint16.uint16.uint32` |
| mdcb_firmware | no | MDCB | MDCB | `byte[8]`, `a.b.c => uint16.uint16.uint32` |
| location | yes | MDCB | MDCB | String |
| geolocation | yes | MDCB | MDCB | String |
| mdb_app_id | no | MDB | MDB | String |

`Editable = yes` innebär att värdet kan ändras via gränssnittet. `Origin` anger
var värdet skapas och `Storage` var det lagras.

## 5. Matter och Home Assistant

- `Commissioning` lägger PSMU i en `Fabric` och gör enheten styrbar från Home
  Assistant.
- `Operation Mode` och övergångar specificeras i
  [`specification.md`](specification.md).
- Mätning sker endast i `OPERATIONAL`. Vilka mätvärden som exponeras är delvis
  fastställt (`TBD-10`, `TBD-11`).
- `Pairing Timeout`, `Commissioning Timeout`, `Reconnect` och hantering av flera
  `Fabric` är öppna frågor (`TBD-01`, `TBD-02`, `TBD-06`, `TBD-07`, `TBD-09`).

### 5.1 Enhetstyper

PSMU exponeras som två enhetstyper på samma endpoint (`TBD-C9`):

| Enhetstyp | ID | Klass | Roll |
|---|---|---|---|
| **On/Off Plug-in Unit** | `0x010A` | Simple | Växlar `Output`. |
| **Electrical Sensor** | `0x0510` | Utility | Mäter den anslutna `Load`. |

Core §9.2 tillåter flera enhetstyper i `DeviceTypeList` på samma endpoint. Ett
kluster som är gemensamt för flera enhetstyper på endpointen ska finnas som en
delad instans.

Om PSMU i stället utförs som en fast monterad enhet gäller Matter 1.4+ kravet
att använda **Mounted On/Off Control** (`0x010F`), med On/Off Plug-in Unit som
valfri extra post i `DeviceTypeList`. Monteringssättet är inte fastställt
(`TBD-23`).

### 5.2 Kluster

| Kluster | ID | Krävs av | Conformance | Användning |
|---|---|---|---|---|
| Identify | `0x0003` | On/Off Plug-in Unit | M | Identifiering vid `Commissioning`. |
| Groups | `0x0004` | On/Off Plug-in Unit | M | Gruppstyrning. |
| On/Off | `0x0006` | On/Off Plug-in Unit | M | `OnOff` speglar `Output State`. |
| Level Control | `0x0008` | On/Off Plug-in Unit | O | Rekommenderad för gruppstyrning; får inte påverka `Output` utöver av/på. |
| Scenes Management | `0x0062` | On/Off Plug-in Unit | M | Scener i Home Assistant. |
| Occupancy Sensing | `0x0406` | On/Off Plug-in Unit | O (client) | Används inte av PSMU. |
| Electrical Power Measurement | `0x0090` | Electrical Sensor | O.a+ | Momentana mätvärden. |
| Electrical Energy Measurement | `0x0091` | Electrical Sensor | O.a+ | Energimätning. |
| Power Topology | `0x009C` | Electrical Sensor | M | Anger vilka endpoints mätningen avser (Core §11.8). |

Electrical Sensor kräver minst ett av Electrical Power Measurement och
Electrical Energy Measurement (`O.a+`); PSMU använder båda.

### 5.3 Mappning

Tabellen är ett förslag. Vilka mätvärden som faktiskt exponeras avgörs i
`TBD-10`. Att `phase_delay` rapporteras som `PowerFactor` är beslutat
(`TBD-C10`), men enhet och skalning är öppna (`TBD-22`).

| Datamodell | Matter-attribut | Enhet i Matter |
|---|---|---|
| `ac_switch` | `OnOff` (On/Off) | bool |
| `vac` | `Voltage` eller `RMSVoltage` (Electrical Power Measurement) | mV |
| `iac` | `ActiveCurrent` eller `RMSCurrent` | mA |
| `pact` | `ActivePower` | mW |
| `papp` | `ApparentPower` | mVA |
| `preact` | `ReactivePower` | mVAR |
| `frequency` | `Frequency` | mHz |
| `energy` | `CumulativeEnergyImported` (Electrical Energy Measurement) | mWh |
| `phase_delay` | `PowerFactor` | percent100ths (öppen, `TBD-22`) |

Matter skalar mätvärden i mV, mA, mW och mHz, medan datamodellen anger `int64`
utan enhet. Skalningen ska fastställas (`TBD-20`). Om energimätningen ska
ackumuleras över `Power-up` är öppet (`TBD-12`).

## 6. Nätverk och provisionering

Hur PSMU får Wi-Fi-uppgifter, hur återanslutning sker och hur enheten beter sig
när Home Assistant inte nås är inte fastställt (`TBD-05`, `TBD-06`, `TBD-07`).

## 7. Byggmiljö och körtid

- Mjukvaran byggs med **ESP-IDF 6.1** (`TBD-C8`) och **ESP-Matter**
  `release/v1.6.1` för Matter 1.6.1. Projektet i detta repository innehåller
  endast dokumentation, inte firmware-källkod.
- Var Espressif-dokumentationen finns och hur den slås upp:
  [`../reference/external/esp-idf/INDEX.md`](../reference/external/esp-idf/INDEX.md).
- Hårdvarutarget och byggkonfiguration är inte fastställda.
- ESP-Matter rekommenderar ESP-IDF v6.0.2; avvikelsen utreds i `TBD-21`.
- Firmware- och applikationsversioner lagras enligt datamodellen ovan.

## 8. Säkerhet och uppdatering

Matter-attestering, nyckelhantering, säker lagring och eventuell OTA-uppdatering
är ännu inte utrett. Inga krav eller lösningar är fastställda.

## 9. Öppna frågor

Samtliga öppna frågor och fattade beslut finns i
[`decisions.md`](../project/decisions.md).
