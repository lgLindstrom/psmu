# Användarhandbok – PSMD

**Power Switch and Measurement Device (PSMD)**

> Status: Utkast. Avsnitt markerade med `[...]` behöver fyllas i när produktens
> funktioner och beteende är fastställda. Öppna frågor och varifrån de kommer
> samlas i [`decisions.md`](decisions.md).
>
> **Äger:** användarens väg genom produkten – komma igång, användning,
> felsökning och tekniska data.
> **Läs vid behov:**
> [`specification.md`](specification.md) för exakt
> beteende och [`decisions.md`](decisions.md) för värden som
> ännu är `[...]`.

## 1. Om denna handbok

Den här handboken beskriver hur du som användare kommer igång med och använder
**[Power Switch and Measurement Device (PSMD)]** – en smart plugg som slår på och
stänger av ansluten utrustning samt mäter spänning, ström och effekt. Produkten
består av både hårdvara och mjukvara.

Handboken vänder sig till [målgrupp, t.ex. slutanvändare, installatörer och
administratörer] och förutsätter [förkunskaper, t.ex. inga särskilda
förkunskaper].

Begrepp som `Operation Mode` och `Output State` skrivs med sina engelska
mjukvarutermer och förklaras i ordlistan (avsnitt 10).

- **Version:** [dokumentversion]
- **Gäller produktversion:** [hårdvaruversion / mjukvaruversion]
- **Senast uppdaterad:** [datum]

## 2. Produktöversikt

PSMD är en smart plugg som placeras mellan vägguttaget och den utrustning du vill
styra. Med PSMD kan du:

- **Slå på och stänga av** ansluten utrustning – lokalt eller via Home Assistant.
- **Mäta spänning och ström** i den anslutna utrustningen (värdena visas i `OPERATIONAL`).
- **Följa effekt och energiförbrukning** över tid i Home Assistant (se avsnitt 6).
- **Integrera med Home Assistant** via Matter för automationer och gemensam
  styrning (se avsnitt 4.4).

PSMD består av en hårdvaruenhet (själva pluggen) och en mjukvarudel som används
för att styra och läsa av enheten.

### 2.1 Användningssätt

PSMD kan användas på två sätt:

- **Lokalt (fristående):** pluggen sköter styrning (`ON`/`OFF`) på egen hand, utan
  molntjänst eller internetuppkoppling. Inga mätvärden visas och ingen data lämnar
  den lokala miljön.
- **Integrerat med Home Assistant:** om PSMD ansluts till Wi-Fi och du har en
  **Matter Server** i Home Assistant kan enheten läggas till där. Du kan då styra
  och läsa av PSMD tillsammans med dina övriga enheter och automationer.

> Matter-integrationen kräver att Home Assistant har en Matter Server installerad
> och att PSMD och Home Assistant finns i samma nätverk.

I `OPERATIONAL` fungerar knappen fortfarande, så lokal manövrering finns kvar
även när enheten är parkopplad.

### 2.2 Ingår i förpackningen

| Del | Antal | Kommentar |
|---|---|---|
| PSMD-enhet | 1 | Smart plugg med mätfunktion |
| [Snabbstart] | 1 | |
| [Övrigt tillbehör] | | |

### 2.3 Systemkrav (mjukvara)

| Krav | Minsta | Rekommenderat |
|---|---|---|
| Användning lokalt | Ingen extra mjukvara | – |
| Home Assistant | Version [ ] med Matter Server | Senaste version |
| Wi-Fi | [ 2,4 GHz ] | [ ] |
| Nätverk | PSMD och Home Assistant i samma nätverk | – |

## 3. Säkerhet och viktig information

Läs igenom detta innan PSMD tas i bruk. Anvisningarna gäller både vid
installation och vid daglig användning.

- PSMD är avsedd för **[inomhusbruk]** och ansluts till ett jordat vägguttag.
- Anslut inte en last som överstiger **[max belastning, t.ex. 16 A / 3680 W]**.
- Täck inte över enheten och placera den inte där den kan utsättas för fukt,
  värme eller mekanisk påverkan.
- Dra alltid ur kontakten innan rengöring eller service.
- [Ytterligare säkerhetsanvisning enligt gällande standard/krav]


## 4. Komma igång

