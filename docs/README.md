# Dokumentation – PSMD

**Power Switch and Measurement Device (PSMD)**

Index över dokumentationen. Innehållet skrivs på svenska; fil- och mappnamn är
engelska.

| Område | Dokument | Beskrivning | Status |
|---|---|---|---|
| Användare | [`manual.md`](manual.md) | Användarhandbok för PSMD. | Utkast |
| Specification | [`specification.md`](specification.md) | Beteende, krav, arkitektur, datamodell, Matter och byggmiljö. | Utkast |
| Project | [`decisions.md`](decisions.md) | Beslut, öppna frågor och kända luckor. | Utkast |
| Källkod | [`../firmware/`](../firmware/) | ESP32-firmware (ESP-IDF + ESP-Matter). Byggs via `esp-idf`-MCP:n. | Ej påbörjad |
| Reference | [`reference/glossary.md`](reference/glossary.md) | Terminologi och godkända begrepp. | Utkast |
| Reference | [`reference/external/matter-1.6.1/`](reference/external/matter-1.6.1/) | Matter 1.6.1-utdrag (read-only). | – |
| Reference | [`reference/external/esp-idf/INDEX.md`](reference/external/esp-idf/INDEX.md) | Var ESP-IDF 6.1 och ESP-Matter-dokumentationen finns. | Utkast |

## Ämneskarta

Sök på nyckelordet i stället för att läsa alla dokument. Varje dokument inleds
med en **Äger** / **Läs vid behov**-rad som räcker för att avgöra om filen
behövs.

| Fråga eller sökord | Läs |
|---|---|
| Driftläge, övergång, knapp, lysdiod, `Power-up`, mätning, lokal styrning | [`specification.md`](specification.md) §2–8 |
| Krav, acceptanskriterium, spårbarhet, teststrategi | [`specification.md`](specification.md) §9 |
| Komponent, datamodell, Matter, Home Assistant, provisionering, byggmiljö, säkerhet | [`specification.md`](specification.md) §10–16 |
| Beslut, öppen fråga, källa `K1`–`K6` | [`decisions.md`](decisions.md) |
| Lucka, nästa steg, `GAP-01`–`GAP-05` | [`decisions.md`](decisions.md) |
| Term, benämning, `Operation Mode LED`, `Output State LED` | [`reference/glossary.md`](reference/glossary.md) |
| Komma igång, användning, felsökning, teknisk data | [`manual.md`](manual.md) |
| Attribut-ID, kluster, device type, cluster revision | [`reference/external/matter-1.6.1/INDEX.md`](reference/external/matter-1.6.1/INDEX.md), därefter `grep` i `text/*.txt` |
| ESP-IDF, ESP-Matter, GPIO, NVS, OTA, `idf.py`, sökväg i `$IDF_PATH` | [`reference/external/esp-idf/INDEX.md`](reference/external/esp-idf/INDEX.md) |
| Firmware, källkod, loggning, `psmd_`, `TAG` | [`../firmware/`](../firmware/), [`specification.md`](specification.md) §15 |
| Branch, commit, pull request | [Workflow](#workflow) |

## Arbetssätt

- Branching och commit-format finns i [Workflow](#workflow) nedan.
- Nya fakta ska in i rätt dokument – inte i `README.md`.
- Öppna frågor hanteras i [`decisions.md`](decisions.md).
- Kända luckor och nästa steg finns i [`decisions.md`](decisions.md).
- Termer ska följa [`reference/glossary.md`](reference/glossary.md).
- Okända värden lämnas som `[ ]`; de får inte fyllas i med antaganden.

## Workflow

Vi använder en enkel **GitHub Flow**-modell:

1. `main` är alltid den fungerande versionen.
2. Skapa en kortlivad branch från `main`.
3. Gör ändringarna och commit:a enligt nedan.
4. Öppna en pull request mot `main`.
5. Granska, merga och ta bort branchen.

Vi har inga långlivade `develop`-, `release`- eller `hotfix`-branches. Allt går
via `main`.

### Branching

#### Branchnamn

| Prefix | Används för | Exempel |
|---|---|---|
| `feat/` | Ny funktion eller nytt innehåll | `feat/matter-clusters` |
| `fix/` | Rätta fel | `fix/pairing-timeout` |
| `docs/` | Endast dokumentation | `docs/spec` |
| `chore/` | Underhåll, verktyg, konfiguration | `chore/gitignore` |

Använd korta, beskrivande namn med gemener och bindestreck.

### Conventional Commits

Commit-meddelanden följer [Conventional Commits](https://www.conventionalcommits.org/):

```
<type>(<scope>): <beskrivning>
```

`scope` är valfri. Beskrivningen skrivs i imperativ form, med gemener och utan
punkt på slutet.

#### Typer

| Typ | Används för |
|---|---|
| `feat` | Ny funktion eller nytt innehåll |
| `fix` | Rätta fel |
| `docs` | Endast dokumentation |
| `refactor` | Omskrivning utan beteendeändring |
| `test` | Tester |
| `chore` | Underhåll och verktyg |
| `build` | Byggsystem och beroenden |
| `ci` | CI-konfiguration |

#### Scopes

Används för att visa vilken del som ändras:

`manual`, `spec`, `decisions`, `reference`, `matter`, `build`.

#### Exempel

```
docs(spec): add requirements traceability
docs(decisions): update open questions
fix(spec): correct pairing timeout
feat(spec): describe energy persistence
chore: ignore Python cache files
```

Brytande ändring markeras med `!` efter typ/scope och förklaras i en footer:

```
feat(spec)!: change data model field names

BREAKING CHANGE: `ac_switch` har bytt namn till `output_state`.
```

### Pull requests

Pull requestens titel följer samma format som en commit. Vid squash-merge blir
PR-titeln commit-meddelandet på `main`.
