cmake_minimum_required(VERSION 3.20.0)
project(uart VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)

include_directories(../../peripheralsApi)

add_library(uart SHARED
        ../../peripheralsApi/PeripheralDevice.h
        uart.h
        uart.cpp
)