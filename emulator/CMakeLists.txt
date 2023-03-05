cmake_minimum_required(VERSION 3.14)
project(emulator LANGUAGES CXX)

#Example options: https://github.com/fwsGonzo/libriscv/blob/master/lib/CMakeLists.txt
option(MINIMAL     "Minimal build" OFF)
option(DEBUGGING   "Debugging build" OFF)
option(GCSECTIONS  "Garbage collect empty sections" ON)

include(FetchContent)
FetchContent_Declare(libriscv
        GIT_REPOSITORY https://github.com/fwsGonzo/libriscv
        GIT_TAG        master
        )

FetchContent_MakeAvailable(libriscv)

add_executable(emulator program.cpp)
target_link_libraries(emulator riscv)