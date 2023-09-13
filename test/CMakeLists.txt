#Includes helper script which allows us to use the AddTests(<TARGET_NAME>) function
include(Testing)

#All test directories
add_subdirectory(gpio)
add_subdirectory(uart)
add_subdirectory(events)