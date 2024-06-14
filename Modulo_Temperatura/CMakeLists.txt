cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD "pico")

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(ADC_DMA)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(adc_dma
	adc_dma.c
)

pico_enable_stdio_usb(adc_dma 1)
# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(adc_dma pico_stdlib hardware_dma hardware_adc hardware_pwm hardware_sync hardware_resets)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(adc_dma)