CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
add_library(pfd_instruments STATIC
    src/ArtificialHorizon.cpp
    inc/ArtificialHorizon.hpp
    src/AirspeedIndicator.cpp
    inc/AirspeedIndicator.hpp
    src/Altimeter.cpp
    inc/Altimeter.hpp
    src/HSI.cpp
    inc/HSI.hpp
    src/SystemStatusHeader.cpp
    inc/SystemStatusHeader.hpp
)
target_include_directories(pfd_instruments PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
target_link_libraries(pfd_instruments PUBLIC
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Eigen3::Eigen
)
