# Specification – PSMD

**Power Switch and Measurement Device (PSMD)**

> Status: Utkast
> Version: 0.1
> Ansvarig: [ ]
> Senast uppdaterad: 2026-11-14
> Relaterade: [`decisions.md`](decisions.md),
> [`manual.md`](manual.md),
> [`glossary.md`](glossary.md)

> **Äger:** driftlägen, knapp, lysdioder, `Power-up`, mätning, krav,
> arkitektur, datamodell, Matter-integration och byggmiljö.
> **Läs vid behov:** [`decisions.md`](decisions.md) för öppna frågor
> och okända värden, samt
> [`reference/external/matter-1.6.1/INDEX.md`](reference/external/matter-1.6.1/INDEX.md)
> för Matter-utdragen (sök där, läs inte hela textfilerna).

## 1. Syfte

Det här dokumentet specificerar PSMD: mjukvarubeteende (§2–8), krav med
acceptanskriterier (§9) och mjukvarans övergripande uppbyggnad (§10–16) –
komponenter, ansvarsgränser, dataflöden, datamodell och integrationen med
Matter och Home Assistant.

Dokumentet beskriver **avsedd arkitektur**. Exakt implementation,
mjukvaruversion och hårdvarutarget är inte fastställda ännu.

## 2. Operation Modes

| `Operation Mode` | Beskrivning | `Operation Mode LED` |
|---|---|---|
| **`LOCAL`** | Standardläge vid första `Power-up`. Ingen anslutning till Home Assistant. `Short Press` växlar `Output State`. | Snabbt blink i 5 s vid `Power-up`, därefter släckt. |
| **`PAIRING`** | Enheten är tillgänglig för Matter-`Commissioning`. `Short Press` har ingen effekt. | Långsamt blink. |
| **`OPERATIONAL`** | Enheten ingår i en `Fabric` och kan styras via Home Assistant. `Short Press` växlar `Output State`. | Snabbt blink i 5 s vid `Power-up`, därefter fast sken. |

## 3. Övergångar

| Från | Händelse | Till | Kommentar |
|---|---|---|---|
| – | `Power-up`, första gången | `LOCAL` | Standardläge. |
| – | `Power-up`, tidigare ansluten | `OPERATIONAL` | Beteende när Home Assistant inte nås är inte fastställt (`TBD-06`). |
| `LOCAL` | `Long Press` | `PAIRING` | |
| `PAIRING` | Lyckad `Commissioning` | `OPERATIONAL` | Enheten läggs i en `Fabric`. |
| `PAIRING` | `Pairing Timeout` eller misslyckad anslutning | `LOCAL` | Preliminärt 3 minuter (`TBD-01`). |
| `PAIRING` | `Long Press` | `LOCAL` | `Decommissioning` enligt `TBD-03`. |
| `OPERATIONAL` | `Long Press` | `PAIRING` | |

`Short Press` ändrar aldrig `Operation Mode`; den växlar endast `Output State` i
`LOCAL` och `OPERATIONAL`.

## 4. Button

| Händelse | Definition | Beteende |
|---|---|---|
| **`Short Press`** | Tryckning ≤ 3 s. | Växlar `Output State` i `LOCAL` och `OPERATIONAL`. Ingen effekt i `PAIRING`. |
| **`Long Press`** | Tryckning > 3 s. | Växlar `Operation Mode` enligt tabellen ovan. |
| **`Press Threshold`** | 3 s. | Gräns mellan `Short Press` och `Long Press`. |

Debounce-tid och exakt avläsning av knappen är inte fastställda.

## 5. LEDs

| LED | Beteende |
|---|---|
| **`Operation Mode LED`** (`LED1`) | Visar `Operation Mode`: snabbt blink i 5 s vid `Power-up` (både `LOCAL` och `OPERATIONAL`), släckt i `LOCAL`, långsamt blink i `PAIRING`, fast sken i `OPERATIONAL`. |
| **`Output State LED`** (`LED2`) | Speglar `Output State`: tänd vid `ON`, släckt vid `OFF`, oberoende av `Operation Mode`. |

| Mönster | Tänd / släckt | Frekvens |
|---|---|---|
| **`Fast Blink`** | 0,25 s / 0,25 s | 2 Hz, i 5 s vid `Power-up`. |
| **`Slow Blink`** | 0,5 s / 0,5 s | 1 Hz. |
| **`Solid`** | Tänd | – |
| **`Off`** | Släckt | – |

## 6. Power-up och lagring

