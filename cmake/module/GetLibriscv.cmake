#Download libriscv from github (version v0.20 is selected by GIT_TAG)
include(FetchContent)
FetchContent_Declare(
        libriscv
        GIT_REPOSITORY https://github.com/fwsGonzo/libriscv
        GIT_TAG 442925844a
)
FetchContent_MakeAvailable(libriscv)
