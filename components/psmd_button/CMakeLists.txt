idf_component_register(SRCS "psmd_button.c"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES esp_driver_gpio esp_timer psmd_board)
