cmake -S . -B build
cmake --build build
cd build
ctest
cd ..
rm -rf build
