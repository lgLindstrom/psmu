# Documentation – PSMD

**Power Switch and Measurement Device (PSMD)**

Index of the documentation. Every document is written in English except
[`manual.md`](manual.md), which is written in Swedish; file and folder names are
in English.

| Area | Document | Description |
|---|---|---|
| Users | [`manual.md`](manual.md) | User manual for PSMD (Swedish). |
| Specification | [`specification.md`](specification.md) | Behavior, requirements, architecture, data model, Matter and Home Assistant. |
| Decisions | [`decisions.md`](decisions.md) | Decisions, open questions and known gaps. |
| Plan | [`project.md`](project.md) | What is done when: phases, order, dependencies, exit criteria. |
| Tests | [`tests/`](tests/) | Manual test plans for the development board, e.g. M2 and M3. |
| Hardware | [`hardware.md`](hardware.md) | Connectors, electrical safety, environment, technical data. |
| Source code | [`../main/`](../main/), [`../components/`](../components/) | ESP32 firmware (ESP-IDF + ESP-Matter). Built via the `esp-idf` MCP. |
| Firmware | [`../README.md`](../README.md) | Build, flash, debug, logging and the ESP-IDF project standard for the firmware. |
| Reference | [`glossary.md`](glossary.md) | Terminology, approved terms and their Swedish wording in the manual. |
| Lookup | [`lookup/matter-1.6.1/`](lookup/matter-1.6.1/) | Matter 1.6.1 extracts (read-only). |
| Lookup | [`lookup/esp-idf.md`](lookup/esp-idf.md) | Where the ESP-IDF 6.1 and ESP-Matter documentation lives. |
| Tooling | [`../.opencode/agents/`](../.opencode/agents/) | Subagents. `matter-lookup` answers questions from the Matter extracts. |

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
| Gap, next step, `GAP-01`–`GAP-07` | [`decisions.md`](decisions.md) |
| Plan, phase, order, milestone, simulation, dev board, ESP32-C6, what comes next | [`project.md`](project.md) |
| Test plan, test case, `TC-NN`, manual test on the board, M2, M3 | [`tests/`](tests/) |
| Connector, electrical safety, environment, temperature, supply, dimension, weight | [`hardware.md`](hardware.md) |
| Term, naming, `Operation Mode LED`, `Output State LED` | [`glossary.md`](glossary.md) |
| Getting started, usage, troubleshooting, technical data | [`manual.md`](manual.md) |
| Attribute ID, cluster, device type, cluster revision | [`lookup/matter-1.6.1/INDEX.md`](lookup/matter-1.6.1/INDEX.md), then `grep` in `text/*.txt` |
| ESP-IDF, ESP-Matter, GPIO, NVS, OTA, path in `$IDF_PATH` | [`lookup/esp-idf.md`](lookup/esp-idf.md) |
| Firmware, source code, logging, `psmd_`, `TAG`, ESP-IDF layout, components, board variant, pin map, managed components, target | [`../README.md`](../README.md), [`specification.md`](specification.md) §14 |
| Branch, commit, pull request | [Workflow](#workflow) |

## Working method

- Branching and commit format are described in [Workflow](#workflow) below.
- New facts go into the right document – not into `README.md`.
- Open questions are handled in [`decisions.md`](decisions.md).
- Known gaps are in [`decisions.md`](decisions.md); the order they are worked
  in is in [`project.md`](project.md).
- Terms must follow [`glossary.md`](glossary.md).
- Unknown values are left as `[text]`; they must not be filled in with guesses.
- [`manual.md`](manual.md) owns the behavior. [`specification.md`](specification.md)
  and [`glossary.md`](glossary.md) are derived from it: the specification adds
  implementation, data model and Matter, the glossary adds the terms. Behavior
  the manual has and the specification lacks is a gap, recorded in
  [`decisions.md`](decisions.md).
- What is done when, in which order and with which exit criteria is in
  [`project.md`](project.md).
- Which document to read for which task is in `AGENTS.md` § Reading budget.
  That table is the single copy; do not duplicate it here.

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

- Run `python3 scripts/check_docs.py`. It checks relative links, section
  references, decision IDs and the language rule; CI runs it on every pull
  request.
- Check that Markdown tables render.
- Keep diffs focused.

## Reading budget

The reading budget (which document to read for which task, and what never to
read) is owned by `AGENTS.md` § Reading budget. It is deliberately kept in one
place: the agent needs it without a read, and two copies drifted apart.
