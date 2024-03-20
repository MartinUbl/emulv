cmake_minimum_required(VERSION 3.20.0)
project(gpio VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Core)
qt_standard_project_setup(REQUIRES 6.5)

include_directories(../../peripheralsApi)


add_library(gpio SHARED
        ../../peripheralsApi/PeripheralsApi.h
        GpioPort.cpp
        GpioPort.h
        gpio.h
        gpio.cpp
)



qt_add_resources(gpio "gpio"
        PREFIX "/gpio"
        FILES
        resources/gpio_input_high.svg
        resources/gpio_input_low.svg
        resources/gpio_output_high.svg
        resources/gpio_output_low.svg
        resources/panel.qml
)

target_link_libraries(gpio
        PRIVATE Qt6::Quick
)
