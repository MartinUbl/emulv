#pragma once

#include <memory>
#include <iostream>
#include <utility>

#ifdef WIN32

#include "Windows.h"

#endif
#ifdef __linux__

#include <dlfcn.h>

#endif

/**
 * Dynamic library loader providing a linux / windows interface for loading a C++ dynamic library.
 * @tparam T The class contained in this library.
 */
template<class T>
class libraryLoader {
private:

#ifdef WIN32
    HMODULE _handle;
#endif
#ifdef __linux__
    void *_handle;
#endif

    std::string _pathToLib;
    std::string _allocClassSymbol;
    std::string _deleteClassSymbol;

public:
    explicit libraryLoader(std::string pathToLib,
                       std::string allocClassSymbol = "allocator",
                       std::string deleteClassSymbol = "deleter") :
            _handle(nullptr), _pathToLib(std::move(pathToLib)),
            _allocClassSymbol(std::move(allocClassSymbol)), _deleteClassSymbol(std::move(deleteClassSymbol)) {}

    virtual ~libraryLoader() {
        closeLib();
    }

    /**
     * Opens the dynamic library (specified by pathToLib in the constructor)
     */
    void openLib() {
#ifdef WIN32
        if (!(_handle = LoadLibrary(_pathToLib.c_str()))) {
            throw std::runtime_error("Can't open and load " + _pathToLib);
        }
#endif
#ifdef __linux__
        if (!(_handle = dlopen(_pathToLib.c_str(), RTLD_NOW | RTLD_LAZY))) {
                throw std::runtime_error(dlerror());
        }
#endif
    }

    /**
     * Creates an instance of the class contained in this dynamic library.
     * @return Shared pointer to an instance of the class
     */
    std::shared_ptr<T> getInstance() {
        using allocClass = T *(*)();
        using deleteClass = void (*)(T *);

#ifdef WIN32
        auto allocFunc = reinterpret_cast<allocClass>(GetProcAddress(_handle, _allocClassSymbol.c_str()));
        auto deleteFunc = reinterpret_cast<deleteClass>(GetProcAddress(_handle, _deleteClassSymbol.c_str()));
#endif
#ifdef __linux__
        auto allocFunc = reinterpret_cast<allocClass>(dlsym(_handle, _allocClassSymbol.c_str()));
        auto deleteFunc = reinterpret_cast<deleteClass>(dlsym(_handle, _deleteClassSymbol.c_str()));
#endif

        if (!allocFunc || !deleteFunc) {
            closeLib();
            throw std::runtime_error("Can't find allocator or deleter symbol in " + _pathToLib);
        }

        return std::shared_ptr<T>(
                allocFunc(),
                [deleteFunc](T *p) { deleteFunc(p); });
    }

    /**
     * Closes the dynamically loaded library.
     */
    void closeLib() {
#ifdef WIN32
        if (FreeLibrary(_handle) == 0) {
            throw std::runtime_error("Can't close " + _pathToLib);
        }
#endif
#ifdef __linux__
        if (dlclose(_handle) != 0) {
            throw std::runtime_error(std::string(dlerror()));
        }
#endif
    }
};
