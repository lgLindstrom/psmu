# Tests – PSMD

**Power Switch and Measurement Device (PSMD)**

> Related: [`../specification.md`](../specification.md),
> [`../project.md`](../project.md), [`../decisions.md`](../decisions.md)

> **Owner:** the test plans that verify an increment on the development board.
> **Read when needed:** before testing a build on the board, and when recording
> the result of an increment.

The tests here are manual unless a plan says otherwise. The requirements and
their acceptance criteria are in [`../specification.md`](../specification.md)
§10; the increments and their exit criteria are in
[`../project.md`](../project.md) §4.

## Test plans

| Plan | Covers | State |
|---|---|---|
| [`manual-m2-m3.md`](manual-m2-m3.md) | M2: pairing, reconnect after a restart, decommissioning. M3: `Operation Mode`, button, LEDs, `Power-up`, the values Home Assistant shows. | Written, not run |

## Conventions

- A test case ID is `TC-NN` and unique within its plan.
- A plan records the firmware commit, the board's MAC and pass/fail per case.
- The result of an increment goes into [`../project.md`](../project.md) §4.
- A failure that needs a decision is recorded in
  [`../decisions.md`](../decisions.md); a missing requirement is a gap there.
