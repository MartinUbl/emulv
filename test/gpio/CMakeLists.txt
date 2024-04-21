add_executable(gpio_test test_configuration.cpp test_change_state.cpp)

# Needed for testing purposes
if (WIN32)
    # Copy the configuration file
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/testGpioConfig/windows/testGpioConfig.json DESTINATION ${CMAKE_BINARY_DIR}/bin/ui/)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/testGpioConfig/windows/testGpioConfig.json DESTINATION ${CMAKE_BINARY_DIR}/test/gpio/)

    # Attempt to copy QT CORE DLL
    if(EXISTS ${CMAKE_PREFIX_PATH}/bin/Qt6Core.dll)
#        add_custom_command(TARGET gpio_test POST_BUILD
#                COMMAND ${CMAKE_COMMAND} -E copy
#                ${CMAKE_PREFIX_PATH}/bin/Qt6Core.dll ${CMAKE_BINARY_DIR}/bin/ui/)
        add_custom_command(TARGET gpio_test POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_PREFIX_PATH}/bin/Qt6Core.dll ${CMAKE_BINARY_DIR}/test/gpio/)
    endif ()

    # Attempt to copy GPIO DLL
    add_custom_command(
            TARGET gpio_test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/bin/ui/libgpio.dll ${CMAKE_BINARY_DIR}/test/gpio/ || (exit 0)
            VERBATIM
    )
endif (WIN32)
if (UNIX)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/testGpioConfig/linux/testGpioConfig.json DESTINATION ${CMAKE_BINARY_DIR}/bin/ui/)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/testGpioConfig/linux/testGpioConfig.json DESTINATION ${CMAKE_BINARY_DIR}/test/gpio/)
endif (UNIX)

target_link_libraries(gpio_test PRIVATE peripheralsApi libLoader config)
AddTests(gpio_test)