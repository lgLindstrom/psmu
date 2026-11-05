# Requirements – PSMU

**Power Switch and Measurement Unit (PSMU)**

> Status: Utkast
> Version: 0.1
> Ansvarig: [ ]
> Senast uppdaterad: 2026-11-04
> Relaterade: [`plan.md`](plan.md), [`decisions.md`](decisions.md),
> [`specification.md`](../specification/specification.md)

> **Äger:** krav med ID, acceptanskriterier och spårbarhet.
> **Läs vid behov:**
> [`specification/specification.md`](../specification/specification.md) för
> beteendet bakom kraven och [`decisions.md`](decisions.md) för öppna frågor.

## 1. Syfte

Det här dokumentet samlar produkt- och systemkrav med ID, acceptanskriterier och
spårbarhet till specifikation och test.

## 2. Funktionella krav

| ID | Krav | Källa | Acceptanskriterium | Status |
|---|---|---|---|---|
| REQ-F-001 | `Short Press` ska växla `Output State` i `LOCAL` och `OPERATIONAL`. | K1 | Utgången växlar vid tryck ≤ 3 s. | Bekräftad |
| REQ-F-002 | `Long Press` ska växla `Operation Mode` enligt specifikationen. | K1 | `LOCAL`/`OPERATIONAL` → `PAIRING`; `PAIRING` → `LOCAL`. | Bekräftad |
| REQ-F-003 | `Output State` ska alltid vara `OFF` vid `Power-up`. | K1 | Utgången är `OFF` efter varje spänningssättning. | Bekräftad |
| REQ-F-004 | Mätning ska endast ske i `OPERATIONAL`. | K1 | Inga mätvärden uppdateras i `LOCAL` eller `PAIRING`. | Bekräftad |
| REQ-F-005 | `Operation Mode LED` och `Output State LED` ska följa specificerade mönster. | K1 | Blink- och skenmönster överensstämmer med specifikationen. | Bekräftad |

## 3. Icke-funktionella krav

| ID | Krav | Källa | Acceptanskriterium | Status |
|---|---|---|---|---|
| REQ-NF-001 | [ ] | [ ] | [ ] | [ ] |

## 4. Spårbarhet

| Krav | Specifikation | Test | Status |
|---|---|---|---|
| REQ-F-001 | [`specification.md`](../specification/specification.md) §4 | [ ] | [ ] |
| REQ-F-002 | [`specification.md`](../specification/specification.md) §3–4 | [ ] | [ ] |
| REQ-F-003 | [`specification.md`](../specification/specification.md) §6 | [ ] | [ ] |
| REQ-F-004 | [`specification.md`](../specification/specification.md) §7 | [ ] | [ ] |
| REQ-F-005 | [`specification.md`](../specification/specification.md) §5 | [ ] | [ ] |

## 5. Teststrategi

- [ ] Enhetstest av mjukvarumoduler.
- [ ] Integrations- och HIL-test av knapp, LED, relä och mätning.
- [ ] Matter-`Commissioning` mot Home Assistant.
- [ ] Acceptanstest mot kraven ovan.

## 6. Öppna frågor

Se [`decisions.md`](decisions.md).
