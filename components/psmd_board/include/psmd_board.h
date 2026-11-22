/* PSMD – board variants
 *
 * The pins and active levels of each board. The board is chosen at build time
 * with CONFIG_PSMD_BOARD (main/Kconfig.projbuild).
 *
 * The development board is the ESP32-C6 bring-up board: a button, one
 * Operation Mode LED (no Output State LED) and a free GPIO that stands in for
 * the `Output` until the real `PU` exists. The production board's pin map is
 * owned by GAP-02 and is not known yet.
 */

#pragma once

#include <sdkconfig.h>

#if CONFIG_PSMD_BOARD_DEV

/* Button and Operation Mode LED are active low. */
#define PSMD_BUTTON_GPIO          3
#define PSMD_BUTTON_PRESSED_LEVEL 0
#define PSMD_MODE_LED_GPIO        2
#define PSMD_MODE_LED_ON_LEVEL    0

/* The dev board has no Output State LED. */
#define PSMD_OUTPUT_LED_GPIO      (-1)
#define PSMD_OUTPUT_LED_ON_LEVEL  1

/* Stands in for the relay (active high) until the real PU exists. */
#define PSMD_OUTPUT_GPIO          10
#define PSMD_OUTPUT_ON_LEVEL      1

#elif CONFIG_PSMD_BOARD_PRODUCTION

/* The pin map is open (GAP-02). Add the pins above when it is decided. */
#error "the PSMD production board pin map is open (GAP-02)"

#else
#error "no PSMD board variant selected"
#endif
