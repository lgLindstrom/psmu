/* PSMD – board variants
 *
 * The pins and active levels of each board. The board is chosen at build time
 * with CONFIG_PSMD_BOARD (components/psmd_board/Kconfig).
 *
 * The development board is the ESP32-C6 bring-up board: a button, an Operation
 * Mode LED, and one Output State line that carries the Output State LED and
 * stands in for the relay until the real PU exists. The production board's pin
 * map is owned by GAP-02 and is not known yet.
 */

#pragma once

#include <sdkconfig.h>

#if CONFIG_PSMD_BOARD_DEV

/* A GPIO of -1 means the board has no such pin and the driver skips it. Each
 * *_ON_LEVEL is the pin level that means on; the button uses the level it has
 * while pressed. The LEDs are active high; the button is active low. */
#define PSMD_BUTTON_GPIO          3
#define PSMD_BUTTON_PRESSED_LEVEL 0
#define PSMD_MODE_LED_GPIO        2
#define PSMD_MODE_LED_ON_LEVEL    1

/* One Output State signal: the relay on/off and its Output State LED (LED2) are
 * on the same line, so there is no separate LED pin. */
#define PSMD_OUTPUT_GPIO          1
#define PSMD_OUTPUT_ON_LEVEL      1

#elif CONFIG_PSMD_BOARD_PRODUCTION

/* The pin map is open (GAP-02). Add the pins above when it is decided. */
#error "the PSMD production board pin map is open (GAP-02)"

#else
#error "no PSMD board variant selected"
#endif
