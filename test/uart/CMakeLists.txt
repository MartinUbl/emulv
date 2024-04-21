add_executable(uart_test test_configuration.cpp test_receiver.cpp test_transmitter.cpp test_base.h)

# Needed for testing purposes
if (WIN32)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/testUartConfig/windows/testUartConfig.json DESTINATION ${CMAKE_BINARY_DIR}/bin/ui/)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/testUartConfig/windows/testUartConfig.json DESTINATION ${CMAKE_BINARY_DIR}/test/uart/)
    # Attempt to copy QT CORE DLL
    if(EXISTS ${CMAKE_PREFIX_PATH}/bin/Qt6Core.dll)
#        add_custom_command(TARGET uart_test POST_BUILD
#                COMMAND ${CMAKE_COMMAND} -E copy
#                ${CMAKE_PREFIX_PATH}/bin/Qt6Core.dll ${CMAKE_BINARY_DIR}/bin/ui/)
        add_custom_command(TARGET uart_test POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_PREFIX_PATH}/bin/Qt6Core.dll ${CMAKE_BINARY_DIR}/test/uart/)
    endif ()

    # Attempt to copy UART DLL
    add_custom_command(
            TARGET uart_test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/bin/ui/libuart.dll ${CMAKE_BINARY_DIR}/test/uart/ || (exit 0)
            VERBATIM
    )

endif (WIN32)
if (UNIX)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/testUartConfig/linux/testUartConfig.json DESTINATION ${CMAKE_BINARY_DIR}/bin/ui/)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/testUartConfig/linux/testUartConfig.json DESTINATION ${CMAKE_BINARY_DIR}/test/uart/)
endif (UNIX)

target_link_libraries(uart_test PRIVATE peripheralsApi libLoader config)
AddTests(uart_test)