include(GetLibriscv)
include(GetRiscv-disassembler)

add_library(emulator EmulatorUnit.cpp)
target_include_directories(emulator PUBLIC include)

#Libriscv library
target_link_libraries(emulator riscv)
#Riscv disassembler library
target_link_libraries(emulator ${riscv-disassembler_SOURCE_DIR}/src/riscv-disas.c)
target_link_libraries(emulator events)
target_link_libraries(emulator peripheralsApi)

#This is necessary or we won't be able to include the riscv-disas.h file
target_include_directories(emulator PRIVATE ${riscv-disassembler_SOURCE_DIR}/src)