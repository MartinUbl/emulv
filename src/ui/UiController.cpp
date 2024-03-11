#include "UiController.h"

FileSystemModel* UiController::getFileSystemModel() const {
    return _fileSystemModel.get();
}

MemoryTableModel* UiController::getMemoryTableModel() const{
    return _memoryTableModel.get();
}

RegistersTableModel* UiController::getRegistersTableModel() const {
    return _registersTableModel.get();
}


