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
    ~UiController();

    FileSystemModel *getFileSystemModel() const;

    MemoryTableModel *getMemoryTableModel() const;

    RegistersTableModel *getRegistersTableModel() const;

    CodeAreaModel *getCodeAreaModel() const;

    Q_INVOKABLE void EmulatorStateChanged();

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
    // Registers request for refresh
    void refreshRegisters();
    // Memory request for refrest
    void refreshMemory();

Q_SIGNALS:
    void disassemblyTextChanged(std::tuple<QList<uint64_t>, QList<QString>> disassembly);
    void errorLoadingFile(QString error);
    // Emulator state
    void emulatorDefaultState();
    void emulatorReadyState();
    void emulatorRunningState();
    void emulatorRunningDebugState();
    void emulatorDebugPausedState();
    void emulatorTerminatedState();
    // Registers
    void registersChanged(std::vector<std::tuple<std::string, uint32_t>> registers);
    // Memory
    void memoryChanged(std::vector<uint8_t> memory, uint64_t startAddress);
    // For highlighting of line during debug stepping
    void steppedTo(uint64_t pc);

private:
    const std::unique_ptr<EmulvApi> _emulvApi{std::make_unique<EmulatorInterface>()};

    const std::unique_ptr<FileSystemModel> _fileSystemModel{std::make_unique<FileSystemModel>()};
    const std::unique_ptr<MemoryTableModel> _memoryTableModel{std::make_unique<MemoryTableModel>()};
    const std::unique_ptr<RegistersTableModel> _registersTableModel{std::make_unique<RegistersTableModel>()};
    const std::unique_ptr<CodeAreaModel> _codeAreaModel{std::make_unique<CodeAreaModel>()};

    // Threading - using std::thread instead of Qt threading for simplicity
    std::unique_ptr<std::thread> _backendThread;
    void _joinBackendThread();

};

inline void UiController::_joinBackendThread()
{
    if (_backendThread && _backendThread->joinable()) {
        _backendThread->join();
    }
}
