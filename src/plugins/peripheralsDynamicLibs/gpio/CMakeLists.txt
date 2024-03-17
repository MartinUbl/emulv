cmake_minimum_required(VERSION 3.20.0)
project(gpio VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)

include_directories(../../peripheralsApi)

add_library(gpio SHARED
        ../../peripheralsApi/PeripheralDevice.h
        gpio.h
        gpio.cpp
)