### 4.1 Packa upp och kontrollera

[Steg för att packa upp och verifiera att allt finns med.]

### 4.2 Ansluta hårdvaran

PSMD har två anslutningar: en som ansluts till vägguttaget och en som den
utrustning du vill styra och mäta ansluts till.

1. Sätt PSMD:s ena anslutning i **vägguttaget**.
2. Anslut den **utrustning** som ska styras och mätas i PSMD:s andra anslutning.
3. Kontrollera att PSMD får spänning. `Output State` är alltid `OFF` vid
   `Power-up`, oavsett hur det stod innan strömavbrottet (`Power-loss Recovery`).

> Anslutningsdonens typ och utförande dokumenteras separat tills de är fastställda.

### 4.3 Använda lokalt

PSMD fungerar direkt efter anslutning och kräver ingen molntjänst. Vid första
`Power-up` startar enheten i `LOCAL`.

1. Kontrollera att PSMD får spänning.
2. Tryck kort på knappen (`Short Press`) för att växla `Output State` mellan
   `ON` och `OFF`.
3. `Output State LED` visar aktuellt `Output State`.

Mätvärdena visas i Home Assistant. I `LOCAL` finns ingen anslutning dit, så
värdena visas först i `OPERATIONAL` (avsnitt 4.4). Mätningen pågår ändå och
energiförbrukningen räknas vidare.

### 4.4 Ansluta till Home Assistant via Matter

Förutsättning: Home Assistant med **Matter Server** installerad och ett Wi-Fi-nät
som PSMD kan ansluta till.

Matter är standarden som Home Assistant använder för att hitta och styra PSMD,
och **Matter Server** är den del av Home Assistant som sköter Matter-anslutningen.
Under `PAIRING` blinkar `Operation Mode LED` långsamt medan enheten väntar på att
läggas till.

1. Håll knappen intryckt (`Long Press`, > 3 s) tills `Operation Mode LED` börjar
   blinka långsamt. Enheten är nu i `PAIRING`.
2. Öppna Home Assistant och välj att lägga till en enhet via Matter.
3. Ange parkopplingskoden från PSMD: skanna QR-koden nedan eller skriv den
   manuella koden. Enheten skriver båda i loggen vid `Power-up`.
4. Välj rum och namn, och slutför guiden.
5. När anslutningen lyckas övergår enheten till `OPERATIONAL` och `Operation Mode LED`
   lyser fast.
6. Kontrollera att PSMD visas med sina mätvärden i Home Assistant.

![QR-kod för parkoppling](images/qr-pairing-example.png)

*Exempel: QR-koden som utvecklingskortet skriver i loggen vid `Power-up`
(`MT:Y.K9042C00KA0648G00`, manuell kod `34970112332`). [Var koden för en färdig
enhet finns]*

Om anslutningen misslyckas, eller om `Pairing Timeout` (preliminärt 3 minuter)
löser ut, återgår enheten till `LOCAL`.

Ett `Long Press` i `PAIRING` avbryter också och går tillbaka till `LOCAL`.
Enhetens information bör då även tas bort i Home Assistant – exakt metod enligt
Matter-standarden ska undersökas.

### 4.5 Verifiera installationen

[Beskriv hur användaren ser att allt fungerar, t.ex. statusindikator,
testkörning eller första mätvärde.]

## 5. Översikt av hårdvara och mjukvara

### 5.1 Hårdvarans delar

PSMD har två anslutningar:

- **Ingång:** ansluts till vägguttaget.
- **Utgång:** den utrustning som ska styras och mätas ansluts här.

På enheten finns dessutom:

- en **knapp** för lokal manövrering,
- **`Operation Mode LED`** (LED1) som visar `Operation Mode`,
- **`Output State LED`** (LED2) som visar om utgången är `ON` eller `OFF`.

[Anslutningsdonens typ och utförande dokumenteras när de är fastställda.]

### 5.2 Knapp och lysdioder

