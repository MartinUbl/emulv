#include "UiController.h"
#include "spdlog/spdlog.h"

//############################################################
//# Constructor
//############################################################

UiController::UiController() : QObject() {
    QObject::connect(this, &UiController::disassemblyTextChanged, _codeAreaModel.get(), &CodeAreaModel::updateDisassembly);
    QObject::connect(_codeAreaModel.get(), &CodeAreaModel::addBreakpoint, this, &UiController::addBreakpoint);
    QObject::connect(_codeAreaModel.get(), &CodeAreaModel::removeBreakpoint, this, &UiController::removeBreakpoint);

    // Invoke this class method when emulator state changes (Switches from running to terminated, etc.)
    EventsLib::globalOn(emulator::State_Changed_Event_Description, [this](EventsLib::EventData data) {
        QMetaObject::invokeMethod(this, "EmulatorStateChanged");
    });
}

//############################################################
//# Public Members
//############################################################


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

void UiController::EmulatorStateChanged()
{

}

//############################################################
//# Slots
//############################################################

void UiController::openFile(const QString &path) {
    spdlog::info("User is trying to open file {0}", path.toStdString());

    std::string filePath = path.toStdString();
    QList<QString> disassembly;
    QList<uint64_t> lineNumbers;

    if (filePath.empty()) {
        return;
    }

    //TODO
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

    //TODO
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
    if(_emulvApi->getProgramState() == emulator::kReady || _emulvApi->getProgramState() == emulator::kTerminated)
        _emulvApi->runProgram();
}

void UiController::debugProgram()
{
    if(_emulvApi->getProgramState() == emulator::kReady || _emulvApi->getProgramState() == emulator::kTerminated)
        _emulvApi->debugProgram();
}

void UiController::debugStep()
{
    _emulvApi->debugStep();
    //TODO PC, registers, memory
}

void UiController::debugContinue()
{
    _emulvApi->debugContinue();
}

void UiController::terminateProgram()
{
    _emulvApi->terminateProgram();
    //TODO Registers, memory
}



