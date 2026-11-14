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
> [`lookup/matter-1.6.1/INDEX.md`](lookup/matter-1.6.1/INDEX.md)
> för Matter-utdragen (sök där, läs inte hela textfilerna).

## 1. Syfte

Det här dokumentet specificerar PSMD: mjukvarubeteende (§3–9), krav med
acceptanskriterier (§10) och mjukvarans övergripande uppbyggnad (§2, §11–15) –
komponenter, ansvarsgränser, dataflöden, datamodell och integrationen med
Matter och Home Assistant.

Dokumentet beskriver firmware för **Matter Controller (MC)** samt gränssnittet
mot **Measurement Unit (MU)**.

Dokumentet beskriver **avsedd arkitektur**. Exakt implementation,
mjukvaruversion och hårdvarutarget är inte fastställda ännu.

## 2. Systemöversikt

PSMD är en Matter-enhet som styr en `Output` och mäter den anslutna `Load`.
Enheten kan användas fristående i `LOCAL` eller via en `Fabric` i Home Assistant.

PSMD består av tre funktionella enheter:

| Enhet | Förkortning | Uppgift |
|---|---|---|
| **Matter Controller** | `MC` | Hanterar gränssnittet mot Home Assistant via Matter-protokollet. |
| **Measurement Unit** | `MU` | Sköter mätningen. |
| **Power Unit** | `PU` | Är gränssnitt mot in- och utsignaler. |

`MC` och `MU` kommunicerar via en I2C-kanal samt direkta signaler.

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

## 3. Operation Modes

| `Operation Mode` | Beskrivning | `Operation Mode LED` |
|---|---|---|
| **`LOCAL`** | Standardläge vid första `Power-up`. Ingen anslutning till Home Assistant. `Short Press` växlar `Output State`. | Snabbt blink i 5 s vid `Power-up`, därefter släckt. |
| **`PAIRING`** | Enheten är tillgänglig för Matter-`Commissioning`. `Short Press` har ingen effekt. | Långsamt blink. |
| **`OPERATIONAL`** | Enheten ingår i en `Fabric` och kan styras via Home Assistant. `Short Press` växlar `Output State`. | Snabbt blink i 5 s vid `Power-up`, därefter fast sken. |

## 4. Övergångar

| Från | Händelse | Till | Kommentar |
|---|---|---|---|
| – | `Power-up`, första gången | `LOCAL` | Standardläge. |
| – | `Power-up`, tidigare ansluten | `OPERATIONAL` | Beteende när Home Assistant inte nås hanteras av ESP-Matter (`TBD-C13`). |
| `LOCAL` | `Long Press` | `PAIRING` | |
| `PAIRING` | Lyckad `Commissioning` | `OPERATIONAL` | Enheten läggs i en `Fabric`. |
| `PAIRING` | `Pairing Timeout` eller misslyckad anslutning | `LOCAL` | `Pairing Timeout` enligt ESP-Matter/Matter (`TBD-C13`). |
| `PAIRING` | `Long Press` | `LOCAL` | `Decommissioning` enligt ESP-Matter (`TBD-C13`). |
| `OPERATIONAL` | `Long Press` | `PAIRING` | |

`Short Press` ändrar aldrig `Operation Mode`; den växlar endast `Output State` i
`LOCAL` och `OPERATIONAL`.

## 5. Button

| Händelse | Definition | Beteende |
|---|---|---|
| **`Short Press`** | Tryckning ≤ 3 s. | Växlar `Output State` i `LOCAL` och `OPERATIONAL`. Ingen effekt i `PAIRING`. |
| **`Long Press`** | Tryckning > 3 s. | Växlar `Operation Mode` enligt tabellen ovan. |
| **`Press Threshold`** | 3 s. | Gräns mellan `Short Press` och `Long Press`. |

Debounce-tid och exakt avläsning av knappen är inte fastställda.

## 6. LEDs

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

## 7. Power-up och lagring

- `Output State` sätts **alltid till `OFF`** vid `Power-up` (`Power-loss Recovery`).
- `Operation Mode` vid `Power-up` följer `Boot Mode`: `LOCAL` första gången,
  annars `OPERATIONAL`.
- Vilket tillstånd som sparas och återställs i övrigt är inte fastställt
  (`TBD-12`).
- `Factory Reset` hanteras av ESP-Matter (`TBD-C13`).

