# Dokumentation – PSMU

**Power Switch and Measurement Unit (PSMU)**

Index över dokumentationen. Innehållet skrivs på svenska; fil- och mappnamn är
engelska.

| Område | Dokument | Beskrivning | Status |
|---|---|---|---|
| Översikt | [`gaps.md`](gaps.md) | Kända luckor och nästa steg, prioriterade. | Utkast |
| Översikt | [`workflow.md`](workflow.md) | Branching och Conventional Commits. | Utkast |
| Användare | [`user/manual.md`](user/manual.md) | Användarhandbok för PSMU. | Utkast |
| Specification | [`specification/architecture.md`](specification/architecture.md) | Systemöversikt, komponenter, datamodell, Matter och byggmiljö. | Utkast |
| Specification | [`specification/specification.md`](specification/specification.md) | Driftlägen, knapp, LED, `Power-up` och mätbeteende. | Utkast |
| Project | [`project/plan.md`](project/plan.md) | Projektplan: mål, omfattning, roller, milstolpar och risker. | Utkast |
| Project | [`project/requirements.md`](project/requirements.md) | Krav med ID, acceptanskriterier och spårbarhet. | Utkast |
| Project | [`project/decisions.md`](project/decisions.md) | Beslut och öppna frågor. | Utkast |
| Reference | [`reference/glossary.md`](reference/glossary.md) | Terminologi och godkända begrepp. | Utkast |
| Reference | [`reference/external/matter-1.6.1/`](reference/external/matter-1.6.1/) | Matter 1.6.1-utdrag (read-only). | – |
| Reference | [`reference/external/esp-idf/INDEX.md`](reference/external/esp-idf/INDEX.md) | Var ESP-IDF 6.1 och ESP-Matter-dokumentationen finns. | Utkast |

## Ämneskarta

Sök på nyckelordet i stället för att läsa alla dokument. Varje dokument inleds
med en **Äger** / **Läs vid behov**-rad som räcker för att avgöra om filen
behövs.

| Fråga eller sökord | Läs |
|---|---|
| Driftläge, övergång, knapp, lysdiod, `Power-up`, mätning, lokal styrning | [`specification/specification.md`](specification/specification.md) §2–8 |
| Komponent, datamodell, Matter, Home Assistant, provisionering, byggmiljö, säkerhet | [`specification/architecture.md`](specification/architecture.md) §3–8 |
| Krav, acceptanskriterium, spårbarhet, teststrategi | [`project/requirements.md`](project/requirements.md) |
| Mål, omfattning, roll, milstolpe, risk, release | [`project/plan.md`](project/plan.md) |
| Beslut, öppen fråga, källa `K1`–`K6` | [`project/decisions.md`](project/decisions.md) |
| Term, benämning, `Operation Mode LED`, `Output State LED` | [`reference/glossary.md`](reference/glossary.md) |
| Lucka, nästa steg, `GAP-01`–`GAP-06` | [`gaps.md`](gaps.md) |
| Komma igång, användning, felsökning, teknisk data | [`user/manual.md`](user/manual.md) |
| Attribut-ID, kluster, device type, cluster revision | [`reference/external/matter-1.6.1/INDEX.md`](reference/external/matter-1.6.1/INDEX.md), därefter `grep` i `text/*.txt` |
| ESP-IDF, ESP-Matter, GPIO, NVS, OTA, `idf.py`, sökväg i `$IDF_PATH` | [`reference/external/esp-idf/INDEX.md`](reference/external/esp-idf/INDEX.md) |
| Branch, commit, pull request | [`workflow.md`](workflow.md) |

## Arbetssätt

- Branching och commit-format finns i [`workflow.md`](workflow.md).
- Nya fakta ska in i rätt dokument – inte i `README.md`.
- Öppna frågor hanteras i [`project/decisions.md`](project/decisions.md).
- Kända luckor och nästa steg finns i [`gaps.md`](gaps.md).
- Termer ska följa [`reference/glossary.md`](reference/glossary.md).
- Okända värden lämnas som `[ ]`; de får inte fyllas i med antaganden.
