add_subdirectory(emulator) # <-- Target emulator depends on modules and events
add_subdirectory(modules) # <-- Target modules depends on events
add_subdirectory(ui)
add_subdirectory(utils) # <-- Targets config and events, events depends on modules