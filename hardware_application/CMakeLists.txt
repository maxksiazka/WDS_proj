cmake_minimum_required(VERSION 3.13)
add_compile_options(-Wall -Wextra)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PICO_BOARD pico2_w)
include(pico_sdk_import.cmake)

project(sumo C CXX ASM)

pico_sdk_init()

add_subdirectory(lib)

add_executable(plane main.c)

target_sources(plane PRIVATE
    src/udp_discovery.c
    src/tcp_api_implementation.c
    src/tcp_typedefs.c
    src/tcp_handling.c
    src/imu.c
)

target_include_directories(plane PRIVATE 
    inc
)

target_link_libraries(plane PRIVATE
    pico_cyw43_arch_lwip_poll
    pico_stdlib
    libGY87
    hardware_i2c
)
target_compile_definitions(plane PRIVATE
    WIFI_SSID="$ENV{WIFI_SSID}"
    WIFI_PASSWORD="$ENV{WIFI_PASSWORD}"
)
pico_enable_stdio_usb(plane 1)
pico_enable_stdio_uart(plane 1)

pico_add_extra_outputs(plane)