- `Output State` sätts **alltid till `OFF`** vid `Power-up` (`Power-loss Recovery`).
- `Operation Mode` vid `Power-up` följer `Boot Mode`: `LOCAL` första gången,
  annars `OPERATIONAL`.
- Vilket tillstånd som sparas och återställs i övrigt är inte fastställt
  (`TBD-12`).
- Om och hur `Factory Reset` ska finnas är inte beslutat (`TBD-04`).

## 7. Mätning

Mätning sker **endast i `OPERATIONAL`**. Inget mäts i `LOCAL` eller `PAIRING`.
Mätningen är aktiv även när `Output State` är `OFF`.

| Storhet | Status |
|---|---|
| Spänning | Fastställd som mätvärde. |
| Ström | Fastställd som mätvärde. |
| Aktiv effekt | Fastställd som mätvärde. |
| Energi | Fastställd som mätvärde, men ackumulering över `Power-up` är inte beslutad (`TBD-12`). |
| Frekvens | Inte fastställd (`TBD-10`). |
| Effektfaktor | Inte fastställd (`TBD-10`). |

`Measurement Interval` är inte fastställt (`TBD-11`), och vilka värden som
rapporteras till Matter avgörs i `TBD-10`. Fältnamn och skalning finns i §12.

`phase_delay` rapporteras som `PowerFactor` (`TBD-C10`); enhet och skalning är
inte fastställda (`TBD-22`). Mappningen mellan datamodellens fält och
Matter-attributen finns i §13.3.

## 8. Lokal och extern styrning

- **`LOCAL`:** endast `Short Press` styr `Output State`.
- **`OPERATIONAL`:** Home Assistant och `Short Press` kan båda ändra
  `Output State`. `Output State LED` följer alltid det aktuella tillståndet.
- **`PAIRING`:** `Output State` behåller sitt aktuella värde; `Short Press` har
  ingen effekt.

## 9. Krav och acceptanskriterier

Krav med ID, källa, acceptanskriterium och spårbarhet till specifikationen i
detta dokument.

| ID | Krav | Källa | Acceptanskriterium | Spec | Test | Status |
|---|---|---|---|---|---|---|
| REQ-F-001 | `Short Press` ska växla `Output State` i `LOCAL` och `OPERATIONAL`. | K1 | Utgången växlar vid tryck ≤ 3 s. | §4 | [ ] | Bekräftad |
| REQ-F-002 | `Long Press` ska växla `Operation Mode` enligt specifikationen. | K1 | `LOCAL`/`OPERATIONAL` → `PAIRING`; `PAIRING` → `LOCAL`. | §3–4 | [ ] | Bekräftad |
| REQ-F-003 | `Output State` ska alltid vara `OFF` vid `Power-up`. | K1 | Utgången är `OFF` efter varje spänningssättning. | §6 | [ ] | Bekräftad |
| REQ-F-004 | Mätning ska endast ske i `OPERATIONAL`. | K1 | Inga mätvärden uppdateras i `LOCAL` eller `PAIRING`. | §7 | [ ] | Bekräftad |
| REQ-F-005 | `Operation Mode LED` och `Output State LED` ska följa specificerade mönster. | K1 | Blink- och skenmönster överensstämmer med specifikationen. | §5 | [ ] | Bekräftad |

Icke-funktionella krav och konkreta acceptanstester kopplade till varje krav
saknas ännu; se luckan i [`decisions.md`](decisions.md).

### Teststrategi

- [ ] Enhetstest av mjukvarumoduler.
- [ ] Integrations- och HIL-test av knapp, LED, relä och mätning.
- [ ] Matter-`Commissioning` mot Home Assistant.
- [ ] Acceptanstest mot kraven ovan.

## 10. Systemöversikt

PSMD är en Matter-enhet som styr en `Output` och mäter den anslutna `Load`.
Enheten kan användas fristående i `LOCAL` eller via en `Fabric` i Home Assistant.

```mermaid
flowchart LR
    Load[Load] --- PSMD[PSMD]
    PSMD -- Matter --> MS[Matter Server]
    MS --- HA[Home Assistant]
```

PSMD ansvarar för:

- att växla `Output State` (`ON`/`OFF`),
- att mäta spänning, ström och effekt i `OPERATIONAL`,
- att hantera `Operation Mode` och lokal manövrering,
- att lagra den konfiguration och det tillstånd som krävs vid `Power-up`.

Home Assistant och `Matter Server` ligger utanför PSMD och ansvarar för
`Commissioning`, automationer och användargränssnitt.

## 11. Komponenter

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

## 12. Datamodell

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

## 13. Matter och Home Assistant

