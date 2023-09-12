add_library(events EventEmitter.cpp EventData.cpp Events.cpp)
target_include_directories(events PUBLIC include)
