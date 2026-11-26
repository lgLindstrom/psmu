/* PSMD – UI bring-up: the two LEDs
 *
 * Blinks the Operation Mode LED fast and the Output State LED slow, so both
 * pins and both drivers are checked on the board. The assertions cover the
 * driver calls; the blink itself is checked by eye.
 */

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_log.h>
#include <unity.h>

#include "psmd_led.h"
#include "psmd_output.h"

static const char *TAG = "psmd_test_ui";

/* 10 half periods of Slow Blink (1 s) = 10 s. */
#define OUTPUT_TOGGLES 10

TEST_CASE("UI: operation mode LED fast, output state LED slow", "[psmd][ui]")
{
    TEST_ASSERT_EQUAL(ESP_OK, psmd_led_init());
    TEST_ASSERT_EQUAL(ESP_OK, psmd_output_init());
    TEST_ASSERT_EQUAL(ESP_OK, psmd_led_set_pattern(PSMD_LED_PATTERN_FAST_BLINK));

    for (int i = 0; i < OUTPUT_TOGGLES; i++) {
        TEST_ASSERT_EQUAL(ESP_OK, psmd_output_toggle());
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    TEST_ASSERT_EQUAL(ESP_OK, psmd_led_set_pattern(PSMD_LED_PATTERN_OFF));
    TEST_ASSERT_EQUAL(ESP_OK, psmd_output_set(false));

    ESP_LOGI(TAG, "check: mode LED fast, output state LED slow");
}

void app_main(void)
{
    UNITY_BEGIN();
    unity_run_all_tests();
    UNITY_END();
}
