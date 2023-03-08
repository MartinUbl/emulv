cmake_minimum_required(VERSION 3.14)
project(emulv LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
		libriscv
		
		GIT_REPOSITORY https://github.com/fwsGonzo/libriscv
		GIT_TAG        master
)

FetchContent_MakeAvailable(libriscv)

add_executable(
		emulv

		main.cpp
)
target_link_libraries(emulv riscv)
