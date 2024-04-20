#pragma once

#if defined(WIN32)
#include "Windows.h"
#include <system_error>
using HANDLE_TYPE = HMODULE;

#define my_dlopen(path) LoadLibraryA(path)

#define my_dlsym(handle, symbol) GetProcAddress(handle, symbol)

#define my_dlclose(handle) FreeLibrary(handle)
#define has_dlclose_failed(return_value) return_value == 0

#define error_msg std::string("LoadLibrary / CloseLibrary error code: ") + std::to_string(GetLastError())

#elif defined(__linux__)
#include <dlfcn.h>
    using HANDLE_TYPE = void*;

    #define my_dlopen(path) dlopen(path, RTLD_NOW | RTLD_LAZY)

    #define my_dlsym(handle, symbol) dlsym(handle, symbol)

    #define my_dlclose(handle) dlclose(handle)
    #define has_dlclose_failed(return_value) return_value != 0

    #define error_msg dlerror()

#else
    #error "Unsupported platform"
#endif

