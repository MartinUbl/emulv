add_library(modules ExampleDevice.cpp gpio.cpp uart.cpp)
target_include_directories(modules PUBLIC include)
#We need to depend on events because of EventEmitter and of other event headers
target_link_libraries(modules events)