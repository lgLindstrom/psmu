# Gaps and next steps – PSMD

**Power Switch and Measurement Device (PSMD)**

> Status: Utkast
> Version: 0.1
> Ansvarig: [ ]
> Senast uppdaterad: 2026-11-04
> Relaterade: [`README.md`](README.md),
> [`project/decisions.md`](project/decisions.md)

Det här dokumentet listar kända luckor i dokumentationen och vad som behöver tas
fram. Det är en arbetslista – inte fastställda krav. Beslut och öppna frågor
hanteras i [`project/decisions.md`](project/decisions.md).

> **Äger:** kända luckor och nästa steg (arbetslista, inte fastställda krav).
> **Läs vid behov:** [`project/decisions.md`](project/decisions.md) för besluten
> bakom luckorna och
> [`specification/architecture.md`](specification/architecture.md) för var de ska
> fyllas i.

## Prioriterade luckor

| ID | Prio | Område | Vad som saknas | Dokument |
|---|---|---|---|---|
| GAP-01 | Hög | Matter | Device type, endpoints, kluster, attribut, enheter och rapporteringsintervall. | [`specification/architecture.md`](specification/architecture.md) |
| GAP-02 | Hög | Hårdvara/mjukvara | Pin-map, aktiv nivå, debounce, ADC-kanal, mätområden och kalibrering. | [`specification/architecture.md`](specification/architecture.md) |
| GAP-03 | Hög | Datamodell | Enheter, skalning, intervall, `timestamp`, energi vid `Power-up`/overflow och vilka fält som är `Persisted State`. | [`specification/architecture.md`](specification/architecture.md) |
| GAP-04 | Medel | Verifiering | Icke-funktionella krav och konkreta acceptanstester kopplade till varje krav. | [`project/requirements.md`](project/requirements.md) |
| GAP-05 | Medel | Användarhandbok | Avsnitt 5.4 förutsätter ett lokalt UI med vyer och skärmbilder. Klargör om PSMD har något eget gränssnitt utöver knapp, LED och Home Assistant. | [`user/manual.md`](user/manual.md) |

## Detaljer

### GAP-01 – Matter-gränssnitt

`specification/architecture.md` säger att endpoints och kluster är TBD. Det
behövs:

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

Datamodellen i `specification/architecture.md` har fältnamn och typer, men
saknar:

- enheter och skalning för `vac`, `iac`, `pact` och `energy`,
- intervall och teckenkonvention för `preact` och `phase_delay`,
- vad `timestamp` representerar,
- hur `energy` hanteras vid `Power-up` och overflow,
- vilka fält som faktiskt är `Persisted State`.

### GAP-04 – Verifiering

`project/requirements.md` har fem funktionella krav. Det saknas:

- icke-funktionella krav inom prestanda, minne, robusthet och strömförbrukning,
- elsäkerhet och EMC, om de ska hanteras i detta repo,
- konkreta acceptanstester för varje krav.

### GAP-05 – Användarhandbokens UI-avsnitt

Avsnitt 5.4 i `user/manual.md` beskriver “huvudvyer, menyer och viktiga
funktioner – gärna med skärmbilder”. Det förutsätter ett lokalt gränssnitt som
inte framgår någon annanstans. PSMD verkar ha knapp, två LED och Home Assistant –
ingen egen skärm. Avsnittet bör tas bort eller förtydligas.

### GAP-06 – Repo-hygien

**Åtgärdad 2026-11-04.** `.gitignore` ignorerar nu `__pycache__/` och `*.pyc`,
befintlig cache är borttagen och repot har en första commit.

## Rekommenderad ordning

1. GAP-01 och GAP-03 – de låser upp krav, test och Home Assistant-integration.
2. GAP-02 – behövs innan firmware och mätning kan implementeras.
3. GAP-04 och GAP-05 – förbättrar spårbarhet och användardokumentation.
