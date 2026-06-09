CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
add_library(dcp_instruments STATIC
    src/QNH_knob.cpp
    inc/QNH_knob.hpp
    src/CRS_knob.cpp
    inc/CRS_knob.hpp
)
target_include_directories(dcp_instruments PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
target_link_libraries(dcp_instruments PUBLIC
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)
