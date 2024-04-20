add_executable(gpio_test test_configuration.cpp test_change_state.cpp)

# Needed for testing purposes
if (WIN32)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/testGpioConfig/windows/testGpioConfig.json DESTINATION ${CMAKE_BINARY_DIR}/bin/ui/)
endif (WIN32)
if (UNIX)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/testGpioConfig/linux/testGpioConfig.json DESTINATION ${CMAKE_BINARY_DIR}/bin/ui/)
endif (UNIX)

target_link_libraries(gpio_test PRIVATE peripheralsApi libLoader config)
AddTests(gpio_test)