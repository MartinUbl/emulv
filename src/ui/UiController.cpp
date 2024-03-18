#include "UiController.h"
#include "spdlog/spdlog.h"

//############################################################
//# Constructor
//############################################################

UiController::UiController() : QObject() {
    QObject::connect(this, &UiController::disassemblyTextChanged, _codeAreaModel.get(), &CodeAreaModel::updateDisassembly);
    QObject::connect(this, &UiController::steppedTo, _codeAreaModel.get(), &CodeAreaModel::stepTo);
    QObject::connect(_codeAreaModel.get(), &CodeAreaModel::addBreakpoint, this, &UiController::addBreakpoint);
    QObject::connect(_codeAreaModel.get(), &CodeAreaModel::removeBreakpoint, this, &UiController::removeBreakpoint);

    QObject::connect(this, &UiController::registersChanged, _registersTableModel.get(), &RegistersTableModel::updateRegisters);

    QObject::connect(this, &UiController::memoryChanged, _memoryTableModel.get(), &MemoryTableModel::loadMemory);

    // Invoke a method when emulator state changes (Switches from running to terminated, etc.)
    EventsLib::globalOn(emulator::State_Changed_Event_Description, [this](EventsLib::EventData data) {
        QMetaObject::invokeMethod(this, "EmulatorStateChanged");
    });
}

UiController::~UiController()
{
    terminateProgram();
}

//############################################################
//# Public Members
//############################################################

void UiController::EmulatorStateChanged()
{
    auto currentState = _emulvApi->getProgramState();
    switch(currentState) {
    case emulator::kDefault:
        Q_EMIT emulatorDefaultState();
        break;
    case emulator::kReady:
        Q_EMIT emulatorReadyState();
        break;
    case emulator::kRunning:
        Q_EMIT emulatorRunningState();
        break;
    case emulator::kRunningDebug:
        Q_EMIT emulatorRunningDebugState();
        break;
    case emulator::kDebugPaused:
        Q_EMIT emulatorDebugPausedState();
        refreshRegisters();
        refreshMemory();
        Q_EMIT steppedTo(_emulvApi->getPc());
        break;
    case emulator::kTerminated:
        Q_EMIT emulatorTerminatedState();
        refreshRegisters();
        // Reset the line highlighting
        Q_EMIT steppedTo(-1);
        break;
    }
}

//############################################################
//# Get / set
//############################################################

FileSystemModel *UiController::getFileSystemModel() const {
    return _fileSystemModel.get();
}

MemoryTableModel *UiController::getMemoryTableModel() const {
    return _memoryTableModel.get();
}

RegistersTableModel *UiController::getRegistersTableModel() const {
    return _registersTableModel.get();
}

CodeAreaModel *UiController::getCodeAreaModel() const
{
    return _codeAreaModel.get();
}

//############################################################
//# Slots
//############################################################

void UiController::openFile(const QString &path) {
    spdlog::info("User is trying to open file {0}", path.toStdString());

    terminateProgram();

    std::string filePath = path.toStdString();
    QList<QString> disassembly;
    QList<uint64_t> lineNumbers;

    if (filePath.empty()) {
        return;
    }

    // TODO?
    //    emulvApi_->terminateProgram();
    //    JoinThread();

    try {
        _emulvApi->loadFile(filePath);
        auto [lineVec, disVector] = _emulvApi->getDisassembly();
        lineNumbers = QList(lineVec.begin(),lineVec.end());
        for (const std::string& str : disVector) {
            disassembly.emplace_back(QString::fromStdString(str));
        }

    }
    catch (const std::exception &e) {
        Q_EMIT errorLoadingFile(QString::fromStdString("Error: Could not load file " + filePath + '\n' + e.what()));
        // Clear old content
        Q_EMIT disassemblyTextChanged({{}, {}});
        return;
    }
    Q_EMIT disassemblyTextChanged({lineNumbers, disassembly});

    EventsLib::globalEmit(emulator::State_Changed_Event_Description);

    // TODO?
    //    memory_widget_->Clear();
    //    registers_widget_->SetRegisters({});
    //    emulvApi_->resetPeripherals();
    //    peripherals_tab_widget_->UpdateWidgets();
    //    lbl_file_->setText(QString::fromStdString(path));

}

void UiController::removeBreakpoint(uint64_t address)
{
    _emulvApi->removeBreakpoint(address);
}

void UiController::addBreakpoint(uint64_t address)
{
    _emulvApi->addBreakpoint(address);
}

void UiController::runProgram()
{
    if(_emulvApi->getProgramState() == emulator::kReady || _emulvApi->getProgramState() == emulator::kTerminated) {
        _joinBackendThread();
        _backendThread = std::make_unique<std::thread>(&EmulvApi::runProgram, _emulvApi.get());
    }
}

void UiController::debugProgram()
{
    if(_emulvApi->getProgramState() == emulator::kReady || _emulvApi->getProgramState() == emulator::kTerminated) {
        _joinBackendThread();
        _backendThread = std::make_unique<std::thread>(&EmulvApi::debugProgram, _emulvApi.get());
    }
}

void UiController::debugStep()
{
    if(_emulvApi->getProgramState() == emulator::kDebugPaused) {
        _emulvApi->debugStep();
        refreshRegisters();
        refreshMemory();
        Q_EMIT steppedTo(_emulvApi->getPc());
    }
}

void UiController::debugContinue()
{
    if(_emulvApi->getProgramState() == emulator::kDebugPaused) {
        _joinBackendThread();
        // Reset the line highlighting
        Q_EMIT steppedTo(-1);
        _backendThread = std::make_unique<std::thread>(&EmulvApi::debugContinue, _emulvApi.get());
    }
}

void UiController::terminateProgram()
{
    auto state = _emulvApi->getProgramState();

    if(state != emulator::kTerminated || state == emulator::kReady || state == emulator::kDefault) {
        _emulvApi->terminateProgram();
    }

    _joinBackendThread();
}

void UiController::refreshRegisters()
{
    Q_EMIT registersChanged(_emulvApi->getRegisters());
}

void UiController::refreshMemory()
{
    Q_EMIT memoryChanged(_emulvApi->getMemory(_emulvApi->getRamStartAddress(), _emulvApi->getRamEndAddress()), _emulvApi->getRamStartAddress());
}
