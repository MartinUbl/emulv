FetchContent_Declare(
        riscv-disassembler
        GIT_REPOSITORY https://github.com/michaeljclark/riscv-disassembler.git
)
FetchContent_MakeAvailable(riscv-disassembler)

#include_directories(${riscv-disassembler_SOURCE_DIR}/src)