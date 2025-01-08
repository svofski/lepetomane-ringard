cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(lepetomane-ringard C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(ringard
  main.cpp
  usb_descriptors.c
  reset.c
)

pico_enable_stdio_usb(ringard 1)
pico_enable_stdio_uart(ringard 0)

target_include_directories(ringard PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_compile_definitions(ringard PRIVATE 
    PICO_STDIO_USB_ENABLE_RESET_VIA_VENDOR_INTERFACE=1  # doesn't work
    PICO_STDIO_USB_ENABLE_RESET_VIA_BAUD_RATE=1		# could work but it's under the wrong define, so we do it on our own -- see reset.c
    PICO_STDIO_USB_RESET_MAGIC_BAUD_RATE=1200
    PICO_STDIO_USB_RESET_BOOTSEL_INTERFACE_DISABLE_MASK=0)

pico_add_extra_outputs(ringard)
target_link_libraries(ringard PRIVATE pico_stdlib pico_stdio_usb tinyusb_device tinyusb_board)
