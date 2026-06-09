CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
add_library(mfd_instruments STATIC
    src/NavigationScreen.cpp
    inc/NavigationScreen.hpp
    src/TelemetryScreen.cpp
    inc/TelemetryScreen.hpp
)
target_include_directories(mfd_instruments PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
target_link_libraries(mfd_instruments PUBLIC
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)
