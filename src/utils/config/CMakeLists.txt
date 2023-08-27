include(GetJson)
add_library(config ConfigLoader.cpp)
#Headers inside the include directory can now be included without specifying absolute path,
#by adding PUBLIC this applies to any targets which depends on the config library.
target_include_directories(config PUBLIC include)
target_link_libraries(config nlohmann_json)