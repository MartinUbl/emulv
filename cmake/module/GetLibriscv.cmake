#Setting this variable to OFF will disable libriscv memory arena, making all of the virtual memory pages accessible through the m_pages field in libriscv/lib/libriscv/memory.hpp
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(RISCV_FLAT_RW_ARENA OFF)

#Download libriscv from github (version v0.20 is selected by GIT_TAG)
include(FetchContent)
FetchContent_Declare(
        libriscv
        GIT_REPOSITORY https://github.com/fwsGonzo/libriscv
        GIT_TAG v1.1
)
FetchContent_MakeAvailable(libriscv)
