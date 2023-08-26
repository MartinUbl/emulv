cmake_minimum_required(VERSION 3.5)
project(emulv VERSION 1.1.0 DESCRIPTION "Emulator platformy RISC-V" LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/module")

# Disable in-source builds
include(NoInSourceBuilds)

add_subdirectory(src bin)
