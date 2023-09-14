add_library(peripherals gpio.cpp uart.cpp)
target_include_directories(peripherals PUBLIC include)
#We need to depend on events because of EventEmitter and of other event headers
target_link_libraries(peripherals events)