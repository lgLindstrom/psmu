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

## 2. Open data

Nothing in this table is decided. The questions are in
[`decisions.md`](decisions.md) under **Hardware and electrical safety**.

| ID | What is missing | Manual |
|---|---|---|
| `TBD-13` | Type and rating of the input and output connectors. | `manual.md` §4.2, §5.1, §11 |
| `TBD-14` | Maximum load and electrical safety rating. | `manual.md` §3, §11 |
| `TBD-15` | Whether the device is for indoor use only. | `manual.md` §3 |
| `TBD-23` | Mounting of the plug-in unit. | – |
| `TBD-25` | Whether a protective earth (grounded outlet) is required. | `manual.md` §3 |
| `TBD-26` | Humidity, heat, storage and operating temperature. | `manual.md` §3, §7, §11 |
| `TBD-27` | Mains supply, dimensions and weight. | `manual.md` §11 |

## 3. Radio

The Wi-Fi band (2.4 or 5 GHz) follows the hardware implementation. The network
requirements the manual states for Home Assistant are therefore still open
(`GAP-06`).
