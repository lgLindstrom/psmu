# Specification – PSMU

**Power Switch and Measurement Unit (PSMU)**

> Status: Utkast
> Version: 0.1
> Ansvarig: [ ]
> Senast uppdaterad: 2026-11-04
> Relaterade: [`architecture.md`](architecture.md),
> [`decisions.md`](../project/decisions.md),
> [`glossary.md`](../reference/glossary.md)

## 1. Syfte

Det här dokumentet specificerar PSMU:s mjukvarubeteende: `Operation Mode`,
övergångar, knapp, lysdioder, `Power-up`, mätning och lokal styrning.
Arkitektur och datamodell finns i [`architecture.md`](architecture.md).

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

`Measurement Interval` och vilka värden som rapporteras till Matter är inte
fastställda (`TBD-11`). Fältnamn och skalning finns i
[`architecture.md`](architecture.md#4-datamodell).

## 8. Lokal och extern styrning

- **`LOCAL`:** endast `Short Press` styr `Output State`.
- **`OPERATIONAL`:** Home Assistant och `Short Press` kan båda ändra
  `Output State`. `Output State LED` följer alltid det aktuella tillståndet.
- **`PAIRING`:** `Output State` behåller sitt aktuella värde; `Short Press` har
  ingen effekt.

## 9. Öppna frågor

Samtliga öppna frågor och fattade beslut finns i
[`decisions.md`](../project/decisions.md).
