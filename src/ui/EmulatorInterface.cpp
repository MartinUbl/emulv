#include "EmulatorInterface.h"

#include "spdlog/spdlog.h"

#include "ConfigLoader.h"
#include "uart.h"

//###############################################################
//# Constructor / destructor
//###############################################################

EmulatorInterface::EmulatorInterface() {
    spdlog::info("An EmulatorInterface instance has been created");
    emulatorUnit_ = std::make_unique<emulator::EmulatorUnit>();
}

EmulatorInterface::~EmulatorInterface() {
    clearActivePeripherals();
    spdlog::info("An EmulatorInterface instance has been destructed");
}

//###############################################################
//# Class members
//###############################################################

void EmulatorInterface::loadConfig_(const std::string &path) {
    const configLoader::ConfigData data = configLoader::configParser(path);

    emulatorUnit_->SetRamSize(data.ramSize);
    emulatorUnit_->SetRamStartAddress(data.ramStartAddress);
    programArguments_ = data.programArgs;
    activePeripherals_ = data.peripheralDevices;
}

void EmulatorInterface::registerPeripherals_() {
    emulatorUnit_->RegisterPeripherals(activePeripherals_);
    spdlog::info("The active peripherals map has been registered with emulator unit");
}

//###############################################################
//# UI interface methods (EmulvApi.h)
//###############################################################

void EmulatorInterface::clearActivePeripherals() {
    for (auto const &peripheral: activePeripherals_) {
        delete peripheral.second;
    }
    activePeripherals_.clear();
    spdlog::info("Active peripheral map has been cleared");

    registerPeripherals_();
}

void EmulatorInterface::configureEmulator(const std::string &path) {
    clearActivePeripherals();

    spdlog::info("Configuring the EmulatorUnit instance...");

    loadConfig_(path);

    registerPeripherals_();
}

void EmulatorInterface::loadFile(std::string filePath) {
    emulatorUnit_->LoadElfFile(filePath);
    emulatorUnit_->ClearBreakpoints();
    spdlog::trace("A new ELF file has been loaded");
}

void EmulatorInterface::runProgram() {
    emulatorUnit_->Execute(programArguments_);
    spdlog::trace("A program was executed in the emulator unit");
}

void EmulatorInterface::debugProgram() {
    emulatorUnit_->Debug(programArguments_);
    spdlog::trace("A program was executed in the emulator unit in debug mode");
}

void EmulatorInterface::debugStep() {
    emulatorUnit_->DebugStep();
    spdlog::trace("A debug step was performed");
}

void EmulatorInterface::debugContinue() {
    emulatorUnit_->DebugContinue();
    spdlog::trace("A debug continue was performed");
}

void EmulatorInterface::terminateProgram() {
    emulatorUnit_->Terminate();
    spdlog::trace("A program was terminated");
}

void EmulatorInterface::addBreakpoint(uint64_t address) {
    emulatorUnit_->AddBreakpoint(address);
    spdlog::trace("A breakpoint was added at address {0}", address);
}

void EmulatorInterface::removeBreakpoint(uint64_t address) {
    emulatorUnit_->RemoveBreakpoint(address);
    spdlog::trace("A breakpoint was removed at address {0}", address);
}

void EmulatorInterface::setPinStatus(std::string moduleName, int pin, bool status) {
    // Try to get GPIO module by name
//    auto port = dynamic_cast<peripherals::GPIO_Port *>(activePeripherals_[moduleName]);
//    if (port == nullptr) {
//        return;
//    }
//
//    port->Set_Pin_Level(pin, status ? peripherals::GPIO_Pin_Level::HIGH : peripherals::GPIO_Pin_Level::LOW);
//    spdlog::trace("The pin {0} for module {1} was set to {2}", pin, moduleName, status);
}

void EmulatorInterface::sendUartMessage(std::string uartName, std::string message) {
    auto uart = dynamic_cast<peripherals::UART_Device *>(activePeripherals_[uartName]);
    uart->TransmitToDevice(message);
    spdlog::trace("A UART message {0} was transmitted to device {1}", uartName, message);
}

void EmulatorInterface::resetPeripherals() {
    for (const auto& peripheral: activePeripherals_) {
        peripheral.second->Reset();
    }
    spdlog::info("Active peripherals' state has been reset");
}
