# Project plan – PSMD

**Power Switch and Measurement Device (PSMD)**

> Related: [`manual.md`](manual.md), [`specification.md`](specification.md),
> [`decisions.md`](decisions.md), [`glossary.md`](glossary.md)

> **Owner:** what is done when – phases, order, dependencies and exit criteria.
> **Read when needed:** [`decisions.md`](decisions.md) for the open questions
> and gaps (`TBD-NN`, `GAP-NN`), [`README.md`](README.md) for the document map.

There are no dates. The plan gives the order, what each phase needs before it
starts and what it must produce before the next one starts.

## 1. How work flows

[`manual.md`](manual.md) owns the behavior. A function that is added or removed
starts in the manual; the specification and the glossary are updated from it in
the same change. A statement in the manual that has no equivalent in the
specification is a gap in [`decisions.md`](decisions.md) – it is not filled in
with an invented behavior.

The manual is not finished: functions can still be added and removed. Until it
is, the specification is derived from what the manual states today, and every
change to the manual is a change to the specification.

## 2. State (2026-11-16)

| Document | State | Remains |
|---|---|---|
| [`manual.md`](manual.md) | Draft. Button, LEDs, `Operation Mode` and `Power-up` are settled. About 40 placeholders. | §1, §2.2–2.3, §3, §4.1, §4.4–4.5, §5.1 (connectors), §5.4, §6.2–6.3, §7, §8 (last row), §9, §11. |
| [`specification.md`](specification.md) | Behavior derived from the manual, with a `Manual` column in §3–10. Architecture, data model and Matter device types in place. | Implementation detail: `GAP-01`–`GAP-04`, `GAP-06`. |
| [`glossary.md`](glossary.md) | Terms in the manual and internal terms are separated. | Terms follow the manual as it grows. |
| [`decisions.md`](decisions.md) | Open questions and gaps with IDs and sources. | The IDs move to **Decided questions** as they are closed. |
| [`hardware.md`](hardware.md) | Stub. Scope is set; the values are open (`TBD-13`–`TBD-15`, `TBD-23`, `TBD-25`–`TBD-27`). | Filled in from K4 (`GAP-07`). |
| [`firmware/`](../firmware/) | Source and build rules exist; the build runs via the `esp-idf` MCP. | Bring-up on the target (`GAP-02`). |

## 3. Phases

| # | Phase | Goal | Exit criteria | Depends on |
|---|---|---|---|---|
| 1 | Steering | The manual owns the behavior and the other documents derive from it. | No document contradicts the order in §1; `scripts/check_docs.py` passes. | – (done 2026-11-16) |
| 2 | Manual v1 | The manual covers the first version of the product. | No placeholder that waits for an answer from the product owner remains in chapters 1–8 and 11. | Answers from K1 and K4 |
| 3 | Specification detail | The specification is implementable. | `GAP-01` and `GAP-03` closed: endpoints, clusters, attributes, units, reporting, and the data model per field. `GAP-06` filled in. | 1, and the manual sections involved |
| 4 | Hardware interface | The interface between hardware and software is fixed. | `GAP-02` closed: pin map, active level, debounce, ADC channel, ranges, calibration. `GAP-07` closed: connectors, ratings, environment and technical data. | Hardware from K4 |
| 5 | Verification | Every requirement has an acceptance test. | `GAP-04` closed: non-functional requirements and one concrete test per requirement. | 3 |
| 6 | Firmware | The firmware runs on the target. | Builds and flashes via the `esp-idf` MCP; button, LEDs, relay and measurement behave as the specification says. | 4 |
| 7 | Integration | PSMD works with Home Assistant. | Matter `Commissioning` succeeds and the measured values show up in Home Assistant. | 3, 6 |

## 4. Work items

| ID | What | Phase |
|---|---|---|
| `GAP-01` | Matter interface: device type, endpoints, clusters, attributes, units, reporting. | 3 |
| `GAP-03` | Semantics of the data model: units, scaling, interval, energy at `Power-up`, `Persisted State`. | 3 |
| `GAP-06` | Manual statements with no equivalent in the specification: network requirements, privacy in `LOCAL`. | 3 |
| `GAP-02` | Hardware/software interface: pin map, active level, debounce, ADC, calibration. | 4 |
| `GAP-07` | The hardware document: connectors, electrical safety, environment, technical data. | 4 |
| `GAP-04` | Verification: non-functional requirements and acceptance tests. | 5 |
| `GAP-05` | The UI section of the user manual assumes an interface PSMD does not have. | 2 |

Open questions (`TBD-NN`) are worked in the phase of the document they affect
and are defined in [`decisions.md`](decisions.md); they are not repeated here.

## 5. Adding or removing a function

1. Change [`manual.md`](manual.md) first: what the user sees and does.
2. If a value is unknown, leave the placeholder and add or update the entry in
   [`decisions.md`](decisions.md). Do not guess.
3. Derive the change in [`specification.md`](specification.md) §3–10 and fill in
   the `Manual` column, and in [`glossary.md`](glossary.md) when a term is
   involved.
4. Update requirements and acceptance criteria in `specification.md` §10 when
   the behavior changed.
5. If the manual states something the specification cannot hold yet, record a
   gap. It is filled in later, not invented now.
