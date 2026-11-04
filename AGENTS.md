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
