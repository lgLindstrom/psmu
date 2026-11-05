# Terminologi – PSMU

Engelska termer som används i mjukvara, dokumentation och fortsatt diskussion.
Svensk förklaring anges för läsbarhet.

Status: `Bekräftad` = fastställd, `Förslag` = föreslagen term, `TBD` = att
undersöka/fastställa. Öppna frågor med källa finns i
[`decisions.md`](../project/decisions.md).

Beteckningarna `LED1` och `LED2` används endast om det fysiska kortets märkning.
I text och mjukvara används de beskrivande namnen **`Operation Mode LED`** och
**`Output State LED`**. Godkända förkortningar när det passar: **`Mode LED`** och
**`Output LED`**.

> **Äger:** terminologi och godkända begrepp.
> **Läs vid behov:** [`../project/decisions.md`](../project/decisions.md) för
> terminologifrågor. Sök i tabellerna i stället för att läsa hela filen.

## Driftlägen (Operation Modes)

| Term (EN) | Svenska | Definition | Status |
|---|---|---|---|
| **Operation Mode** | Driftläge | Enhetens övergripande tillstånd: `LOCAL`, `PAIRING` eller `OPERATIONAL`. | Bekräftad |
| **`LOCAL`** (Local Mode) | Lokalt läge | Standardläge vid första `Power-up`. Ingen anslutning till Home Assistant. `Operation Mode LED` blinkar snabbt i 5 s vid `Power-up` och är därefter släckt. `Short Press` växlar `Output State`. | Bekräftad |
| **`PAIRING`** (Pairing Mode) | Parkopplingsläge | Läge där enheten är tillgänglig för Matter-`Commissioning`. `Operation Mode LED` blinkar långsamt. `Short Press` har ingen effekt. Lämnas vid lyckad anslutning, vid `Pairing Timeout` eller genom `Long Press` → `LOCAL`. | Bekräftad |
| **`OPERATIONAL`** (Operational Mode) | Anslutet driftläge | Läge efter lyckad Matter-`Commissioning`. Enheten ingår i en `Fabric` och kan styras via Home Assistant. `Operation Mode LED` lyser fast. `Short Press` växlar `Output State`. Enheten startar i detta läge vid `Power-up` om den tidigare varit ansluten; `Operation Mode LED` blinkar då snabbt i 5 s innan den övergår till fast sken. `Long Press` går tillbaka till `PAIRING`. | Bekräftad |
| **Transition** | Övergång | Byte av `Operation Mode`, t.ex. `LOCAL → PAIRING`. | Förslag |

## Styrning och indikering (Controls and Indicators)

| Term (EN) | Svenska | Definition | Status |
|---|---|---|---|
| **Button** | Knapp | Enhetens enda fysiska manöverdon. | Bekräftad |
| **Short Press** | Kort tryck | Tryckning ≤ 3 s. Växlar `Output State` i `LOCAL` och `OPERATIONAL`. Har ingen effekt i `PAIRING`. | Bekräftad |
| **Long Press** | Långt tryck | Tryckning > 3 s. Växlar `Operation Mode` till `PAIRING` från `LOCAL` och `OPERATIONAL`. I `PAIRING` växlar `Long Press` tillbaka till `LOCAL`. | Bekräftad |
| **Press Threshold** | Trycktröskel | 3 s – gränsen mellan `Short Press` och `Long Press`. | Bekräftad |
| **`Operation Mode LED`** (LED1) | Lägeslysdiod | Visar `Operation Mode`: blinkar snabbt i 5 s vid `Power-up` (både `LOCAL` och `OPERATIONAL`), är därefter släckt i `LOCAL`, blinkar långsamt i `PAIRING` och lyser fast i `OPERATIONAL`. | Bekräftad |
| **`Output State LED`** (LED2) | Utgångslysdiod | Speglar `Output State`: tänd vid `ON`, släckt vid `OFF`, oberoende av `Operation Mode`. | Bekräftad |
| **Slow Blink** | Långsamt blink | Blinkmönster som indikerar `PAIRING`: 0,5 s tänd / 0,5 s släckt (1 Hz). | Bekräftad |
| **Fast Blink** | Snabbt blink | Blinkmönster som indikerar `Power-up` under 5 s: 0,25 s tänd / 0,25 s släckt (2 Hz). | Bekräftad |
| **Solid** | Fast sken | Lysdiod som lyser utan att blinka. | Bekräftad |
| **Off** | Släckt | Lysdiod som inte lyser. | Bekräftad |

## Utgång och last (Output and Load)

| Term (EN) | Svenska | Definition | Status |
|---|---|---|---|
| **Output** | Utgång | Den nätutgång som ansluten utrustning matas från. | Bekräftad |
| **Output State** | Utgångstillstånd | `ON` eller `OFF` för `Output`. | Bekräftad |
| **Relay** | Relä | Den fysiska komponent som sluter/öppnar `Output`. | Förslag |
| **Load** | Last | Den utrustning som är ansluten till `Output`. | Bekräftad |
| **Toggle** | Växla | Ändra `Output State` mellan `ON` och `OFF`. | Bekräftad |

