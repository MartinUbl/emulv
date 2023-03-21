# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/xPC/Documents/GitHub/emulv/src/cmake-build-debug/_deps/libriscv-src"
  "C:/Users/xPC/Documents/GitHub/emulv/src/cmake-build-debug/_deps/libriscv-build"
  "C:/Users/xPC/Documents/GitHub/emulv/src/cmake-build-debug/_deps/libriscv-subbuild/libriscv-populate-prefix"
  "C:/Users/xPC/Documents/GitHub/emulv/src/cmake-build-debug/_deps/libriscv-subbuild/libriscv-populate-prefix/tmp"
  "C:/Users/xPC/Documents/GitHub/emulv/src/cmake-build-debug/_deps/libriscv-subbuild/libriscv-populate-prefix/src/libriscv-populate-stamp"
  "C:/Users/xPC/Documents/GitHub/emulv/src/cmake-build-debug/_deps/libriscv-subbuild/libriscv-populate-prefix/src"
  "C:/Users/xPC/Documents/GitHub/emulv/src/cmake-build-debug/_deps/libriscv-subbuild/libriscv-populate-prefix/src/libriscv-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/xPC/Documents/GitHub/emulv/src/cmake-build-debug/_deps/libriscv-subbuild/libriscv-populate-prefix/src/libriscv-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/xPC/Documents/GitHub/emulv/src/cmake-build-debug/_deps/libriscv-subbuild/libriscv-populate-prefix/src/libriscv-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
