cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/tools/CMakeLists.txt)

project(USB_TO_PS2_PICO C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(USB_TO_PS2_PICO)

add_subdirectory(PS2_device_lib)
add_subdirectory(atkPico)
add_subdirectory(USB_driver_wrapper)

# Example source
target_sources(USB_TO_PS2_PICO PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        )

# Make sure TinyUSB can find tusb_config.h
target_include_directories(USB_TO_PS2_PICO PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/usb
        ${CMAKE_CURRENT_LIST_DIR}/ps2)

# In addition to pico_stdlib required for common PicoSDK functionality, add dependency on tinyusb_host
# for TinyUSB device support and tinyusb_board for the additional board support library used by the example
target_link_libraries(USB_TO_PS2_PICO PUBLIC pico_stdlib tinyusb_host tinyusb_board hardware_pio atk_pico_lib PS2_DEVICE_lib USB_driver_wrapper)

pico_add_extra_outputs(USB_TO_PS2_PICO)

 
pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)
