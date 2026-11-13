# ESP-IDF 6.1 och ESP-Matter – lokal referens

**Power Switch and Measurement Unit (PSMU)**

> Status: Utkast
> Version: 0.1
> Ansvarig: [ ]
> Senast uppdaterad: 2026-11-13
> Relaterade: [`matter-1.6.1/INDEX.md`](../matter-1.6.1/INDEX.md),
> [`architecture.md`](../../../specification/architecture.md) §5 och §7

> **Äger:** var Espressif-dokumentationen finns och hur den slås upp.
> **Läs vid behov:** [`matter-1.6.1/INDEX.md`](../matter-1.6.1/INDEX.md) för
> Matter-specifika kluster och attribut.

Firmware byggs med **ESP-IDF 6.1** (beslut `TBD-C8`) och Matter-delen med
**ESP-Matter** på grenen `release/v1.6.1`. Inget av Espressifs material kopieras
in i det här repot: dokumentationen läses från den installerade checkouten, som
alltid har samma version som kompilatorn.

## Var dokumentationen finns

| Källa | Sökväg | Används för |
|---|---|---|
| ESP-IDF, källkod | `$IDF_PATH/docs/en/**/*.rst` | Primär källa. Versionen är den vi bygger mot. |
| ESP-IDF, HTML | `https://docs.espressif.com/projects/esp-idf/en/v6.1/` | När ingen checkout finns på maskinen |
| ESP-IDF, genererad HTML | `idf.py docs` i ett projekt, sedan `build/docs/` | Offline, med rätt target |
| ESP-Matter | `$ESP_MATTER_PATH/docs/en/**/*.rst` | Matter-kluster, device types, provisioning |
| ESP-Matter, HTML | `https://docs.espressif.com/projects/esp-matter/en/latest/` | Översikt; avsnittet *Getting the Repositories* |

Maskinen har i dag `~/esp/esp-idf-v4.3` och `~/esp/esp-idf-v5.5`. **v6.1 saknas**
och måste installeras innan firmware kan byggas:

```sh
cd ~/esp
git clone -b v6.1 --depth 1 --recursive https://github.com/espressif/esp-idf.git esp-idf-v6.1
cd esp-idf-v6.1 && ./install.sh
```

`esp-idf`-MCP-servern (`.opencode/esp-idf-mcp.sh`) väljer automatiskt den högsta
`~/esp/esp-idf-v*`-katalogen, så v6.1 tas i bruk utan konfigurationsändring.

## Var i dokumentationen PSMU behöver leta

Avsnittsnamnen nedan är verifierade mot API Reference för v6.1. Sök i katalogen i
stället för att gissa filnamn; Sphinx flyttar filer mellan versioner.

| PSMU-del | ESP-IDF-område | Katalog under `$IDF_PATH/docs/en/` |
|---|---|---|
| `Output State`, `Button`, `Operation Mode LED`, `Output State LED` | Peripherals API → GPIO & RTC GPIO | `api-reference/peripherals/` |
| Blinkmönster (`Slow Blink`, `Fast Blink`) | Peripherals API → LED Control (LEDC) | `api-reference/peripherals/` |
| Tidsstyrning, `Pairing Timeout`, `Reconnect` | System API → ESP Timer, System Time | `api-reference/system/` |
| Mätning (`TBD-10`, `TBD-11`) | Peripherals API → ADC, eller extern mätkrets via I2C/SPI/UART | `api-reference/peripherals/` |
| Sparat `Output State`, `Power-up` | Storage API → Non-Volatile Storage Library | `api-reference/storage/` |
| Wi-Fi och provisionering (`TBD-05`) | Networking APIs → Wi-Fi; Provisioning API | `api-reference/network/`, `api-reference/provisioning/` |
| Matter-endpoints och kluster | ESP-Matter, inte ESP-IDF | `$ESP_MATTER_PATH/docs/en/` |
| OTA (`TBD-12`) | System API → ESP HTTPS OTA, Over The Air Updates | `api-reference/system/` |
| Loggning och felsökning | System API → Logging library, Console, Watchdogs | `api-reference/system/` |
| Partitionstabell och app-format | API Guides → Partition Tables; System API → App Image Format | `api-guides/`, `api-reference/system/` |

## Söka

```sh
# en typ eller ett fält i hela API-referensen
rg -n "adc_continuous_config_t" "$IDF_PATH/docs/en/api-reference"

# ett avsnitt, för att hoppa rätt
rg -n "^LED Control" "$IDF_PATH/docs/en/api-reference/peripherals"

# Kconfig-alternativ (CONFIG_*) finns i config-referensen
rg -n "CONFIG_ESP_TASK_WDT" "$IDF_PATH/docs/en/api-reference"
```

Samma regel som för Matter: **läs aldrig ett helt träd**. `docs/en` är 6,0 MB
reST och hela `$IDF_PATH` är 3,6 GB — inkludera aldrig den sökvägen i ett
codegraph-index eller i en läsning utan `rg` och radintervall.

Rubriken är inte normativ i sig: kontrollera signaturer i
`$IDF_PATH/components/*/include/*.h`, som är den kod som faktiskt kompileras.

## Versioner och kompatibilitet

| Komponent | Version | Källa |
|---|---|---|
| ESP-IDF | **6.1** | Beslut `TBD-C8` |
| ESP-Matter | `release/v1.6.1` (Matter 1.6.1) | [`matter-1.6.1/INDEX.md`](../matter-1.6.1/INDEX.md) |
| Matter-specifikation | 1.6.1 | [`matter-1.6.1/INDEX.md`](../matter-1.6.1/INDEX.md) |

**Känd avvikelse:** ESP-Matters README för både `release/v1.6.1` och `main`
rekommenderar ESP-IDF **v6.0.2**, inte 6.1. Vi ligger alltså ett steg före vad
Espressif testar mot. Frågan är registrerad som `TBD-21` i
[`decisions.md`](../../../project/decisions.md).

## Filer här

| Fil | Spårad | Innehåll |
|---|---|---|
| `INDEX.md` | ja | Den här kartan |

Inga PDF:er, inga textextrakt och ingen kopia av Espressifs dokumentation lagras
i repot. ESP-IDF:s dokumentation är CC-BY-SA 4.0 och får återges med
källhänvisning, men en kopia skulle ändå bli en inaktuell andrahandskälla.
