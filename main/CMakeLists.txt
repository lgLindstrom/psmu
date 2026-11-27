idf_component_register(SRCS "app_main.cpp"
                       REQUIRES psmd_button psmd_led psmd_measure psmd_mode psmd_mu
                                psmd_output)

target_compile_options(${COMPONENT_LIB} PRIVATE "-DCHIP_HAVE_CONFIG_H")
