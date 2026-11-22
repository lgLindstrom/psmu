# Hardware – PSMD

**Power Switch and Measurement Device (PSMD)**

> Related: [`manual.md`](manual.md), [`specification.md`](specification.md),
> [`decisions.md`](decisions.md)

> **Owner:** mains connectors, electrical safety and ratings, the environment the
> device tolerates, and technical/mechanical data – the part of the product the
> firmware specification does not own.
> **Read when needed:** [`decisions.md`](decisions.md) for the IDs, and
> `manual.md` §3, §7 and §11 for what the user is told.

## 1. Scope

[`specification.md`](specification.md) specifies a firmware project: software
behavior, requirements, architecture, data model and Matter. Electrical safety,
mains connectors, environment and technical data are not specified there. They
are owned here and referred to from the specification.

[`manual.md`](manual.md) states the same facts to the user and keeps them as
`[...]` until they are decided.

Nothing here is decided. The open values and their IDs (`TBD-13`–`TBD-15`,
`TBD-23`, `TBD-25`–`TBD-27`) are in [`decisions.md`](decisions.md) under
**Hardware and electrical safety**, with the manual sections each one affects.

## 2. Radio

The Wi-Fi band (2.4 or 5 GHz) follows the hardware implementation. The network
requirements the manual states for Home Assistant are therefore still open
(`GAP-06`).
