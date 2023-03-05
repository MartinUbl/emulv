#!/bin/bash
set -e

mkdir -p cmake-build-debug
pushd cmake-build-debug
cmake .. -DCMAKE_BUILD_TYPE=Release -DFETCHCONTENT_UPDATES_DISCONNECTED=ON
make -j4
popd

# fib(64) == 10610209857723
./cmake-build-debug/emulator fib.rv64.elf 64
