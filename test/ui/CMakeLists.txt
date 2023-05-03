cmake_minimum_required(VERSION 3.5)
project(ui_tests LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(ADDITIONAL_CMAKE_CXX_FLAGS "-pthread")
SET(ADDITIONAL_CMAKE_EXE_LINKER_FLAGS "-lpthread")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CMAKE_CXX_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${ADDITIONAL_CMAKE_EXE_LINKER_FLAGS}")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test Widgets)

set(BASE_DIR
        ../../src/)

set(PROJECT_SOURCES
    TS_02/TS_02_04.cpp
    ${BASE_DIR}ui/resources/resources.qrc
    ${BASE_DIR}ui/mainwindow.cpp
    ${BASE_DIR}ui/aboutwindow.h ${BASE_DIR}ui/aboutwindow.cpp
    ${BASE_DIR}ui/disassembly/DisassemblyWidget.h ${BASE_DIR}ui/disassembly/DisassemblyWidget.cpp
    ${BASE_DIR}ui/Controller.cpp ${BASE_DIR}ui/Controller.h
    ${BASE_DIR}ui/peripherals/gpio/GPIOPortWidget.cpp ${BASE_DIR}ui/peripherals/gpio/GPIOPortWidget.h
    ${BASE_DIR}ui/peripherals/gpio/GPIOPinButton.cpp ${BASE_DIR}ui/peripherals/gpio/GPIOPinButton.h
    ${BASE_DIR}ui/peripherals/gpio/GPIOWidget.cpp ${BASE_DIR}ui/peripherals/gpio/GPIOWidget.h
    ${BASE_DIR}ui/peripherals/uart/UARTWidget.cpp ${BASE_DIR}ui/peripherals/uart/UARTWidget.h
    ${BASE_DIR}ui/peripherals/PeripheralsTabWidget.cpp ${BASE_DIR}ui/peripherals/PeripheralsTabWidget.h
    ${BASE_DIR}ui/disassembly/BreakpointAreaWidget.cpp ${BASE_DIR}ui/disassembly/BreakpointAreaWidget.h
    ${BASE_DIR}ui/RegistersWidget.cpp ${BASE_DIR}ui/RegistersWidget.h
    ${BASE_DIR}ui/memory/MemoryWidget.cpp ${BASE_DIR}ui/memory/MemoryWidget.h
    ${BASE_DIR}ui/memory/MemoryFormatter.cpp ${BASE_DIR}ui/memory/MemoryFormatter.h
    ${BASE_DIR}ui/peripherals/PeripheralWidget.h
    )

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

add_library(emulator ${BASE_DIR}emulator/EmulatorUnit.cpp)
add_library(modules ${BASE_DIR}modules/gpio.cpp ${BASE_DIR}modules/gpio.h ${BASE_DIR}modules/PeripheralDevice.h ${BASE_DIR}modules/ExampleDevice.cpp ${BASE_DIR}modules/ExampleDevice.h ${BASE_DIR}modules/uart.cpp ${BASE_DIR}modules/uart.h)
add_library(events ${BASE_DIR}utils/events/EventEmitter.cpp ${BASE_DIR}utils/events/EventEmitter.h ${BASE_DIR}utils/events/AbstractEvent.h ${BASE_DIR}utils/events/SimpleEvent.cpp ${BASE_DIR}utils/events/SimpleEvent.h ${BASE_DIR}utils/events/PeripheralDeviceEvent.h ${BASE_DIR}utils/events/gpio_events.cpp ${BASE_DIR}utils/events/gpio_events.h ${BASE_DIR}utils/events/BreakpointAreaWidgetEvents.cpp ${BASE_DIR}utils/events/BreakpointAreaWidgetEvents.h ${BASE_DIR}utils/events/uart_event.cpp ${BASE_DIR}utils/events/uart_event.h)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ui_tests
            ${PROJECT_SOURCES}
            )
else ()
    add_executable(ui_tests ${PROJECT_SOURCES})
endif ()
#Emulator target depends on riscv, riscv-disassembler, events, modules
target_link_libraries(emulator riscv)
target_link_libraries(emulator ${riscv-disassembler_SOURCE_DIR}/src/riscv-disas.c)
target_link_libraries(emulator events)
target_link_libraries(emulator modules)

#Project target depends on riscv, emulator, modules, events, json-lib
target_link_libraries(${PROJECT_NAME} PRIVATE riscv)
target_link_libraries(${PROJECT_NAME} PRIVATE emulator)
target_link_libraries(${PROJECT_NAME} PRIVATE modules)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE events)

target_link_libraries(ui_tests PRIVATE
        Qt${QT_VERSION_MAJOR}::Test
        Qt${QT_VERSION_MAJOR}::Widgets
        )

set_target_properties(ui_tests PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

install(TARGETS ui_tests
        BUNDLE DESTINATION .
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )
