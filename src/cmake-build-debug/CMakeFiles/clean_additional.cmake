# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\emulator_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\emulator_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\emulv_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\emulv_autogen.dir\\ParseCache.txt"
  "_deps\\libriscv-build\\lib\\CMakeFiles\\riscv_autogen.dir\\AutogenUsed.txt"
  "_deps\\libriscv-build\\lib\\CMakeFiles\\riscv_autogen.dir\\ParseCache.txt"
  "_deps\\libriscv-build\\lib\\riscv_autogen"
  "emulator_autogen"
  "emulv_autogen"
  )
endif()
