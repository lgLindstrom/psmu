# Project instructions – PSMU documentation

This repository contains the Swedish user manual for **PSMU (Power Switch and
Measurement Unit)**, a Matter-enabled smart plug that switches and measures
connected equipment. It is documentation only: there is no firmware source,
build, or test suite here.

## Layout

- `docs/README.md` – documentation index.
- `docs/gaps.md` – known gaps and next steps.
- `docs/workflow.md` – branching model and Conventional Commits.
- `docs/user/manual.md` – the user manual (primary deliverable).
- `docs/specification/architecture.md` – system overview, components, data model,
  Matter integration, build environment.
- `docs/specification/specification.md` – operation modes, button, LEDs,
  `Power-up`, measurement behavior.
- `docs/project/plan.md` – project plan.
- `docs/project/requirements.md` – requirements and traceability.
- `docs/project/decisions.md` – decisions and open questions with source
  (`K1`–`K6`).
- `docs/reference/glossary.md` – terminology, English term → Swedish
  explanation, status.
- `docs/reference/external/matter-1.6.1/` – read-only Matter 1.6.1 extracts. Do
  not edit the PDFs, `text/`, or `extract_text.py`; search `text/*.txt` with grep
  rather than reading whole files (they are large).

## Reading budget

Load only what the task needs. The internal docs total ~50 kB; the Matter text
extracts total ~5 MB (≈1.3 M tokens). Reading the wrong file costs orders of
magnitude more than reading too few.

| Task | Always read | Read on demand | Do not read |
| --- | --- | --- | --- |
| Manual text | `docs/user/manual.md`, `docs/reference/glossary.md` | `specification/specification.md` (behaviour), `project/decisions.md` (unknown values) | Matter extracts, `plan.md`, `requirements.md`, `gaps.md` |
| Behaviour / specification | `docs/specification/specification.md` | `architecture.md`, `project/decisions.md` | `user/manual.md`, Matter extracts |
| Architecture / Matter | `docs/specification/architecture.md`, `docs/reference/external/matter-1.6.1/INDEX.md` | targeted `grep` in `text/*.txt` | whole `text/*.txt`, `user/manual.md` |
| Requirements / plan | `project/requirements.md` or `project/plan.md` | `project/decisions.md` | Matter extracts, `user/manual.md` |
| Terminology | only the matching `grep` hits in `reference/glossary.md` | – | `reference/glossary.md` in full |
| Branch / commit | `docs/workflow.md` | – | everything else |

Rules:

- Never read `docs/reference/external/matter-1.6.1/text/*.txt` in full. Start at
  `INDEX.md`, then `grep -n` with `-A`/`-B` context and `read` only those line
  ranges (`offset`/`limit`).
- Read sections, not files: `grep -n '^##' file.md` first, then `read` with
  `offset`/`limit`.
- Grep `project/decisions.md` and `reference/glossary.md` for the specific ID or
  term instead of loading the whole file.
- Do not read `plan.md`, `requirements.md` or `gaps.md` unless the task is
  explicitly about project management.
- Every document starts with an **Äger** / **Läs vid behov** line. The first
  paragraph is enough to decide whether the file is needed for the task.

## Writing conventions

- Write documentation in **Swedish**. Keep filenames, folder names, protocol and
  software terms in English as defined in `docs/reference/glossary.md`.
- Use **Conventional Commits** and the simple GitHub Flow described in
  `docs/workflow.md`: short-lived branches from `main`, pull request, merge,
  delete branch.
- `docs/reference/glossary.md` is the source of truth for terminology. Use the
  approved names `Operation Mode LED` and `Output State LED`, not `LED1`/`LED2`.
- Keep the manual's numbered section structure (`## 1.`, `### 4.4`, …) intact
  when editing; do not renumber sections without updating cross-references.
- Placeholders look like `[text]`. Do **not** invent values to fill them. If a
  fact is unknown or undecided, leave the placeholder and add or update an entry
  in `docs/project/decisions.md` with a source ID.
- When a question in `docs/project/decisions.md` is decided, move it to
  “Beslutade frågor” and apply the decision in the relevant document.

## Verification

- No build or tests. Before finishing, check that relative links and section
  references still resolve and that Markdown tables render.
- Keep diffs focused; this is prose, so avoid unrelated rewording.
