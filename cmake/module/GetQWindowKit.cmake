#Download QWindowKit (Qt Quick borderless window) from github
include(FetchContent)
FetchContent_Declare(
        QWindowKit
        GIT_REPOSITORY https://github.com/stdware/qwindowkit
        GIT_TAG 1.0.1
)
set(QWINDOWKIT_BUILD_WIDGETS OFF CACHE BOOL "Don't build qwindowkit widgets" FORCE)
set(QWINDOWKIT_BUILD_STATIC ON CACHE BOOL "Use the qwindowkit static library" FORCE)

FetchContent_MakeAvailable(QWindowKit)