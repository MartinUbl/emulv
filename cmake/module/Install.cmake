# This script should be able to deploy the QtApp on linux. All of our own libraries should be
# statically linked (they have the .a file extension), so there should be no need to include
# them with the program itself.

# TODO - Figure our LINUX and WINDOWS deployment of Qt
# https://stackoverflow.com/questions/2036873/static-library-install-under-linux-should-it-be-similar-to-dynamic-library-ins
# https://github.com/ttroy50/cmake-examples/blob/master/06-installer/deb/CMakeLists.txt
# https://www.qt.io/blog/deploying-to-linux-with-cmake

# CPack configuration
set(CPACK_PACKAGE_VENDOR "JD")
set(CPACK_PACKAGE_CONTACT "info@zcu.cz")
set(CPACK_PACKAGE_DESCRIPTION "RISC-V emulator")
include(CPack)
