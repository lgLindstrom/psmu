# Project instructions – PSMD documentation

This repository contains the user manual for **PSMD (Power Switch and
Measurement Device)**, a Matter-enabled smart plug that switches and measures
connected equipment, and the firmware source for it: C/C++ for ESP32 built on
ESP-IDF 6.1 and ESP-Matter, at the repository root (`main/`, `components/`).

## Layout

The inventory of documents (what each one covers) is in `docs/README.md` § Topic
map and its table. It has a single owner there; do not repeat it here. What an
agent needs on top of it:

- `docs/manual.md` is the primary deliverable and the only Swedish document. It
  owns the behavior: it describes what the product does from the user's point of
  view. `docs/specification.md` and `docs/glossary.md` are derived from it – the
  specification adds implementation, requirements, architecture, data model and
  the Matter integration; the glossary adds the terms. Behavior the manual has
  and the specification lacks is a gap: record it in `docs/decisions.md`, do not
  invent it in the specification.
- `docs/project.md` owns what is done when: phases, order, dependencies and exit
  criteria. `docs/decisions.md` owns the IDs of open questions and gaps.
- `docs/hardware.md` owns what the firmware specification does not: connectors,
  electrical safety, environment and technical data. Those questions stay there
  and in `docs/decisions.md`; they are not written into the specification.
- `docs/lookup/matter-1.6.1/` is read-only. Do not edit the PDFs, `text/`, or
  `extract_text.py`; search `text/*.txt` with grep rather than reading whole
  files (they are large). The `matter-lookup` subagent does this for you.
- `docs/lookup/esp-idf.md`: where the installed ESP-IDF 6.1 and ESP-Matter
  documentation lives. It is never copied into this repository. Builds and
  flashing go through the `esp-idf` MCP, which activates the EIM installation
  that `eim select` points at; do not source `activate_idf_v*.sh` or wrap
  commands in `eim run` (both print to stdout).
- `.opencode/agents/` holds the subagents. `matter-lookup` is read-only and
  answers Matter questions with file and line references.

## Reading budget

Load only what the task needs. The internal docs total ~60 kB and are in
English; the Matter text extracts total ~5 MB (≈1.3 M tokens); the PDFs (33 MB)
are binary and are never worth reading. Reading the wrong file costs orders of
magnitude more than reading too few.

| Task | Always read | Read on demand | Do not read |
| --- | --- | --- | --- |
| Manual text | `docs/manual.md` | `specification.md` §3–9, `decisions.md` (unknown values), `grep` on the term in `glossary.md` | Matter extracts |
| Specification (all) | `docs/specification.md` | `decisions.md` | `manual.md`, Matter extracts |
| Architecture / Matter | `specification.md` §2, §11–15, `docs/lookup/matter-1.6.1/INDEX.md` | targeted `grep` in `text/*.txt` | whole `text/*.txt`, PDFs, `manual.md` |
| ESP-IDF / ESP-Matter API | `docs/lookup/esp-idf.md` | targeted `grep` in `$IDF_PATH/docs/en/` or in `managed_components/espressif__esp_matter/` | whole doc trees, `$IDF_PATH` in a codegraph index (734 MB) |
| Firmware / C code | the `main/`, `components/` and `test/` files the task touches | `specification.md` §3–9 and §14 (behavior, build), the ESP-IDF project standard in `README.md`, the `esp-idf` skill | `manual.md`, Matter extracts |
| Requirements | `specification.md` §10 | `decisions.md` | Matter extracts, `manual.md` |
| Decision / open question | `decisions.md` (grep on the ID or topic) | the document the question concerns | everything else |
| Gap (what is missing) | `decisions.md` (gaps) | `specification.md` §2, §11–15 (where the gap gets filled in) | `manual.md`, Matter extracts |
| Plan, order, next phase | `docs/project.md` | `decisions.md` for the ID of a work item | `manual.md`, Matter extracts |
| Connector, electrical safety, environment, technical data | `docs/hardware.md` | `decisions.md` for the IDs, `manual.md` §3, §7, §11 | `specification.md`, Matter extracts |
| Index / structure | `docs/README.md` | – | everything else |
| Terminology | only the matching `grep` hits in `glossary.md` | – | `glossary.md` in full |
| Branch / commit | `docs/README.md` §Workflow | – | everything else |
| Unfamiliar task | `docs/README.md` §Topic map, then the document it points to | – | Matter extracts, PDFs |

Rules:

- Never read `docs/lookup/matter-1.6.1/text/*.txt` in full. Start at
  `INDEX.md`, then `grep -n` with `-A`/`-B` context and `read` only those line
  ranges (`offset`/`limit`).
- Never read the Matter PDFs; the text extracts exist and the binaries are not
  readable anyway.
- Read sections, not files: `grep -n '^##' file.md` first, then `read` with
  `offset`/`limit`.
- Grep `decisions.md` and `glossary.md` for the specific ID or term instead of
  loading the whole file.
- Every document except `docs/README.md` and `matter-1.6.1/INDEX.md` has an
  **Owner** / **Read when needed** block (around lines 9–17). That block is
  enough to decide whether the file is needed.
- `docs/README.md` §Topic map is the human view (keyword → document); where it
  disagrees with the table above, the table above wins.

## Writing conventions

- Write documentation in **English**, except `docs/manual.md`, which is written
  in Swedish. The Swedish column in `docs/glossary.md` is the wording the manual
  is written from; it is the one place where Swedish belongs outside the manual.
- Software terms are always English, so that a term can be used unchanged as an
  identifier in C, in the specification and in the manual's glossary link.
  `docs/glossary.md` is the source of truth: use the approved names
  `Operation Mode LED` and `Output State LED`, not `LED1`/`LED2`.
- Use **Conventional Commits** and the simple GitHub Flow described in
  `docs/README.md` §Workflow: short-lived branches from `main`, pull request,
  merge, delete branch.
- Firmware code logs with the ESP-IDF macros and sets a per-module `TAG` that
  always starts with `psmd_` (`README.md` §Logging), and documents its API in
  the headers rather than in the `.c` files (`README.md` §Code documentation).
- Keep the manual's numbered section structure (`## 1.`, `### 4.4`, …) intact
  when editing; do not renumber sections without updating cross-references.
- Placeholders look like `[text]`. Do **not** invent values to fill them. If a
  fact is unknown or undecided, leave the placeholder and add or update an entry
  in `docs/decisions.md` with a source ID.
- When a question in `docs/decisions.md` is decided, move it to “Decided
  questions” and apply the decision in the relevant document.

## Verification

- Firmware: build via the `esp-idf` MCP (`build_project`) before claiming
  completion; flash only when asked.
- Documentation: `python3 scripts/check_docs.py` must pass. It checks relative
  links, section references, decision IDs and the language rule; CI runs it on
  every pull request.
- Keep diffs focused; this is prose, so avoid unrelated rewording.
