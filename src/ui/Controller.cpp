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
#include "../utils/config/ConfigLoader.h"

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

    ConfigureEmulator_("config.json");
}

void Controller::RegisterPeripherals_() {
    emulatorUnit_->RegisterPeripherals(activePeripherals_);
}

std::map<std::string, modules::PeripheralDevice *> Controller::GetPeripherals() {
    return activePeripherals_;
}

Controller::~Controller() {
    delete emulatorUnit_;
    //Delete peripheral objects
    for (auto const &x: activePeripherals_) {
        delete x.second;
    }
}

void Controller::ConfigureEmulator_(const std::string &path) {
    const nlohmann::json &config = loadConfig(path);

    for (const auto &item: config.items()) {
        if (item.key() == "device") {
            //The device element

            for (const auto &val: item.value().items()) {
                if (val.key() == "ram") {
                    //Information about ram
                    uint64_t maxMemory = val.value()["size"].get<uint64_t>();
                    uint64_t ramStartAddress = std::strtoull(val.value()["start-address"].get<std::string>().c_str(),
                                                             nullptr, 16);
                    emulatorUnit_->SetMaxMemory(maxMemory);
                    emulatorUnit_->SetRamStartAddress(ramStartAddress);
                } else if (val.key() == "program-arguments") {
                    //The program arguments list
                    programArguments_ = val.value().get<std::vector<std::string>>();
                }

            }
        } else if (item.key() == "peripherals") {
            //The peripherals element

            //Iterate over the peripherals list
            for (const auto &val: item.value().items()) {
                std::string type = val.value()["type"].get<std::string>();
                std::string name = val.value()["name"].get<std::string>();
                uint64_t startAddress = std::strtoull(
                        val.value()["mapping"]["start-address"].get<std::string>().c_str(), nullptr, 16);
                uint64_t endAddress = std::strtoull(val.value()["mapping"]["end-address"].get<std::string>().c_str(),
                                                    nullptr, 16);

                //A GPIO_Port element
                if (type == "GPIO_Port") {
                    activePeripherals_[name] = new modules::GPIO_Port(name, emitter_, startAddress, endAddress);
                }

                //An UART_Device element
                if (type == "UART_Device") {
                    activePeripherals_[name] = new modules::UART_Device(name, emitter_, startAddress, endAddress);
                }

            }

        }

    }

    RegisterPeripherals_();
}

//######################################################################################################################
//# UI interface methods
//######################################################################################################################

void Controller::RunProgram() {
    std::cout << std::endl << "Running program..." << std::endl;
    emulatorUnit_->Execute(programArguments_);
}

void Controller::DebugProgram() {
    std::cout << std::endl << "Running program in debug mode..." << std::endl;
    emulatorUnit_->Debug(programArguments_);
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
    auto port = dynamic_cast<modules::GPIO_Port *>(activePeripherals_[module]);
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
    auto uart = dynamic_cast<modules::UART_Device *>(activePeripherals_[uart_name]);
    uart->TransmitToDevice(message);
}

void Controller::ResetPeripherals() {
    for (auto peripheral : activePeripherals_) {
        peripheral.second->Reset();
    }
}
