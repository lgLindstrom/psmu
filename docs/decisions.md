# Decisions, open questions and gaps – PSMD

Samlade beslut, öppna frågor och kända luckor, med **varifrån de kommer**.
Dokumentet är arbetsmaterial: öppna frågor flyttas till **Beslutade frågor** när
ett beslut fattats, och berörda dokument uppdateras.

- **Status:** Utkast
- **Version:** 0.1
- **Senast uppdaterad:** 2026-11-14
- **Relaterade dokument:** [`manual.md`](manual.md),
  [`glossary.md`](glossary.md),
  [`specification.md`](specification.md)

> **Äger:** beslut, öppna frågor och kända luckor med källa (`K1`–`K6`).
> **Läs vid behov:** det dokument frågan eller luckan gäller – sök på ID eller
> ämne i stället för att läsa hela filen.

## Statusar

| Status | Betydelse |
|---|---|
| **Öppen** | Frågan är identifierad men inte påbörjad. |
| **Att undersöka** | Någon behöver ta reda på fakta, t.ex. i Matter-standarden. |
| **Väntar beslut** | Underlag finns eller behövs, men ett beslut måste fattas. |
| **Stängd** | Beslut fattat; flyttas till respektive dokument. |

## Källor (varifrån frågorna kommer)

| ID | Källa | Beskrivning |
|---|---|---|
| **K1** | Produktägaren | Svar och krav i projektdiskussionen (2026-11-04). |
| **K2** | Matter-standarden | Matter-specifikationen och dess krav på beteende. |
| **K3** | Home Assistant | Home Assistants Matter Server och integrationsflöde. |
| **K4** | Produktkrav / elsäkerhet | Krav på funktion, märkdata och säkerhet. |
| **K5** | Dokumentutkast | Frågor som uppstått när handboken skrivits. |
| **K6** | Terminologi | Termval som behöver bekräftas. |

## Öppna frågor

### Anslutning och Matter

| ID | Fråga | Källa | Påverkar | Status / nästa steg |
|---|---|---|---|---|
| TBD-01 | Är `Pairing Timeout` 3 minuter korrekt enligt Matter-standarden? | K1 (preliminärt), K2 | `PAIRING`, handbok 4.4/5.3 | Att undersöka |
| TBD-02 | Vilken `Commissioning Timeout` gäller enligt Matter-standarden? | K2 | `PAIRING`, felhantering | Att undersöka |
| TBD-03 | Hur ska enheten tas bort i Home Assistant när `PAIRING` lämnas med `Long Press` (`Decommissioning`)? | K1, K2, K3 | `PAIRING` → `LOCAL`, HA-data | Att undersöka |
| TBD-04 | Ska `Factory Reset` finnas, och hur triggas det i så fall? | K1 | Livscykel, `Fabric` | Väntar beslut |
| TBD-05 | Hur får enheten Wi-Fi-uppgifter vid `Commissioning`? Följ Matter/HA:s föreskrivna flöde. | K1, K2, K3 | `PAIRING`, `Wi-Fi Provisioning` | Att undersöka |
| TBD-06 | Vad händer vid `Power-up` i `OPERATIONAL` när Home Assistant/Matter inte nås? Ska enheten stanna i `OPERATIONAL` och försöka igen, eller falla tillbaka till `LOCAL`? | K1, K2 | `Boot Mode`, `Reconnect` | Att undersöka |
| TBD-07 | Vilket `Reconnect`-beteende gäller efter strömavbrott och nätverksbortfall? | K2, K3 | `OPERATIONAL`, mätning | Att undersökas tillsammans med TBD-06 |
| TBD-08 | Vilken lägsta Home Assistant-version och Matter Server-version krävs? | K3 | Handbok 2.3 | Att undersöka |
| TBD-09 | Ska enheten kunna läggas till i flera `Fabric` samtidigt? | K1, K2 | `Long Press` i `OPERATIONAL` | Väntar beslut |

### Mätning

