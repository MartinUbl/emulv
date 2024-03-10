##########################################################
# Initialization
##########################################################

# Set additional project information
set(COMPANY "EMULV")
set(COPYRIGHT "Copyright (c) 2024. All rights reserved.")
set(IDENTIFIER "com.emulv.emulator")

##########################################################
# Set up Qt
##########################################################

message(STATUS "Trying to find Qt6 Quick... (Required version >= 6.5)")
find_package(Qt6 QUIET 6.5 COMPONENTS Quick)
if (Qt6_FOUND)
    message(STATUS "Qt6 found successfully.")
else ()
    message(FATAL_ERROR "ERROR: Cannot find Qt6, make sure CMAKE_PREFIX_PATH, CMAKE_C_COMPILER, CMAKE_CXX_COMPILER are correctly set in the CMakePresets.json file.")
endif ()

qt_standard_project_setup(REQUIRES 6.5)
set(CMAKE_AUTORCC ON)

qt_add_executable(appBorderless_lib_test
        main.cpp
        Controller.cpp
        Controller.h
        EmulvApi.h
        resources.qrc
)

# The colors file must be set as singleton
set_source_files_properties(./qml/styles/Colors.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)
# This is needed so headers inside /qml/models/ will be discovered by compiler
include_directories(./qml/models)
# Specify all QML files and other sources
qt_add_qml_module(appBorderless_lib_test
        URI Borderless_lib_test
        VERSION 1.0
        QML_FILES ./qml/views/Main.qml
        QML_FILES ./qml/controls/WindowButton.qml
        QML_FILES ./qml/windows/BorderlessWindow.qml
        QML_FILES ./qml/styles/Colors.qml
        QML_FILES ./qml/windows/BorderlessPopup.qml
        QML_FILES ./qml/layouts/MainWindowLayout.qml
        QML_FILES ./qml/controls/ActionButton.qml
        QML_FILES ./qml/panels/CenterPlaceholderPanel.qml
        QML_FILES ./qml/controls/SidebarButton.qml
        QML_FILES ./qml/panels/FileExplorerPanel.qml
        QML_FILES ./qml/panels/LastOpenedFilesPanel.qml
        QML_FILES ./qml/controls/IconButton.qml
        QML_FILES ./qml/panels/SidePanelControlsPanel.qml
        QML_FILES ./qml/panels/MemoryAreaPanel.qml
        QML_FILES ./qml/panels/RegistersPanel.qml
        QML_FILES ./qml/panels/DebugInfoPanel.qml
        QML_FILES ./qml/panels/CodeAreaPanel.qml
        QML_FILES ./qml/views/About.qml
        SOURCES ./qml/models/filesystemmodel.cpp
        SOURCES ./qml/models/filesystemmodel.h
        SOURCES ./qml/models/RegistersTableModel.cpp
        SOURCES ./qml/models/RegistersTableModel.h
        SOURCES ./qml/models/MemoryTableModel.cpp
        SOURCES ./qml/models/MemoryTableModel.h
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appBorderless_lib_test PROPERTIES
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(appBorderless_lib_test
        PRIVATE Qt6::Quick
)

#Link downloaded library
target_link_libraries(appBorderless_lib_test PRIVATE QWindowKit::Quick)

##########################################################
# Dependencies of QtApp
##########################################################
target_link_libraries(appBorderless_lib_test PUBLIC riscv)
target_link_libraries(appBorderless_lib_test PUBLIC emulator)
target_link_libraries(appBorderless_lib_test PUBLIC peripherals)
target_link_libraries(appBorderless_lib_test PUBLIC events)
target_link_libraries(appBorderless_lib_test PUBLIC config)

##########################################################
# Other
##########################################################
include(Install)