- `Commissioning` lägger PSMD i en `Fabric` och gör enheten styrbar från Home
  Assistant.
- `Operation Mode` och övergångar specificeras i §2–3.
- Mätning sker endast i `OPERATIONAL`. Vilka mätvärden som exponeras är delvis
  fastställt (`TBD-10`, `TBD-11`).
- `Pairing Timeout`, `Commissioning Timeout`, `Reconnect` och hantering av flera
  `Fabric` är öppna frågor (`TBD-01`, `TBD-02`, `TBD-06`, `TBD-07`, `TBD-09`).

### 13.1 Enhetstyper

PSMD exponeras som två enhetstyper på samma endpoint (`TBD-C9`):

| Enhetstyp | ID | Klass | Roll |
|---|---|---|---|
| **On/Off Plug-in Unit** | `0x010A` | Simple | Växlar `Output`. |
| **Electrical Sensor** | `0x0510` | Utility | Mäter den anslutna `Load`. |

Core §9.2 tillåter flera enhetstyper i `DeviceTypeList` på samma endpoint. Ett
kluster som är gemensamt för flera enhetstyper på endpointen ska finnas som en
delad instans.

Om PSMD i stället utförs som en fast monterad enhet gäller Matter 1.4+ kravet
att använda **Mounted On/Off Control** (`0x010F`), med On/Off Plug-in Unit som
valfri extra post i `DeviceTypeList`. Monteringssättet är inte fastställt
(`TBD-23`).

### 13.2 Kluster

| Kluster | ID | Krävs av | Conformance | Användning |
|---|---|---|---|---|
| Identify | `0x0003` | On/Off Plug-in Unit | M | Identifiering vid `Commissioning`. |
| Groups | `0x0004` | On/Off Plug-in Unit | M | Gruppstyrning. |
| On/Off | `0x0006` | On/Off Plug-in Unit | M | `OnOff` speglar `Output State`. |
| Level Control | `0x0008` | On/Off Plug-in Unit | O | Rekommenderad för gruppstyrning; får inte påverka `Output` utöver av/på. |
| Scenes Management | `0x0062` | On/Off Plug-in Unit | M | Scener i Home Assistant. |
| Occupancy Sensing | `0x0406` | On/Off Plug-in Unit | O (client) | Används inte av PSMD. |
| Electrical Power Measurement | `0x0090` | Electrical Sensor | O.a+ | Momentana mätvärden. |
| Electrical Energy Measurement | `0x0091` | Electrical Sensor | O.a+ | Energimätning. |
| Power Topology | `0x009C` | Electrical Sensor | M | Anger vilka endpoints mätningen avser (Core §11.8). |

Electrical Sensor kräver minst ett av Electrical Power Measurement och
Electrical Energy Measurement (`O.a+`); PSMD använder båda.

### 13.3 Mappning

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

## 14. Nätverk och provisionering

Hur PSMD får Wi-Fi-uppgifter, hur återanslutning sker och hur enheten beter sig
när Home Assistant inte nås är inte fastställt (`TBD-05`, `TBD-06`, `TBD-07`).

## 15. Byggmiljö och körtid

- Mjukvaran byggs med **ESP-IDF 6.1** (`TBD-C8`) och **ESP-Matter**
  `release/v1.6.1` för Matter 1.6.1. Firmwarekällkoden ligger i `firmware/` i
  detta repository och byggs/flashes via `esp-idf`-MCP:n.
- Var Espressif-dokumentationen finns och hur den slås upp:
  [`reference/external/esp-idf/INDEX.md`](reference/external/esp-idf/INDEX.md).
- Loggning sker med ESP-IDF-makrona `ESP_LOGI`, `ESP_LOGW`, `ESP_LOGE`,
  `ESP_LOGD` och `ESP_LOGV` – inte `printf`.
- Varje modul som loggar definierar en `TAG` (`static const char *TAG`). `TAG`
  börjar alltid med `psmd_` följt av modulnamnet, till exempel `psmd_meas`, så
  att PSMD:s egna loggar kan filtreras fram från ESP-IDF- och
  ESP-Matter-bibliotekens.
- Hårdvarutarget och byggkonfiguration är inte fastställda.
- ESP-Matter rekommenderar ESP-IDF v6.0.2; avvikelsen utreds i `TBD-21`.
- Firmware- och applikationsversioner lagras enligt datamodellen ovan.

## 16. Säkerhet och uppdatering

Matter-attestering, nyckelhantering, säker lagring och eventuell OTA-uppdatering
är ännu inte utrett. Inga krav eller lösningar är fastställda.
