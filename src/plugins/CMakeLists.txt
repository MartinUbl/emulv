# Header only library
add_library(libraryLoader INTERFACE)
target_include_directories(libraryLoader INTERFACE ./libraryLoader)

# Header only library
add_library(peripherals INTERFACE)
target_include_directories(peripherals INTERFACE ./peripheralsApi)

# Dynamic library cmake projects
add_subdirectory(peripheralsDynamicLibs)