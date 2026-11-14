# Documentation – PSMD

**Power Switch and Measurement Device (PSMD)**

Index of the documentation. Content is written in Swedish; file and folder names
are in English.

| Area | Document | Description | Status |
|---|---|---|---|
| Users | [`manual.md`](manual.md) | User manual for PSMD. | Draft |
| Specification | [`specification.md`](specification.md) | Behavior, requirements, architecture, data model, Matter and Home Assistant. | Draft |
| Project | [`decisions.md`](decisions.md) | Decisions, open questions and known gaps. | Draft |
| Source code | [`../firmware/`](../firmware/) | ESP32 firmware (ESP-IDF + ESP-Matter). Built via the `esp-idf` MCP. | Not started |
| Firmware | [`../firmware/README.md`](../firmware/README.md) | Build, flash, debug and logging rules for the firmware. | Draft |
| Reference | [`glossary.md`](glossary.md) | Terminology and approved terms. | Draft |
| Lookup | [`lookup/matter-1.6.1/`](lookup/matter-1.6.1/) | Matter 1.6.1 extracts (read-only). | – |
| Lookup | [`lookup/esp-idf.md`](lookup/esp-idf.md) | Where the ESP-IDF 6.1 and ESP-Matter documentation lives. | Draft |

## Topic map

Search by keyword instead of reading all documents. Each document starts with an
**Owner** / **Read when needed** line that is enough to decide whether the file
is needed.

| Question or keyword | Read |
|---|---|
| Operating mode, transition, button, LED, `Power-up`, measurement, local control | [`specification.md`](specification.md) §3–9 |
| Requirements, acceptance criterion, traceability, test strategy | [`specification.md`](specification.md) §10 |
| Component, data model, Matter, Home Assistant, provisioning, security | [`specification.md`](specification.md) §2, §11–15 |
| Decision, open question, source `K1`–`K6` | [`decisions.md`](decisions.md) |
| Gap, next step, `GAP-01`–`GAP-05` | [`decisions.md`](decisions.md) |
| Term, naming, `Operation Mode LED`, `Output State LED` | [`glossary.md`](glossary.md) |
| Getting started, usage, troubleshooting, technical data | [`manual.md`](manual.md) |
| Attribute ID, cluster, device type, cluster revision | [`lookup/matter-1.6.1/INDEX.md`](lookup/matter-1.6.1/INDEX.md), then `grep` in `text/*.txt` |
| ESP-IDF, ESP-Matter, GPIO, NVS, OTA, path in `$IDF_PATH` | [`lookup/esp-idf.md`](lookup/esp-idf.md) |
| Firmware, source code, logging, `psmd_`, `TAG` | [`../firmware/README.md`](../firmware/README.md), [`specification.md`](specification.md) §14 |
| Branch, commit, pull request | [Workflow](#workflow) |

## Working method

- Branching and commit format are described in [Workflow](#workflow) below.
- New facts go into the right document – not into `README.md`.
- Open questions are handled in [`decisions.md`](decisions.md).
- Known gaps and next steps are in [`decisions.md`](decisions.md).
- Terms must follow [`glossary.md`](glossary.md).
- Unknown values are left as `[ ]`; they must not be filled in with guesses.

## Workflow

We use a simple **GitHub Flow** model:

1. `main` is always the working version.
2. Create a short-lived branch from `main`.
3. Make the changes and commit according to the rules below.
4. Open a pull request against `main`.
5. Review, merge and delete the branch.

We have no long-lived `develop`, `release` or `hotfix` branches. Everything
goes through `main`.

### Branching

#### Branch names

| Prefix | Used for | Example |
|---|---|---|
| `feat/` | New feature or new content | `feat/matter-clusters` |
| `fix/` | Fix a bug | `fix/pairing-timeout` |
| `docs/` | Documentation only | `docs/spec` |
| `chore/` | Maintenance, tooling, configuration | `chore/gitignore` |

Use short, descriptive names in lowercase with hyphens.

### Conventional Commits

Commit messages follow [Conventional Commits](https://www.conventionalcommits.org/):

```
<type>(<scope>): <description>
```

| Type | Meaning |
|---|---|
| `feat` | New feature or content |
| `fix` | Bug fix |
| `docs` | Documentation only |
| `chore` | Maintenance, tooling |
| `refactor` | Refactoring without behavior change |

Example: `docs(spec): restructure overview and add functional units`.

### Before you finish

- Check that relative links and section references still resolve.
- Check that Markdown tables render.
- Keep diffs focused.

## Reading budget

Load only what the task needs. The internal docs total ~55 kB; the Matter text
extracts total ~5 MB (≈1.3 M tokens); the PDFs (33 MB) are binary and are
never worth reading. Reading the wrong file costs orders of magnitude more than
reading too few.

| Task | Always read | Read on demand | Do not read |
|---|---|---|---|
| Manual text | `docs/manual.md` | `specification.md` §3–9, `decisions.md` (unknown values), `grep` on the term in `glossary.md` | Matter extracts |
| Specification (all) | `docs/specification.md` | `decisions.md` | `manual.md`, Matter extracts |
| Architecture / Matter | `specification.md` §2, §11–15, `docs/lookup/matter-1.6.1/INDEX.md` | targeted `grep` in `text/*.txt` | whole `text/*.txt`, PDFs, `manual.md` |
| ESP-IDF / ESP-Matter API | `docs/lookup/esp-idf.md` | targeted `grep` in `$IDF_PATH/docs/en/` or `$ESP_MATTER_PATH/docs/en/` | whole doc trees, `$IDF_PATH` in a codegraph index (734 MB) |
| Firmware / C code | the `firmware/` files the task touches | `specification.md` §3–9 and §14 (behavior, build), the `esp-idf` skill | `manual.md`, Matter extracts |
| Requirements | `specification.md` §10 | `decisions.md` | Matter extracts, `manual.md` |
| Decision / open question | `decisions.md` (grep on the ID or topic) | the document the question concerns | everything else |
| Gap / next step | `decisions.md` (gaps) | `specification.md` §2, §11–15 (where the gap gets filled in) | `manual.md`, Matter extracts |
| Index / structure | `docs/README.md` | – | everything else |
| Terminology | only the matching `grep` hits in `glossary.md` | – | `glossary.md` in full |
| Branch / commit | `docs/README.md` §Workflow | – | everything else |
| Unfamiliar task | `docs/README.md` §Topic map, then the document it points to | – | Matter extracts, PDFs |