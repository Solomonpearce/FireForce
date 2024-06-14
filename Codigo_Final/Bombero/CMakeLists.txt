cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD "pico_w")
#SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
SET(GCC_EXE_LINKER_FLAGS    "-Wl,--print-memory-usage")

SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_EXE_LINKER_FLAGS}")

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(AccessSys)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(access
	access_sys.c Temperatura.c UltraS.c
)

target_include_directories(access PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(access pico_stdlib hardware_timer hardware_gpio hardware_pwm hardware_irq hardware_sync hardware_adc pico_time)

pico_enable_stdio_uart(access 0)
pico_enable_stdio_usb(access 1)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(access)