| Komponent | Beteende |
|---|---|
| **`Button`** | `Short Press` (≤ 3 s) växlar `Output State` i `LOCAL` och `OPERATIONAL`. `Long Press` (> 3 s) växlar `Operation Mode` till `PAIRING` från `LOCAL` och `OPERATIONAL`, och från `PAIRING` tillbaka till `LOCAL`. |
| **`Operation Mode LED`** | Blinkar snabbt i 5 s vid `Power-up` (2 Hz: 0,25 s tänd / 0,25 s släckt), både i `LOCAL` och `OPERATIONAL`. Därefter släckt i `LOCAL`, långsamt blinkande i `PAIRING` (0,5 s tänd / 0,5 s släckt) och fast sken i `OPERATIONAL`. |
| **`Output State LED`** | Följer `Output State` oberoende av `Operation Mode`: tänd vid `ON`, släckt vid `OFF`. |

Blinkmönstren anges som tänd tid / släckt tid: `Fast Blink` är 2 Hz och
`Slow Blink` 1 Hz. Endast `Operation Mode LED` skiljer driftlägena åt;
`Output State LED` visar bara utgången.

### 5.3 Driftlägen (Operation Modes)

Driftläget beskriver enhetens förhållande till Home Assistant – inte om utgången
är på eller av.

| Driftläge | Beskrivning | Lämnas genom |
|---|---|---|
| **`LOCAL`** | Standardläge vid första `Power-up`. Ingen Home Assistant-anslutning. `Operation Mode LED` blinkar snabbt i 5 s vid `Power-up` och är därefter släckt. `Short Press` växlar `Output State`. | `Long Press` → `PAIRING` |
| **`PAIRING`** | Enheten är tillgänglig för Matter-`Commissioning`. `Operation Mode LED` blinkar långsamt (0,5 s tänd / 0,5 s släckt). `Short Press` har ingen effekt. | Lyckad `Commissioning` → `OPERATIONAL`, `Pairing Timeout`/misslyckande → `LOCAL`, eller `Long Press` → `LOCAL` |
| **`OPERATIONAL`** | Enheten är ansluten till en `Fabric` och kan styras via Home Assistant. `Operation Mode LED` blinkar snabbt i 5 s vid `Power-up` och lyser därefter fast. `Short Press` växlar `Output State`. Enheten startar i detta läge vid `Power-up` om den tidigare varit ansluten. | `Long Press` → `PAIRING` |

**Tidsgränser:**

- `Press Threshold`: 3 s (`Long Press` > 3 s).
- `Pairing Timeout`: preliminärt 3 minuter. Ska verifieras mot Matter-standarden.

> **Att undersöka:** när `PAIRING` lämnas med `Long Press` ska enhetens information
> även tas bort i Home Assistant. Exakt metod och ansvar enligt Matter-standarden
> är inte fastställt (`Decommissioning`).

> **Att undersöka:** `Factory Reset` hanteras av ESP-Matter (`TBD-C13`), men om
> och hur den ska beskrivas i handboken är inte fastställt.

> Termerna förklaras i [ordlistan](glossary.md). Beteendet vid `Power-up` i
> `OPERATIONAL` när Home Assistant inte nås är ännu inte fastställt – ska
> undersökas mot Matter-standarden.

### 5.4 Mjukvarans gränssnitt

[Beskriv huvudvyer, menyer och viktiga funktioner – gärna med skärmbilder.]

## 6. Använda produkten

### 6.1 Slå på och stänga av

- **Lokalt (`LOCAL`):** ett `Short Press` på knappen växlar `Output State`.
  `Output State LED` följer `Output State`.
- **Via Home Assistant (`OPERATIONAL`):** utgången styrs från Home Assistant.
  `Short Press` växlar även `Output State` lokalt.
- **I `PAIRING`:** `Short Press` har ingen effekt. Utgången behåller aktuellt
  `Output State`.

Efter ett strömavbrott är utgången alltid `OFF` när PSMD får spänning igen
(`Power-loss Recovery`).

### 6.2 Läsa av mätvärden

Mätningen pågår kontinuerligt och påverkas inte av `Operation Mode`. Mätningen är
aktiv även när `Output State` är `OFF`. Värdena förmedlas via Matter till Home
Assistant endast i `OPERATIONAL`; i `LOCAL` och `PAIRING` visas inga mätvärden
där.

PSMD mäter den anslutna utrustningen och kan presentera följande värden:

- Spänning
- Ström
- Skenbar effekt
- Aktiv effekt
- Reaktiv effekt
- Fasförskjutning
- Frekvens
- Energiförbrukning

