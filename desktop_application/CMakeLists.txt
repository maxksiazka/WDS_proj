cmake_minimum_required(VERSION 3.16)
project(Qt_server_test_app VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Network)


add_executable(test_app main.cpp
)

target_include_directories(test_app PRIVATE inc)
target_sources(test_app PRIVATE 
    src/SensorLink.cpp
    inc/SensorLink.hpp
    src/MainWindow.cpp
    inc/MainWindow.hpp
)

target_link_libraries(test_app PRIVATE Qt6::Widgets Qt6::Core Qt6::Network)
