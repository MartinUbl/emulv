#pragma once

#include <qqml.h>
#include "EmulvApi.h"
#include "EmulatorInterface.h"

#include "FileSystemModel.h"
#include "MemoryTableModel.h"
#include "RegistersTableModel.h"

class UiController : public QObject {
    Q_OBJECT

    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(FileSystemModel* fileSystemModel READ getFileSystemModel CONSTANT)
    Q_PROPERTY(MemoryTableModel* memoryTableModel READ getMemoryTableModel CONSTANT)
    Q_PROPERTY(RegistersTableModel* registersTableModel READ getRegistersTableModel CONSTANT)


public:
    explicit UiController() : QObject() {}
    explicit UiController(QObject *parent) : QObject(parent) {}

    FileSystemModel* getFileSystemModel() const;

    MemoryTableModel* getMemoryTableModel() const;

    RegistersTableModel* getRegistersTableModel() const;

private:
    const std::unique_ptr<EmulvApi> _emulvApi{std::make_unique<EmulatorInterface>()};
    const std::unique_ptr<FileSystemModel> _fileSystemModel{std::make_unique<FileSystemModel>()};
    const std::unique_ptr<MemoryTableModel> _memoryTableModel{std::make_unique<MemoryTableModel>()};
    const std::unique_ptr<RegistersTableModel> _registersTableModel{std::make_unique<RegistersTableModel>()};
};
