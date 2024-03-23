cmake_minimum_required(VERSION 3.20.0)
project(gpio VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Core)
qt_standard_project_setup(REQUIRES 6.5)

include_directories(../../peripheralsApi)

add_library(uart SHARED
        ../../peripheralsApi/PeripheralsApi.h
        uart.h
        uart.cpp
)

qt_add_resources(uart "uart"
        PREFIX "/uart"
        FILES
        resources/send_icon.svg
        resources/panel.qml
)

target_link_libraries(uart
        PRIVATE Qt6::Quick
)
