add_executable(events_test test_base.h EventData_test.cpp Events_test.cpp)
target_link_libraries(events_test PRIVATE events)
AddTests(events_test)