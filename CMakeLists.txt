cmake_minimum_required(VERSION 3.20.0)
project(emulv VERSION 1.1.0 DESCRIPTION "Emulator platformy RISC-V" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/module")

# Download all external dependencies into the extern folder
set(FETCHCONTENT_BASE_DIR "${PROJECT_SOURCE_DIR}/extern/cmake_deps")

# Disable in-source builds
include(NoInSourceBuilds)

# Set up doxygen
# TO GENERATE DOCUMENTATION USE THE FOLLOWING COMMAND:
# cmake --build cmake/build-debug-linux -t doxygen
# OR BUILD THE "doxygen" TARGET USING YOUR IDE
include(Doxygen)
Doxygen(src docs)

# Set up the spdlog logging framework
# By using link_libraries(), the spdlog library is
# linked to ever target created after this call
include(GetSpdlog)
link_libraries(spdlog::spdlog)

add_subdirectory(src bin)
add_subdirectory(test)
