add_executable(uart_test test_configuration.cpp test_receiver.cpp test_transmitter.cpp test_base.h)
target_link_libraries(uart_test PRIVATE events modules)
AddTests(uart_test)