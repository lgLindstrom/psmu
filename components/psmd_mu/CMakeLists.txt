idf_component_register(SRCS "psmd_mu.c" "mu_sim.c" "mu_sim_model.c" "mu_i2c.c"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES esp_driver_i2c esp_timer psmd_board psmd_output)