## 8. Mätning

Mätning sker **endast i `OPERATIONAL`**. Inget mäts i `LOCAL` eller `PAIRING`.
Mätningen är aktiv även när `Output State` är `OFF`.

| Storhet | Status |
|---|---|
| Spänning | Fastställd som mätvärde. |
| Ström | Fastställd som mätvärde. |
| Skenbar effekt | Fastställd som mätvärde. |
| Aktiv effekt | Fastställd som mätvärde. |
| Reaktiv effekt | Fastställd som mätvärde. |
| Fasfördröjning (`phase_delay`) | Fastställd som mätvärde; rapporteras som `PowerFactor` (`TBD-C10`), enhet och skalning öppna (`TBD-22`). |
| Frekvens | Inte fastställd (`TBD-10`). |
| Energi | Fastställd som mätvärde, men ackumulering över `Power-up` är inte beslutad (`TBD-12`). |

Mätningen utförs av **MU** med ett fast intervall på **5 s**. Alla 8 mätvärden
är synkrona (samma mätomgång). `MC` skickar ett värde i taget till Home
Assistant.

`Measurement Interval` är därmed fastställt till 5 s (`TBD-C11`), men vilka
värden som rapporteras till Matter avgörs i `TBD-10`. Fältnamn och skalning
finns i §11.

Mätvärden rapporteras som **standardattribut** (ett värde per Matter-attribut)
för att Home Assistant ska kunna använda sin standardmappning. Ingen anpassad
struct med alla värden används (`TBD-C12`).

`phase_delay` rapporteras som `PowerFactor` (`TBD-C10`); enhet och skalning är
inte fastställda (`TBD-22`). Mappningen mellan datamodellens fält och
Matter-attributen finns i §11.

## 9. Lokal och extern styrning

- **`LOCAL`:** endast `Short Press` styr `Output State`.
- **`OPERATIONAL`:** Home Assistant och `Short Press` kan båda ändra
  `Output State`. `Output State LED` följer alltid det aktuella tillståndet.
- **`PAIRING`:** `Output State` behåller sitt aktuella värde; `Short Press` har
  ingen effekt.

## 10. Krav och acceptanskriterier

Krav med ID, källa, acceptanskriterium och spårbarhet till specifikationen i
detta dokument.

| ID | Krav | Källa | Acceptanskriterium | Spec | Test | Status |
|---|---|---|---|---|---|---|
| REQ-F-001 | `Short Press` ska växla `Output State` i `LOCAL` och `OPERATIONAL`. | K1 | Utgången växlar vid tryck ≤ 3 s. | §5 | [ ] | Bekräftad |
| REQ-F-002 | `Long Press` ska växla `Operation Mode` enligt specifikationen. | K1 | `LOCAL`/`OPERATIONAL` → `PAIRING`; `PAIRING` → `LOCAL`. | §4–5 | [ ] | Bekräftad |
| REQ-F-003 | `Output State` ska alltid vara `OFF` vid `Power-up`. | K1 | Utgången är `OFF` efter varje spänningssättning. | §7 | [ ] | Bekräftad |
| REQ-F-004 | Mätning ska endast ske i `OPERATIONAL`. | K1 | Inga mätvärden uppdateras i `LOCAL` eller `PAIRING`. | §8 | [ ] | Bekräftad |
| REQ-F-005 | `Operation Mode LED` och `Output State LED` ska följa specificerade mönster. | K1 | Blink- och skenmönster överensstämmer med specifikationen. | §6 | [ ] | Bekräftad |

Icke-funktionella krav och konkreta acceptanstester kopplade till varje krav
saknas ännu; se luckan i [`decisions.md`](decisions.md).

### Teststrategi

- [ ] Enhetstest av mjukvarumoduler.
- [ ] Integrations- och HIL-test av knapp, LED, relä och mätning.
- [ ] Matter-`Commissioning` mot Home Assistant.
- [ ] Acceptanstest mot kraven ovan.

## 11. Datamodell

Tabellen är den preliminära datamodellen. Fältnamn, typer och skalning behöver
bekräftas (`TBD-20`).

