# This script should be able to deploy the QtApp. All of our own libraries should be
# statically linked (they have the .a file extension), so there should be no need to include
# them with the program itself.

# TODO - Add Linux install support
# https://stackoverflow.com/questions/2036873/static-library-install-under-linux-should-it-be-similar-to-dynamic-library-ins
# https://github.com/ttroy50/cmake-examples/blob/master/06-installer/deb/CMakeLists.txt
# https://www.qt.io/blog/deploying-to-linux-with-cmake

#########################################################################
# Start this script only on Release, this avoids exporting Qt dependencies every build
if (NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Build type is ${CMAKE_BUILD_TYPE}, skipping install script...")
    message(TRACE "Set your build type to release, in order to generate Qt dependencies and setup cmake install.")
    return()
endif ()
message(STATUS "Build type is ${CMAKE_BUILD_TYPE}, starting the install script...")

if (WIN32)
    #########################################################################
    # First we need to locate a Qt install.
    # Try to first use Qt6, if it's not available revert back to Qt5
    set(QT_VERSION "Qt6")
    message(STATUS "Trying to find the Qt package install location...")

    find_package(${QT_VERSION} COMPONENTS Core)
    if (NOT ${QT_VERSION}_FOUND)
        message(WARNING
                "\n=============================================\n"
                "Qt6 not found, attempting to use Qt5 instead.\n"
                "============================================="
                )
        set(QT_VERSION "Qt5")
        find_package(${QT_VERSION} COMPONENTS Core REQUIRED)
    endif ()

    #########################################################################
    # Secondly we need to find the windeployqt executable
    get_target_property(qmake_executable ${QT_VERSION}::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${qmake_executable}" DIRECTORY)
    # _qt_bin_dir should now be initialized
    message(STATUS "Binaries of ${QT_VERSION} were found at: ${_qt_bin_dir}")
    # Try to find the windeploy executable, it should be in the binary folder
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

    # Run qtdeploy after target QtApp is built (qtDeploy folder will be created in the build directory)
    add_custom_command(TARGET QtApp POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E
            env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
            --verbose 0
            --dir "${CMAKE_CURRENT_BINARY_DIR}/qtDeploy/"
            --no-compiler-runtime
            \"$<TARGET_FILE:QtApp>\"
            COMMENT "Deploying Qt..."
            )

    #########################################################################
    include(GNUInstallDirs)
    # Install the main target
    install(TARGETS QtApp
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            )
    # Install Qt dependencies
    install(
            DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/qtDeploy/"
            TYPE BIN
    )


    # CPack configuration
    set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE.txt")
    set(CPACK_PACKAGE_VENDOR "JD")
    set(CPACK_PACKAGE_CONTACT "jonasd@students.zcu.cz")
    set(CPACK_PACKAGE_DESCRIPTION "RISC-V emulator")

    include(CPack)
    # Run terminal, go to CMAKE BUILD directory, and execute the "cpack" command to build an installer for your OS.

else ()
    message(STATUS "Aborting install script... Your platform (${CMAKE_SYSTEM_NAME} is not supported yet.)")
endif ()


