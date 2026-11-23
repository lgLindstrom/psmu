# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.22)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(psmd)

# ESP-Matter builds its application code as C++17 and expects CHIP_HAVE_CONFIG_H.
idf_build_set_property(CXX_COMPILE_OPTIONS "-std=gnu++17;-Os;-DCHIP_HAVE_CONFIG_H" APPEND)
idf_build_set_property(C_COMPILE_OPTIONS "-Os" APPEND)
# For RISCV chips, project_include.cmake sets -Wno-format, but does not clear
# various flags that depend on -Wformat
idf_build_set_property(COMPILE_OPTIONS "-Wno-format-nonliteral;-Wno-format-security" APPEND)
# Matter device identity (TBD-30), read by CHIP's device instance info
# provider. The IDs stay at the CSA test values (CONFIG_DEVICE_VENDOR_ID and
# CONFIG_DEVICE_PRODUCT_ID) because the test attestation carries them.
idf_build_set_property(COMPILE_OPTIONS
    "-DCHIP_DEVICE_CONFIG_DEVICE_VENDOR_NAME=\"PSMD\";-DCHIP_DEVICE_CONFIG_DEVICE_PRODUCT_NAME=\"PSMD\";-DCHIP_DEVICE_CONFIG_DEFAULT_DEVICE_HARDWARE_VERSION_STRING=\"1.0\";-DCHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION_STRING=\"0.1.0\""
    APPEND)
