# Include Qt basic functions
include(QtCommon)
# Set PROJECT_VERSION_PATCH and PROJECT_VERSION_TWEAK to 0 if not present, needed by add_project_meta
fix_project_version()
# Set additional project information
set(COMPANY "Example")
set(COPYRIGHT "Copyright (c) 2023. All rights reserved.")
set(IDENTIFIER "com.emulv.emulator")

##########################################################
set(SOURCE_FILES
        EmulvApi.h
        main.cpp
        mainwindow.cpp
        Controller.cpp
        aboutwindow.cpp
        registers/RegistersWidget.cpp
        peripherals/PeripheralsTabWidget.cpp
        peripherals/uart/UARTWidget.cpp
        peripherals/gpio/GPIOPinButton.cpp
        peripherals/gpio/GPIOPortWidget.cpp
        peripherals/gpio/GPIOWidget.cpp
        memory/MemoryFormatter.cpp
        memory/MemoryWidget.cpp
        disassembly/BreakpointAreaWidget.cpp
        disassembly/DisassemblyWidget.cpp
        )

add_project_meta(META_FILES_TO_INCLUDE)

set(RESOURCE_FILES resources/resources.qrc)

# Try to first use Qt6, if it's not available revert back to Qt5
set(QT_VERSION "Qt6")
find_package(${QT_VERSION} COMPONENTS Widgets)
if (NOT ${QT_VERSION}_FOUND)
    message(WARNING
            "\n=============================================\n"
            "Qt6 not found, attempting to use Qt5 instead.\n"
            "============================================="
            )
    set(QT_VERSION "Qt5")
    find_package(${QT_VERSION} COMPONENTS Widgets REQUIRED)
endif ()

# source_group("UI Files" FILES ${UI_FILES}

add_executable(QtApp ${OS_BUNDLE} # Expands to WIN32 or MACOS_BUNDLE depending on OS
        ${SOURCE_FILES} ${META_FILES_TO_INCLUDE} ${RESOURCE_FILES}
        )

target_precompile_headers(QtApp INTERFACE QtWidgets.h)

target_link_libraries(QtApp ${QT_VERSION}::Widgets)


SET(ADDITIONAL_CMAKE_CXX_FLAGS "-pthread")
SET(ADDITIONAL_CMAKE_EXE_LINKER_FLAGS "-lpthread")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CMAKE_CXX_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ADDITIONAL_CMAKE_EXE_LINKER_FLAGS}")

##########################################################
# Dependencies of QtApp
##########################################################
target_link_libraries(QtApp riscv)
target_link_libraries(QtApp emulator)
target_link_libraries(QtApp peripherals)
target_link_libraries(QtApp events)
target_link_libraries(QtApp config)

##########################################################
include(Install)