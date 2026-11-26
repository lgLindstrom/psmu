# Test project for the PSMD components (plans and conventions: manual/README.md).
cmake_minimum_required(VERSION 3.22)

# The application's components live one level up.
set(EXTRA_COMPONENT_DIRS "../components")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
# Build main, the components under test and their dependencies only.
idf_build_set_property(MINIMAL_BUILD ON)
project(psmd_test)
