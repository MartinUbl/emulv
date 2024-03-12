#pragma once

#include <qqml.h>
#include "EmulvApi.h"
#include "EmulatorInterface.h"

#include "FileSystemModel.h"
#include "MemoryTableModel.h"
#include "RegistersTableModel.h"
#include "CodeAreaModel.h"

class UiController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    // QML needs to receive pointer
    Q_PROPERTY(FileSystemModel *fileSystemModel READ getFileSystemModel CONSTANT)
    Q_PROPERTY(MemoryTableModel *memoryTableModel READ getMemoryTableModel CONSTANT)
    Q_PROPERTY(RegistersTableModel *registersTableModel READ getRegistersTableModel CONSTANT)
    Q_PROPERTY(CodeAreaModel *codeAreaModel READ getCodeAreaModel CONSTANT)

public:
    explicit UiController();

    FileSystemModel *getFileSystemModel() const;

    MemoryTableModel *getMemoryTableModel() const;

    RegistersTableModel *getRegistersTableModel() const;

    CodeAreaModel *getCodeAreaModel() const;


public Q_SLOTS:
    // File explorer / file dialog
    void openFile(const QString& path);
    // Code Area
    void removeBreakpoint(uint64_t address);
    void addBreakpoint(uint64_t address);
    // Run / Debug buttons in QML
    void runProgram();
    void debugProgram();
    void debugStep();
    void debugContinue();
    void terminateProgram();


Q_SIGNALS:
    void disassemblyTextChanged(std::tuple<QList<uint64_t>, QList<QString>> disassembly);
    void errorLoadingFile(QString error);

private:
    const std::unique_ptr<EmulvApi> _emulvApi{std::make_unique<EmulatorInterface>()};

    const std::unique_ptr<FileSystemModel> _fileSystemModel{std::make_unique<FileSystemModel>()};
    const std::unique_ptr<MemoryTableModel> _memoryTableModel{std::make_unique<MemoryTableModel>()};
    const std::unique_ptr<RegistersTableModel> _registersTableModel{std::make_unique<RegistersTableModel>()};
    const std::unique_ptr<CodeAreaModel> _codeAreaModel{std::make_unique<CodeAreaModel>()};

};
