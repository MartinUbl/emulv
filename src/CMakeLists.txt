cmake_minimum_required(VERSION 3.14)
project(emulv VERSION 1.0 DESCRIPTION "Emulator platformy RISC-V" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

#################################################################################
#Download libriscv from github (version v0.20 is selected by GIT_TAG)
#################################################################################

include(FetchContent)
FetchContent_Declare(
		libriscv
		GIT_REPOSITORY https://github.com/fwsGonzo/libriscv
		GIT_TAG 442925844a
)
FetchContent_MakeAvailable(libriscv)

#################################################################################
#Define libraries (define all libraries used by the project here)
#################################################################################

add_library(emulator ./emulator/EmulatorUnit.cpp)

#################################################################################
#Add files to the project target here
#################################################################################

#Add main file to the project target
add_executable(${PROJECT_NAME} main.cpp)

#################################################################################
#Add directories to target scopes (simplifies file includes)
#################################################################################

#target_include_directories(${PROJECT_NAME} PUBLIC ./emulator)
#target_include_directories(emulator PUBLIC .......)

#################################################################################
#Add library dependencies
#################################################################################

#Project target depends on riscv
target_link_libraries(${PROJECT_NAME} riscv)
#Emulator target depends on riscv
target_link_libraries(emulator riscv)
#Project target depends on emulator
target_link_libraries(${PROJECT_NAME} emulator)