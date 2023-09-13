//
// Created by xPC on 04.04.2023.
//
#include "Controller.h"

#include <QApplication>
#include "spdlog/spdlog.h"

#include "mainwindow.h"
#include "ConfigLoader.h"
#include "uart.h"

Controller::Controller(int argc, char **argv) {
    spdlog::info("A controller instance has been created");
    argc_ = argc;
    argv_ = argv;
    emulator_unit_ = new emulator::EmulatorUnit();

    //Initialize the global emitter
    EventsLib::getGlobalEmitter();
}

Controller::~Controller() {
    delete emulator_unit_;
    clearActivePeripherals();
    spdlog::info("A controller instance has been destructed");
}

int Controller::showWindow() {
    QApplication a(argc_, argv_);
    MainWindow w(nullptr, this);
    w.show();
    spdlog::trace("A Qt window has been shown");
    return a.exec();
}

void Controller::clearActivePeripherals() {
    for (auto const &peripheral: active_peripherals_) {
        delete peripheral.second;
    }
    active_peripherals_.clear();
    spdlog::info("Active peripheral map has been cleared");

    registerPeripherals_();
}

void Controller::configureEmulator(const std::string &path) {
    clearActivePeripherals();

    spdlog::info("Configuring the EmulatorUnit instance...");

    loadConfig_(path);

    registerPeripherals_();
}

void Controller::loadConfig_(const std::string &path) {
    const configLoader::ConfigData data = configLoader::configParser(path);

    emulator_unit_->SetRamSize(data.ramSize);
    emulator_unit_->SetRamStartAddress(data.ramStartAddress);
    program_arguments_ = data.programArgs;
    active_peripherals_ = data.peripheralDevices;
}

void Controller::registerPeripherals_() {
    emulator_unit_->RegisterPeripherals(active_peripherals_);
    spdlog::info("The active peripherals map has been registered with emulator unit");
}

//######################################################################################################################
//# UI interface methods
//######################################################################################################################

void Controller::loadFile(std::string filePath) {
    emulator_unit_->LoadElfFile(filePath);
    emulator_unit_->ClearBreakpoints();
    spdlog::trace("A new ELF file has been loaded");
}

void Controller::runProgram() {
    emulator_unit_->Execute(program_arguments_);
    spdlog::trace("A program was executed in the emulator unit");
}

void Controller::debugProgram() {
    emulator_unit_->Debug(program_arguments_);
    spdlog::trace("A program was executed in the emulator unit in debug mode");
}

void Controller::debugStep() {
    emulator_unit_->DebugStep();
    spdlog::trace("A debug step was performed");
}

void Controller::debugContinue() {
    emulator_unit_->DebugContinue();
    spdlog::trace("A debug continue was performed");
}

void Controller::terminateProgram() {
    emulator_unit_->Terminate();
    spdlog::trace("A program was terminated");
}

void Controller::addBreakpoint(uint64_t address) {
    emulator_unit_->AddBreakpoint(address);
    spdlog::trace("A breakpoint was added at address {0}", address);
}

void Controller::removeBreakpoint(uint64_t address) {
    emulator_unit_->RemoveBreakpoint(address);
    spdlog::trace("A breakpoint was removed at address {0}", address);
}

void Controller::setPinStatus(std::string moduleName, int pin, bool status) {
    // Try to get GPIO module by name
    auto port = dynamic_cast<modules::GPIO_Port *>(active_peripherals_[moduleName]);
    if (port == nullptr) {
        return;
    }

    port->Set_Pin_Level(pin, status ? modules::GPIO_Pin_Level::HIGH : modules::GPIO_Pin_Level::LOW);
    spdlog::trace("The pin {0} for module {1} was set to {2}", pin, moduleName, status);
}

void Controller::sendUartMessage(std::string uartName, std::string message) {
    auto uart = dynamic_cast<modules::UART_Device *>(active_peripherals_[uartName]);
    uart->TransmitToDevice(message);
    spdlog::trace("A UART message {0} was transmitter to device {1}", uartName, message);
}

void Controller::resetPeripherals() {
    for (auto peripheral: active_peripherals_) {
        peripheral.second->Reset();
    }
    spdlog::info("Active peripherals' state has been reset");
}