Spänning och ström är grundvärdena. Effektvärdena och fasförskjutningen
beskriver hur den anslutna utrustningen tar effekt, och energiförbrukningen är
effekt över tid. Energiförbrukningen fortsätter att räknas även när värdena inte
visas i Home Assistant.

Momentanvärdena beräknas även när ingen utrustning är ansluten och visas då som
0. Energiförbrukningen behåller sitt ackumulerade värde. Mätvärdena uppdateras
var 5:e sekund.

### 6.3 Automationer i Home Assistant

När PSMD är tillagd via Matter kan dess mätvärden och strömbrytare användas i
Home Assistants automationer och scener.

1. [Skapa automation]
2. [Välj PSMD som utlösare/villkor/åtgärd]
3. [Spara och testa]

## 7. Underhåll och skötsel

- [Rengöring]
- [Uppdatering av mjukvara/firmware]
- [Byte av förbrukningsdelar]
- [Förvaring]

## 8. Felsökning

Börja med att kontrollera `Operation Mode LED`: ett snabbt blink vid `Power-up`
visar att enheten får spänning, och därefter visar den vilket driftläge enheten
är i.

| Symtom | Möjlig orsak | Åtgärd |
|---|---|---|
| Ingen av lysdioderna lyser | PSMD saknar spänning | Kontrollera att PSMD sitter i vägguttaget |
| Går inte att lägga till i Home Assistant | Matter Server saknas eller PSMD är inte i `PAIRING` | Kontrollera Matter Server och upprepa parkopplingen |
| Inga mätvärden visas | Enheten är inte i `OPERATIONAL`, eller Home Assistant når den inte | Kontrollera driftläget och anslutningen till Home Assistant |
| [ ] | [ ] | [ ] |

## 9. Support och kontakt

- **Webb:** [ ]
- **E-post:** [ ]
- **Telefon:** [ ]
- **Öppettider:** [ ]

## 10. Ordlista

Fullständig terminologi med engelska mjukvarutermer finns i
[`glossary.md`](glossary.md). Termerna är engelska eftersom de används
oförändrade i programvara och dokumentation.

| Term (EN) | Svenska | Kort förklaring |
|---|---|---|
| `Operation Mode` | Driftläge | `LOCAL`, `PAIRING` eller `OPERATIONAL` |
| `LOCAL` | Lokalt läge | Standardläge, ingen anslutning till Home Assistant |
| `PAIRING` | Parkopplingsläge | Tillgänglig för Matter-`Commissioning` |
| `OPERATIONAL` | Anslutet läge | Ansluten till Home Assistant via Matter |
| `Output State` | Utgångstillstånd | `ON` eller `OFF` |
| `Short Press` / `Long Press` | Kort/långt tryck | ≤ 3 s respektive > 3 s |
| `Operation Mode LED` / `Output State LED` | Lägeslysdiod / utgångslysdiod | Visar driftläge respektive utgångstillstånd |
| `Commissioning` | Idrifttagning | Matter-processen för att lägga till enheten |
| `Pairing Timeout` | Parkopplingstimeout | Preliminärt 3 min, sedan återgång till `LOCAL` |
| `Slow Blink` | Långsamt blink | 0,5 s tänd / 0,5 s släckt |
| `Fast Blink` | Snabbt blink | 0,25 s tänd / 0,25 s släckt (2 Hz) |
| `Power-loss Recovery` | Återstart efter strömavbrott | `Output State` sätts alltid till `OFF` |
| `Measurement` | Mätning | Kontinuerlig; värdena visas i `OPERATIONAL`, även vid `OFF` |
| `Decommissioning` | Avregistrering | Lämna `PAIRING`/`Fabric`; metod ska undersökas |

## 11. Tekniska data

| Egenskap | Värde |
|---|---|
| [Mått/vikt] | [ ] |
| [Strömförsörjning] | [ ] |
| [Anslutningar] | [ ] |
| [Miljö/Drifttemperatur] | [ ] |
| Mätvärden: områden och enheter | [ ] |

> Mätområden, upplösning och enheter för mätvärdena är inte fastställda
> (`GAP-02`, `GAP-03` i [`decisions.md`](decisions.md)).
