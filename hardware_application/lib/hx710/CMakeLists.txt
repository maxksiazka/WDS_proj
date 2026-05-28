CMAKE_MINIMUM_REQUIRED(VERSION 3.13)
add_library(hx710 STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/hx710.c
)
pico_generate_pio_header(hx710 ${CMAKE_CURRENT_LIST_DIR}/hx710.pio)
target_include_directories(hx710 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(hx710 PUBLIC
    hardware_pio
    pico_time
)
