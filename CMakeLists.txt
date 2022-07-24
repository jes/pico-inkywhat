cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(picoblink)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(hello
    src/hello.c src/inky.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(hello pico_stdlib pico_stdio_usb hardware_spi)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(hello)
