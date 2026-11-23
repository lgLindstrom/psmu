idf_component_register(SRCS "psmd_led.c"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES esp_driver_gpio esp_timer psmd_board)
