idf_component_register(SRCS "psmd_mode.c"
                       INCLUDE_DIRS "include"
                       REQUIRES psmd_button
                       PRIV_REQUIRES esp_timer psmd_led psmd_output)