| Name | Editable | Matter-attribut | Enhet i Matter |
|---|---|---|---|
| mac_address | no | – | – |
| onoff | yes | `OnOff` (On/Off) | bool |
| vac | no | `Voltage` eller `RMSVoltage` (Electrical Power Measurement) | mV |
| iac | no | `ActiveCurrent` eller `RMSCurrent` | mA |
| papp | no | `ApparentPower` | mVA |
| pact | no | `ActivePower` | mW |
| preact | no | `ReactivePower` | mVAR |
| phase_delay | no | `PowerFactor` | percent100ths (öppen, `TBD-22`) |
| frequency | no | `Frequency` | mHz |
| energy | no | `CumulativeEnergyImported` (Electrical Energy Measurement) | mWh |
| friendly_name | yes | – | – |
| mdb_firmware | no | – | – |
| mdcb_firmware | no | – | – |
| location | yes | – | – |
| geolocation | yes | – | – |
| mdb_app_id | no | – | – |

`Editable = yes` innebär att värdet kan ändras via gränssnittet.

Tabellen är ett förslag. Vilka mätvärden som faktiskt exponeras avgörs i
`TBD-10`. Att `phase_delay` rapporteras som `PowerFactor` är beslutat
(`TBD-C10`), men enhet och skalning är öppna (`TBD-22`).

Matter skalar mätvärden i mV, mA, mW och mHz, medan datamodellen anger `int64`
utan enhet. Skalningen ska fastställas (`TBD-20`). Om energimätningen ska
ackumuleras över `Power-up` är öppet (`TBD-12`).

## 12. Matter och Home Assistant

- `Commissioning` lägger PSMD i en `Fabric` och gör enheten styrbar från Home
  Assistant.
- `Operation Mode` och övergångar specificeras i §3–4.
- Mätning sker endast i `OPERATIONAL`. Vilka mätvärden som exponeras är delvis
  fastställt (`TBD-10`, `TBD-11`).
- `MC` använder standardattributen i Electrical Power/Energy Measurement; HA:s
  standardmappning används. Korrelation mellan värden i samma prov sker på
  applikationsnivå, inte via struct (`TBD-C12`).
- `Pairing`/`Commissioning` Timeout, `Decommissioning`, `Factory Reset`,
  Wi-Fi-provisionering, beteende när Home Assistant inte nås, `Reconnect`,
  hantering av flera `Fabric` och HA/Matter Server-version hanteras av
  ESP-Matter enligt Matter-standarden (`TBD-C13`).

### 12.1 Enhetstyper

PSMD exponeras som två enhetstyper på samma endpoint (`TBD-C9`):

| Enhetstyp | ID | Klass | Roll |
|---|---|---|---|
| **On/Off Plug-in Unit** | `0x010A` | Simple | Växlar `Output`. |
| **Electrical Sensor** | `0x0510` | Utility | Mäter den anslutna `Load`. |

Core §9.2 tillåter flera enhetstyper i `DeviceTypeList` på samma endpoint. Ett
kluster som är gemensamt för flera enhetstyper på endpointen ska finnas som en
delad instans.

PSMD utförs som **plug-in-enhet** (On/Off Plug-in Unit, `0x010A`); Mounted
On/Off Control (`0x010F`) används inte (`TBD-C14`).

### 12.2 Kluster

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

## 13. Nätverk och provisionering

Wi-Fi-provisionering, återanslutning och beteende när Home Assistant inte nås
hanteras av ESP-Matter enligt Matter-standarden (`TBD-C13`).

## 14. Byggmiljö och körtid

- Mjukvaran byggs med **ESP-IDF 6.1** (`TBD-C8`) och **ESP-Matter**
  `release/v1.6.1` för Matter 1.6.1. Firmwarekällkoden ligger i `firmware/` i
  detta repository och byggs/flashes via `esp-idf`-MCP:n.
- Var Espressif-dokumentationen finns och hur den slås upp:
  [`lookup/esp-idf.md`](lookup/esp-idf.md).
- Hårdvarutarget och byggkonfiguration är inte fastställda.
- Praktiska bygg-, flash- och felsökningssteg samt loggkonventioner finns i
  [`../firmware/README.md`](../firmware/README.md).
- ESP-Matter rekommenderar ESP-IDF v6.0.2; avvikelsen utreds i `TBD-21`.
- Firmware- och applikationsversioner lagras enligt datamodellen ovan.

## 15. Säkerhet och uppdatering

Matter-attestering, nyckelhantering, säker lagring och eventuell OTA-uppdatering
är ännu inte utrett. Inga krav eller lösningar är fastställda.
