cmake_minimum_required(VERSION 3.5)
project(emulv VERSION 1.0 DESCRIPTION "Emulator platformy RISC-V" LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#################################################################################
#Setup QT GUI #1
#################################################################################
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp ui/resources/resources.qrc
        ui/mainwindow.cpp ui/mainwindow.ui
        ui/aboutwindow.h ui/aboutwindow.cpp ui/aboutwindow.ui
        ui/disassemblywidget.h ui/disassemblywidget.cpp
        ui/breakpointbutton.h ui/breakpointbutton.cpp
		ui/Controller.cpp ui/Controller.h ui/peripherals/gpio/GPIOPortWidget.cpp ui/peripherals/gpio/GPIOPortWidget.h ui/peripherals/gpio/GPIOPinButton.cpp ui/peripherals/gpio/GPIOPinButton.h ui/peripherals/gpio/GPIOWidget.cpp ui/peripherals/gpio/GPIOWidget.h)

#################################################################################
#Download libraries from git repositories
#################################################################################

#Download libriscv from github (version v0.20 is selected by GIT_TAG)
include(FetchContent)
FetchContent_Declare(
        libriscv
        GIT_REPOSITORY https://github.com/fwsGonzo/libriscv
        GIT_TAG 442925844a
)
FetchContent_MakeAvailable(libriscv)

#Download riscv-disassembler library
FetchContent_Declare(
		riscv-disassembler
		GIT_REPOSITORY https://github.com/michaeljclark/riscv-disassembler.git
)
FetchContent_MakeAvailable(riscv-disassembler)
include_directories(${riscv-disassembler_SOURCE_DIR}/src)

#Download the json parser library
FetchContent_Declare(
		json-lib
		GIT_REPOSITORY https://github.com/nlohmann/json.git
)
FetchContent_MakeAvailable(json-lib)

#################################################################################
#Define libraries (define all libraries used by the project here)
#################################################################################

add_library(emulator ./emulator/EmulatorUnit.cpp)
add_library(modules ./modules/PeripheralDevice.h)
add_library(events utils/events/EventEmitter.cpp utils/events/EventEmitter.h)

#################################################################################
#Set up project target executable file
#################################################################################

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
	qt_add_executable(emulv
			${PROJECT_SOURCES}
			)
else ()
	add_executable(emulv
			${PROJECT_SOURCES}
			)
endif ()

#################################################################################
#Add directories to target scopes (simplifies file includes)
#################################################################################

#target_include_directories(${PROJECT_NAME} PUBLIC ./emulator)
#target_include_directories(emulator PUBLIC .......)

#################################################################################
#Add library dependencies
#################################################################################

#Emulator target depends on riscv, riscv-disassembler, events
target_link_libraries(emulator riscv)
target_link_libraries(emulator ${riscv-disassembler_SOURCE_DIR}/src/riscv-disas.c)
target_link_libraries(emulator events)

#Project target depends on riscv, emulator, modules, events, json-lib
target_link_libraries(${PROJECT_NAME} PRIVATE riscv)
target_link_libraries(${PROJECT_NAME} PRIVATE emulator)
target_link_libraries(${PROJECT_NAME} PRIVATE modules)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE events)

#################################################################################
#Setup QT GUI #2
#################################################################################
target_link_libraries(emulv PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
set_target_properties(emulv PROPERTIES
		MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
		MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
		MACOSX_BUNDLE TRUE
		WIN32_EXECUTABLE TRUE
		)
install(TARGETS emulv
		BUNDLE DESTINATION .
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})