## Anslutning (Connectivity)

| Term (EN) | Svenska | Definition | Status |
|---|---|---|---|
| **Commissioning** | Idrifttagning / parkoppling | Matter-processen då enheten läggs till i en `Fabric`. | Bekräftad |
| **Pairing Window** | Parkopplingsfönster | Tidsperiod under `PAIRING` då `Commissioning` är tillåten. Stängs av `Pairing Timeout`. | Förslag |
| **Pairing Timeout** | Parkopplingstimeout | Tidsgräns som avslutar `PAIRING` och återgår till `LOCAL` om ingen anslutning lyckats. **Preliminärt 3 minuter – ska verifieras mot Matter-standarden.** | TBD |
| **Commissioning Timeout** | Idrifttagningstimeout | Tidsgräns för själva `Commissioning`-processen. Ska verifieras mot Matter-standarden. | TBD |
| **Fabric** | Fabric | Matter-domän som en enhet tillhör; skapas av t.ex. Home Assistant. | Bekräftad |
| **Matter Server** | Matter-server | Home Assistant-komponent som krävs för Matter-anslutning. | Bekräftad |
| **Wi-Fi Provisioning** | Wi-Fi-konfiguration | Hur PSMU får Wi-Fi-uppgifter: följer det sätt som Matter och Home Assistant föreskriver. Exakt flöde TBD. | TBD |
| **Home Assistant (HA)** | Home Assistant | Systemet som PSMU kan integreras med. | Bekräftad |
| **Fallback** | Återgång | Automatiskt byte till `LOCAL` när `PAIRING` misslyckas eller `Pairing Timeout` löser ut. | Förslag |
| **Reconnect** | Återanslutning | Enhetens försök att återuppta kontakten med sin `Fabric` efter omstart. Beteende TBD enligt Matter-standarden. | TBD |
| **Decommissioning** | Avregistrering | Att lämna `PAIRING` med `Long Press` bör även ta bort enhetens information i Home Assistant. **Ska undersökas** – exakt metod och ansvar enligt Matter-standarden. | TBD |

## Livscykel och lagring (Lifecycle and Persistence)

| Term (EN) | Svenska | Definition | Status |
|---|---|---|---|
| **Power-up** | Spänningssättning | När enheten får spänning från vägguttaget. | Bekräftad |
| **Boot Mode** | Startläge | `Operation Mode` som enheten startar i vid `Power-up`: `LOCAL` första gången, annars `OPERATIONAL` om enheten tidigare varit ansluten. | Bekräftad |
| **Persisted State** | Lagrat tillstånd | Tillstånd som sparas i icke-flyktigt minne och återställs vid `Power-up`. | Förslag |
| **Power-loss Recovery** | Återstart efter strömavbrott | `Output State` sätts **alltid till `OFF`** vid `Power-up`, oavsett tidigare tillstånd. | Bekräftad |
| **Factory Reset** | Fabriksåterställning | Nollställning av konfiguration och `Fabric`-tillhörighet. **Inget beslut taget** om det ska finnas eller hur det triggas. | TBD |

## Mätning (Measurement)

Mätning sker **endast i `OPERATIONAL`**. Inget mäts i `LOCAL` eller `PAIRING`.
Mätningen är aktiv även när `Output State` är `OFF` (ström och effekt förväntas
då vara 0).

| Term (EN) | Svenska | Definition | Status |
|---|---|---|---|
| **Measurement** | Mätning | Insamling av elektriska storheter från `Load`. Endast i `OPERATIONAL`, även vid `Output State = OFF`. | Bekräftad |
| **Voltage** | Spänning | Uppmätt spänning över `Load` (V). | Förslag |
| **Current** | Ström | Uppmätt ström genom `Load` (A). | Förslag |
| **Active Power** | Aktiv effekt | Uppmätt aktiv effekt (W). | Förslag |
| **Energy** | Energi | Ackumulerad förbrukning över tid (kWh). | Förslag |
| **Frequency** | Frekvens | Nätfrekvens (Hz), om den mäts. | TBD |
| **Power Factor** | Effektfaktor | Förhållandet mellan aktiv och skenbar effekt, om den mäts. | TBD |
| **Measurement Interval** | Mätintervall | Hur ofta värden uppdateras. | TBD |

## Undvik tvetydiga ord

| Undvik | Använd i stället | Skäl |
|---|---|---|
| "status" (ensamt) | `Output State`, `Operation Mode` eller `Connection Status` | "Status" kan avse flera olika saker. |
| "mode" (ensamt) | `Operation Mode` + värdet (`LOCAL`/`PAIRING`/`OPERATIONAL`) | Tydliggör vilken typ av läge som avses. |
| "på/av" i teknisk text | `ON` / `OFF` | Konsekvent med mjukvarans nomenklatur. |
| "lampa", "LED1", "LED2" i löptext | `Operation Mode LED` / `Output State LED` | Beskrivande beteckningar; LED1/LED2 är endast fysisk märkning. |
