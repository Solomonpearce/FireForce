cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD "pico_w")

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(FIREFORCE)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(FIREFORCE			
	proxim.c
)

pico_enable_stdio_usb(FIREFORCE 1)
pico_enable_stdio_uart(FIREFORCE 0)
# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(FIREFORCE pico_stdlib hardware_gpio hardware_timer)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(FIREFORCE)