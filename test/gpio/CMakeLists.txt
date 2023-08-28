add_executable(gpio_test test_configuration.cpp test_input.cpp test_output.cpp test_base.h)
target_link_libraries(gpio_test PRIVATE events modules)
AddTests(gpio_test)