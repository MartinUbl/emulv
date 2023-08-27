#Download the json parser library
include(FetchContent)
FetchContent_Declare(
        json-lib
        GIT_REPOSITORY https://github.com/nlohmann/json.git
)
FetchContent_MakeAvailable(json-lib)