| ID | Fråga | Källa | Påverkar | Status / nästa steg |
|---|---|---|---|---|
| TBD-10 | Vilka mätvärden exponeras: spänning, ström, aktiv effekt, energi, frekvens, effektfaktor? | K4 | Handbok 6.2, Matter-kluster | Väntar beslut |
| TBD-11 | Vilket `Measurement Interval` ska användas? | K4 | Mätning, HA-rapportering | Väntar beslut |
| TBD-12 | Ska energimätning ackumuleras över strömavbrott (`Persisted State`)? | K4 | Energimätning | Väntar beslut |
| TBD-22 | Vilken enhet och skalning ska `PowerFactor`-rapporteringen av `phase_delay` använda (percent100ths enligt Matter eller grader)? | K4 | Matter-rapportering, `specification.md` §13.3 | Väntar beslut |

### Hårdvara och elsäkerhet

| ID | Fråga | Källa | Påverkar | Status / nästa steg |
|---|---|---|---|---|
| TBD-13 | Vilken typ av anslutningsdon har ingång och utgång? | K1 (uppskjutet), K4 | Handbok 4.2/5.1 | Väntar beslut |
| TBD-14 | Max belastning (A/W) och elsäkerhetsklassning? | K4 | Handbok 3, tekniska data | Väntar beslut |
| TBD-15 | Ska enheten vara avsedd för inomhusbruk endast? | K4 | Handbok 3 | Väntar beslut |
| TBD-23 | Ska PSMD vara en plug-in-enhet eller fast monterad? Fast montering kräver Mounted On/Off Control (`0x010F`) enligt Matter 1.4+. | K4 | `specification.md` §13.1, `DeviceTypeList` | Väntar beslut |

### Terminologi

| ID | Fråga | Källa | Påverkar | Status / nästa steg |
|---|---|---|---|---|
| TBD-16 | Ska `Output State` användas, eller `Relay State`/`Power State`? | K6 | Glossary | Väntar beslut |
| TBD-17 | Ska `Persisted State` användas som term för lagrat tillstånd? | K6 | Glossary | Väntar beslut |
| TBD-18 | Ska `Relay` användas i användardokumentation, eller endast internt? | K6 | Glossary, handbok | Väntar beslut |

### Arkitektur och datamodell

| ID | Fråga | Källa | Påverkar | Status / nästa steg |
|---|---|---|---|---|
| TBD-19 | Vad står `PSMD`, `MDCB`, `MDB` och `DPS` för, och vilka ansvarsgränser har de? | K5 | `specification.md` §11, datamodell | Att undersöka |
| TBD-20 | Ska datamodellens fält, typer och skalning (t.ex. `int64` för mätvärden) bekräftas och dokumenteras per fält? | K5 | `specification.md` §12–13, Matter-rapportering | Att undersöka |
| TBD-21 | ESP-Matter `release/v1.6.1` rekommenderar ESP-IDF v6.0.2, men PSMD bygger på 6.1 (`TBD-C8`). Fungerar kombinationen, eller ska IDF-versionen sänkas? | K2 | Byggmiljö, `specification.md` §15 | Att undersöka |

## Beslutade frågor (stängda)

| ID | Fråga | Beslut | Källa |
|---|---|---|---|
| TBD-C1 | Ska mätning ske i `LOCAL`? | Nej, ingen mätning i `LOCAL` eller `PAIRING`. | K1 |
| TBD-C2 | Ska mätning vara aktiv när `Output State = OFF`? | Ja. | K1 |
| TBD-C3 | `Slow Blink`-frekvens? | 0,5 s tänd / 0,5 s släckt (1 Hz). | K1 |
| TBD-C4 | `Fast Blink`-frekvens? | 0,25 s tänd / 0,25 s släckt (2 Hz), 5 s vid `Power-up`. | K1 |
| TBD-C5 | Ska `Long Press` gå till `PAIRING` även från `OPERATIONAL`? | Ja. | K1 |
| TBD-C6 | Ska `Long Press` i `PAIRING` avbryta till `LOCAL`? | Ja. | K1 |
| TBD-C7 | `Output State` vid `Power-up`? | Alltid `OFF`. | K1 |
| TBD-C8 | Vilken ESP-IDF-version byggs firmware mot? | ESP-IDF 6.1, med ESP-Matter `release/v1.6.1` för Matter 1.6.1. Se [`../lookup/esp-idf.md`](../lookup/esp-idf.md). | K1 |
| TBD-C9 | Vilka Matter-enhetstyper används? | On/Off Plug-in Unit (`0x010A`) och Electrical Sensor (`0x0510`) på samma endpoint. Se [`specification.md`](specification.md) §13.1. Monteringssättet prövas i `TBD-23`. | K4 |
| TBD-C10 | Hur rapporteras `phase_delay`? | Som `PowerFactor` i Electrical Power Measurement. Enhet och skalning är öppen fråga (`TBD-22`). | K4 |

