//
// Created by xPC on 04.04.2023.
//

#include <iostream>
#include "Controller.h"
#include "mainwindow.h"
#include "../utils/events/EventEmitter.h"
#include "../utils/events/SimpleEvent.h"
#include "../modules/uart.h"
#include <QApplication>

int Controller::ShowWindow() {
    QApplication a(argc_, argv_);
    MainWindow w(nullptr, this);

    w.show();
    return a.exec();
}

Controller::Controller(int argc, char **argv) {
    argc_ = argc;
    argv_ = argv;
    emitter_ = EventEmitter();
    emulatorUnit_ = new emulator::EmulatorUnit(emitter_);

    //TODO: add peripheral creation from config file
    CreatePeripherals_();
    RegisterPeripherals_();
}

void Controller::CreatePeripherals_() {
    activePeripherals_["PORT_A"] = new modules::GPIO_Port("PORT_A", emitter_, 0x40010800, 0x40010BFF);
    activePeripherals_["PORT_B"] = new modules::GPIO_Port("PORT_B", emitter_, 0x40010C00, 0x40010FFF);
    activePeripherals_["PORT_C"] = new modules::GPIO_Port("PORT_C", emitter_, 0x40011000, 0x40011BFF);

    activePeripherals_["UART_A"] = new modules::UART_Device("UART_A", emitter_, 0x40013800, 0x40013BFF);
}

void Controller::RegisterPeripherals_() {
    emulatorUnit_->RegisterPeripherals(activePeripherals_);
}

std::map<std::string, modules::PeripheralDevice*> Controller::GetPeripherals() {
    return activePeripherals_;
}

Controller::~Controller() {
    delete emulatorUnit_;
    //Delete peripheral objects
    for (auto const &x: activePeripherals_) {
        delete x.second;
    }
}

//######################################################################################################################
//# UI interface methods
//######################################################################################################################

void Controller::RunProgram() {
    std::vector<std::string> arguments{"Program", "30"}; //TODO: remove
    std::cout << std::endl << "Running program..." << std::endl;
    emulatorUnit_->Execute(arguments);
}

void Controller::DebugProgram() {
    std::vector<std::string> arguments{"Program", "30"}; //TODO: remove
    std::cout << std::endl << "Running program in debug mode..." << std::endl;
    emulatorUnit_->Debug(arguments);
}


std::vector<std::tuple<uint64_t, std::string>> Controller::GetDisassembly() {
    return this->emulatorUnit_->Disassemble();
}

void Controller::LoadFile(std::string file_path) {
    this->openedFile_ = file_path;
    this->emulatorUnit_->LoadElfFile(file_path);
    this->emulatorUnit_->ClearBreakpoints();
}

bool Controller::IsFileLoaded() {
    return !this->openedFile_.empty();
}

std::vector<uint8_t> Controller::GetMemory(const uint64_t from, const uint64_t to) {
    return emulatorUnit_->GetMemory(from, to);
}

std::vector<std::tuple<std::string, uint32_t>> Controller::GetRegisters() {
    return emulatorUnit_->GetRegisters();
}

emulator::EmulatorState Controller::GetProgramState() {
    return emulatorUnit_->GetState();
}

void Controller::DebugStep() {
    emulatorUnit_->DebugStep();
}

uint64_t Controller::GetPc() {
    return emulatorUnit_->GetPc();
}

uint64_t Controller::GetMemoryStartAddress() {
    return emulatorUnit_->GetMemoryStartAddress();
}

uint64_t Controller::GetMemoryEndAddress() {
    return emulatorUnit_->GetMemoryEndAddress();
}

void Controller::DebugContinue() {
    emulatorUnit_->DebugContinue();
}

void Controller::Terminate() {
    emulatorUnit_->Terminate();
}

void Controller::SetPinStatus(std::string module, int pin, bool status) {
    auto port = dynamic_cast<modules::GPIO_Port*>(activePeripherals_[module]);
    if (port == nullptr) {
        return;
    }

    port->Set_Pin_Level(pin, status ? modules::GPIO_Pin_Level::HIGH : modules::GPIO_Pin_Level::LOW);
}

EventEmitter &Controller::GetEventEmitter() {
    return emitter_;
}

int Controller::GetProgramReturnValue() {
    return emulatorUnit_->GetReturnValue();
}

void Controller::AddBreakpoint(uint64_t address) {
    emulatorUnit_->AddBreakpoint(address);
}

void Controller::RemoveBreakpoint(uint64_t address) {
    emulatorUnit_->RemoveBreakpoint(address);
}

void Controller::SendUartMessage(std::string uart_name, std::string message) {
    auto uart = dynamic_cast<modules::UART_Device*>(activePeripherals_[uart_name]);
    uart->TransmitToDevice(message);
}

void Controller::ResetPeripherals() {
    for (auto peripheral : activePeripherals_) {
        peripheral.second->Reset();
    }
}
