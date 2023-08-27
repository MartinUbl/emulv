cmake_minimum_required(VERSION 3.20.0)
project(emulv VERSION 1.1.0 DESCRIPTION "Emulator platformy RISC-V" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/module")

# Disable in-source builds
include(NoInSourceBuilds)

add_subdirectory(src bin)
#TODO add_subdirectory(test)

#TODO include install

