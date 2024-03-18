#pragma once

#include <memory>
#include <utility>
#include <string>
#include <stdexcept>
#include "defines.h"

/**
 * Dynamic library loader providing a linux / windows interface for loading a C++ dynamic library.
 * @tparam T The class contained in this library.
 */
template<class T>
class LibLoader {
private:
    HANDLE_TYPE _handle;
    std::string _pathToLib;
    std::string _allocClassSymbol;
    std::string _deleteClassSymbol;

public:
    explicit LibLoader(std::string pathToLib,
                       std::string allocClassSymbol = "allocator",
                       std::string deleteClassSymbol = "deleter") :
            _handle(nullptr), _pathToLib(std::move(pathToLib)),
            _allocClassSymbol(std::move(allocClassSymbol)), _deleteClassSymbol(std::move(deleteClassSymbol)) {}

    virtual ~LibLoader() {
        closeLib();
    }

    /**
     * Opens the dynamic library (specified by pathToLib in the constructor)
     */
    void openLib() {
        if (!(_handle = my_dlopen(_pathToLib.c_str()))) {
            throw std::runtime_error(error_msg);
        }
    }

    /**
     * Creates an instance of the class contained in this dynamic library.
     * @return Shared pointer to an instance of the class
     */
    std::shared_ptr<T> getInstance() {
        using allocClass = T *(*)();
        using deleteClass = void (*)(T *);

        auto allocFunc = reinterpret_cast<allocClass>(my_dlsym(_handle, _allocClassSymbol.c_str()));
        auto deleteFunc = reinterpret_cast<deleteClass>(my_dlsym(_handle, _deleteClassSymbol.c_str()));

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
        if(!_handle)
            return;

        if(has_dlclose_failed(my_dlclose(_handle))) {
            throw std::runtime_error(error_msg);
        }

        _handle = nullptr;
    }
};
