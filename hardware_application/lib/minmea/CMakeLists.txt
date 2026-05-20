CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

add_library(minmea STATIC 
    minmea.c
    minmea.h
)
target_link_libraries(minmea
    pico_stdlib
)
target_include_directories(minmea PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_definitions(minmea PRIVATE
    timegm=mktime
)
