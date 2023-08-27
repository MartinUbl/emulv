add_library(events BreakpointAreaWidgetEvents.cpp EventEmitter.cpp gpio_events.cpp SimpleEvent.cpp uart_event.cpp)
target_include_directories(events PUBLIC include)
target_link_libraries(events PRIVATE modules)