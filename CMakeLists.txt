cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_PATH "$ENV{HOME}/pico-sdk" CACHE PATH "Path to pico-sdk")
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(THXCard C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(THXCard THXCard.cpp)
add_executable(THXCardDense THXCardDense.cpp)

# Use the authoritative, current ComputerCard header rather than a release copy.
set(COMPUTERCARD_DIR "$ENV{HOME}/coding/GitHub/Workshop_Computer/Demonstrations+HelloWorlds/PicoSDK/ComputerCard"
    CACHE PATH "Directory containing the latest ComputerCard.h")
target_include_directories(THXCard PRIVATE ${COMPUTERCARD_DIR})
target_include_directories(THXCardDense PRIVATE ${COMPUTERCARD_DIR})

target_compile_options(THXCard PRIVATE -O3 -Wdouble-promotion -Wfloat-conversion -Wall -Wextra)
target_compile_options(THXCardDense PRIVATE -O3 -Wdouble-promotion -Wfloat-conversion -Wall -Wextra)
target_compile_definitions(THXCard PRIVATE PICO_XOSC_STARTUP_DELAY_MULTIPLIER=64)
target_compile_definitions(THXCardDense PRIVATE PICO_XOSC_STARTUP_DELAY_MULTIPLIER=64)
target_link_options(THXCard PRIVATE -Wl,--print-memory-usage)
target_link_options(THXCardDense PRIVATE -Wl,--print-memory-usage)
target_link_libraries(THXCard pico_stdlib pico_unique_id hardware_adc hardware_clocks
    hardware_dma hardware_i2c hardware_interp hardware_pwm hardware_spi)
target_link_libraries(THXCardDense pico_stdlib pico_unique_id hardware_adc hardware_clocks
    hardware_dma hardware_i2c hardware_interp hardware_pwm hardware_spi)

pico_enable_stdio_usb(THXCard 0)
pico_enable_stdio_uart(THXCard 0)
pico_set_binary_type(THXCard copy_to_ram)
pico_add_extra_outputs(THXCard)
pico_enable_stdio_usb(THXCardDense 0)
pico_enable_stdio_uart(THXCardDense 0)
pico_set_binary_type(THXCardDense copy_to_ram)
pico_add_extra_outputs(THXCardDense)
