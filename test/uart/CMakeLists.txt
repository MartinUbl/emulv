add_executable(uart_test test_configuration.cpp test_receiver.cpp test_transmitter.cpp test_base.h)

# Needed for testing purposes
if (WIN32)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/testUartConfig/windows/testUartConfig.json DESTINATION ${CMAKE_BINARY_DIR}/bin/ui/)
endif (WIN32)
if (UNIX)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/testUartConfig/linux/testUartConfig.json DESTINATION ${CMAKE_BINARY_DIR}/bin/ui/)
endif (UNIX)

target_link_libraries(uart_test PRIVATE peripheralsApi libLoader config)
AddTests(uart_test)