## Luckor och nästa steg

Kända luckor i dokumentationen och vad som behöver tas fram. Det är en
arbetslista – inte fastställda krav. Beslut och öppna frågor hanteras i
avsnitten ovan.

| ID | Prio | Område | Vad som saknas | Dokument |
|---|---|---|---|---|
| GAP-01 | Hög | Matter | Device type, endpoints, kluster, attribut, enheter och rapporteringsintervall. | [`specification.md`](specification.md) §13 |
| GAP-02 | Hög | Hårdvara/mjukvara | Pin-map, aktiv nivå, debounce, ADC-kanal, mätområden och kalibrering. | [`specification.md`](specification.md) §15 |
| GAP-03 | Hög | Datamodell | Enheter, skalning, intervall, `timestamp`, energi vid `Power-up`/overflow och vilka fält som är `Persisted State`. | [`specification.md`](specification.md) §12 |
| GAP-04 | Medel | Verifiering | Icke-funktionella krav och konkreta acceptanstester kopplade till varje krav. | [`specification.md`](specification.md) §9 |
| GAP-05 | Medel | Användarhandbok | Avsnitt 5.4 förutsätter ett lokalt UI med vyer och skärmbilder. Klargör om PSMD har något eget gränssnitt utöver knapp, LED och Home Assistant. | [`manual.md`](manual.md) |

### GAP-01 – Matter-gränssnitt

`specification.md` §13 säger att endpoints och kluster är TBD. Det behövs:

- device type och antal endpoints,
- vilka kluster som används, t.ex. On/Off och mätkluster,
- attribut, enheter och rapporteringsintervall,
- vilka värden som rapporteras när `Output State = OFF`.

Utan detta går firmware, Home Assistant-integration och acceptanstest inte att
specificera.

### GAP-02 – Hårdvara/mjukvara

Det saknas en beskrivning av gränssnittet mellan hårdvara och mjukvara:

- pin-map för knapp, `Operation Mode LED`, `Output State LED`, relä och ADC,
- aktiv nivå, pull-up/pull-down och debounce,
- mätkanal, kalibrering och mätområden.

### GAP-03 – Datamodellens semantik

Datamodellen i `specification.md` §12 har fältnamn och typer, men saknar:

- enheter och skalning för `vac`, `iac`, `pact` och `energy`,
- intervall och teckenkonvention för `preact` och `phase_delay`,
- vad `timestamp` representerar,
- hur `energy` hanteras vid `Power-up` och overflow,
- vilka fält som faktiskt är `Persisted State`.

### GAP-04 – Verifiering

`specification.md` §9 har fem funktionella krav. Det saknas:

- icke-funktionella krav inom prestanda, minne, robusthet och strömförbrukning,
- elsäkerhet och EMC, om de ska hanteras i detta repo,
- konkreta acceptanstester för varje krav.

### GAP-05 – Användarhandbokens UI-avsnitt

Avsnitt 5.4 i `manual.md` beskriver "huvudvyer, menyer och viktiga
funktioner – gärna med skärmbilder". Det förutsätter ett lokalt gränssnitt som
inte framgår någon annanstans. PSMD verkar ha knapp, två LED och Home Assistant –
ingen egen skärm. Avsnittet bör tas bort eller förtydligas.

### Rekommenderad ordning

1. GAP-01 och GAP-03 – de låser upp krav, test och Home Assistant-integration.
2. GAP-02 – behövs innan firmware och mätning kan implementeras.
3. GAP-04 och GAP-05 – förbättrar spårbarhet och användardokumentation.

## Så används dokumentet

1. Varje ny öppen fråga får ett ID (`TBD-NN`) och en källa.
2. När frågan är beslutad flyttas den till **Beslutade frågor** och motsvarande
   dokument (`manual.md` / `glossary.md` / specifikationen) uppdateras.
3. Källor som är externa (Matter, HA) ska helst kompletteras med länk eller
   version när de verifieras.
