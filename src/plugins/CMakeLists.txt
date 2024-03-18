# Header only library - libLoader
add_library(libLoader INTERFACE)
target_include_directories(libLoader INTERFACE libLoader)

# Header only library - peripheralsApi
add_library(peripheralsApi INTERFACE)
target_include_directories(peripheralsApi INTERFACE ./peripheralsApi)
# peripheralsApi uses Qt objects
find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)
target_link_libraries(peripheralsApi INTERFACE Qt6::Quick)

# Dynamic library cmake projects
add_subdirectory(peripheralsLibs)