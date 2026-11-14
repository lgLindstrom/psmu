# ESP-IDF 6.1 och ESP-Matter – lokal referens

**Power Switch and Measurement Device (PSMD)**

> Status: Utkast
> Version: 0.1
> Ansvarig: [ ]
> Senast uppdaterad: 2026-11-13
> Relaterade: [`matter-1.6.1/INDEX.md`](../matter-1.6.1/INDEX.md),
> [`specification.md`](../../../specification/specification.md) §13 och §15

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

ESP-IDF 6.1 är installerad med **ESP-IDF Installation Manager (EIM)**, inte som
git-klon i hemkatalogen:

```text
$IDF_PATH              /home/lasse/.espressif/v6.1/esp-idf          (734 MB)
$IDF_PYTHON_ENV_PATH   /home/lasse/.espressif/tools/python/v6.1/venv
dokumentation          $IDF_PATH/docs/en                            (6,9 MB reST)
aktivering             ~/.espressif/tools/activate_idf_v6.1.sh
```

Samma maskin har även `~/esp/esp-idf-v4.3` och `~/esp/esp-idf-v5.5` från äldre
installationer. De används inte av PSMD.

## Aktivera miljön

| Behov | Kommando |
|---|---|
| Interaktivt skal | `eim shell v6.1` |
| Ett enskilt kommando | `eim run 'idf.py build' v6.1` |
| Se installerade versioner och vald version | `eim list` |
| Byta vald version | `eim select v6.1` |

`eim` registrerar varje installation i `~/.espressif/tools/eim_idf.json` och
genererar ett aktiveringsskript per version. Skriptet har ett tyst läge,
`bash ~/.espressif/tools/activate_idf_v6.1.sh -e`, som bara skriver
`NYCKEL=VÄRDE`-rader — det är den formen MCP-launchern använder.

`esp-idf`-MCP-servern (`.opencode/esp-idf-mcp.sh`) gör alltså detta automatiskt
vid start:

1. läser `eim_idf.json` och tar den installation som `eim select` pekar ut
   (`idfSelectedId`), annars den högsta versionen; `EIM_IDF_VERSION` överrider,
2. kör aktiveringsskriptets `-e` och exporterar alla variabler, inklusive
   `OPENOCD_SCRIPTS`, `ESP_ROM_ELF_DIR` och esp-clang-sökvägarna,
3. faller tillbaka på `~/esp/esp-idf-v*` (högsta versionen) om EIM saknas.

Verifierat: launchern rapporterar `ESP-IDF v6.1`, och med `eim_idf.json` borta
faller den tillbaka på `ESP-IDF v5.5`.

**Två saker som inte får göras:** sourca `activate_idf_v6.1.sh` utan `-e`, och
köra MCP-servern genom `eim run`/`eim shell`. Alla tre skriver en banner till
stdout — MCP-protokollet går över stdout och skulle brytas. `eim run` är av samma
skäl olämpligt som generell omslutning av agentkommandon.

**ESP-Matter är ännu inte hämtat** på maskinen: `$ESP_MATTER_PATH` är inte satt
och ingen `esp-matter`-checkout finns. Grenen `release/v1.6.1` hämtas enligt
avsnittet *Getting the Repositories* i ESP-Matters Programming Guide när
firmware-arbetet börjar.

## Var i dokumentationen PSMD behöver leta

Avsnittsnamnen nedan är verifierade mot API Reference för v6.1. Sök i katalogen i
stället för att gissa filnamn; Sphinx flyttar filer mellan versioner.

| PSMD-del | ESP-IDF-område | Katalog under `$IDF_PATH/docs/en/` |
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

Samma regel som för Matter: **läs aldrig ett helt träd**. `docs/en` är 6,9 MB
reST och hela `$IDF_PATH` är 734 MB — inkludera aldrig den sökvägen i ett
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
