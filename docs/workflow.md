# Workflow – PSMU

**Power Switch and Measurement Unit (PSMU)**

> Status: Utkast
> Version: 0.1
> Ansvarig: [ ]
> Senast uppdaterad: 2026-11-04
> Relaterade: [`README.md`](README.md)

Det här dokumentet beskriver hur vi arbetar med branches och commits.

## Branching

Vi använder en enkel **GitHub Flow**-modell:

1. `main` är alltid den fungerande versionen.
2. Skapa en kortlivad branch från `main`.
3. Gör ändringarna och commit:a enligt nedan.
4. Öppna en pull request mot `main`.
5. Granska, merga och ta bort branchen.

Vi har inga långlivade `develop`-, `release`- eller `hotfix`-branches. Allt går
via `main`.

### Branchnamn

| Prefix | Används för | Exempel |
|---|---|---|
| `feat/` | Ny funktion eller nytt innehåll | `feat/matter-clusters` |
| `fix/` | Rätta fel | `fix/pairing-timeout` |
| `docs/` | Endast dokumentation | `docs/gaps` |
| `chore/` | Underhåll, verktyg, konfiguration | `chore/gitignore` |

Använd korta, beskrivande namn med gemener och bindestreck.

## Conventional Commits

Commit-meddelanden följer [Conventional Commits](https://www.conventionalcommits.org/):

```
<type>(<scope>): <beskrivning>
```

`scope` är valfri. Beskrivningen skrivs i imperativ form, med gemener och utan
punkt på slutet.

### Typer

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

### Scopes

Används för att visa vilken del som ändras:

`user`, `spec`, `project`, `reference`, `matter`, `build`.

### Exempel

```
docs(spec): add Matter cluster overview
docs(project): update requirements traceability
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
