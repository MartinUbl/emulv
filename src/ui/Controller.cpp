//
// Created by xPC on 04.04.2023.
//
#include "Controller.h"

#include <QApplication>

#include "mainwindow.h"
#include "../utils/events/EventEmitter.h"
#include "../utils/events/SimpleEvent.h"
#include "../utils/config/ConfigLoader.h"
#include "../modules/uart.h"

Controller::Controller(int argc, char **argv) {
    argc_ = argc;
    argv_ = argv;
    emitter_ = EventEmitter();
    emulator_unit_ = new emulator::EmulatorUnit(emitter_);
}

Controller::~Controller() {
    delete emulator_unit_;
    ClearActivePeripherals();
}

int Controller::ShowWindow() {
    QApplication a(argc_, argv_);
    MainWindow w(nullptr, this);
    w.show();
    return a.exec();
}

void Controller::ClearActivePeripherals() {
    for (auto const &peripheral: active_peripherals_) {
        delete peripheral.second;
    }
    active_peripherals_.clear();

    RegisterPeripherals();
}

void Controller::ConfigureEmulator(const std::string &path) {
    ClearActivePeripherals();

    const nlohmann::json &config = loadConfig(path);

    for (const auto &item: config.items()) {
        if (item.key() == "device") {
            //The device element

            for (const auto &val: item.value().items()) {
                if (val.key() == "ram") {
                    //Information about ram
                    uint64_t ramSize = val.value()["size"].get<uint64_t>();
                    uint64_t ramStartAddress = std::strtoull(val.value()["start-address"].get<std::string>().c_str(),
                                                             nullptr, 16);
                    emulator_unit_->SetRamSize(ramSize);
                    emulator_unit_->SetRamStartAddress(ramStartAddress);
                } else if (val.key() == "program-arguments") {
                    //The program arguments list
                    program_arguments_ = val.value().get<std::vector<std::string>>();
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
                    active_peripherals_[name] = new modules::GPIO_Port(name, emitter_, startAddress, endAddress);
                }

                //An UART_Device element
                if (type == "UART_Device") {
                    active_peripherals_[name] = new modules::UART_Device(name, emitter_, startAddress, endAddress);
                }
            }
        }
    }

    RegisterPeripherals();
}

//######################################################################################################################
//# UI interface methods
//######################################################################################################################

void Controller::LoadFile(std::string file_path) {
    emulator_unit_->LoadElfFile(file_path);
    emulator_unit_->ClearBreakpoints();
}

void Controller::RunProgram() {
    emulator_unit_->Execute(program_arguments_);
}

void Controller::DebugProgram() {
    emulator_unit_->Debug(program_arguments_);
}

void Controller::DebugStep() {
    emulator_unit_->DebugStep();
}

void Controller::DebugContinue() {
    emulator_unit_->DebugContinue();
}

void Controller::TerminateProgram() {
    emulator_unit_->Terminate();
}

void Controller::AddBreakpoint(uint64_t address) {
    emulator_unit_->AddBreakpoint(address);
}

void Controller::RemoveBreakpoint(uint64_t address) {
    emulator_unit_->RemoveBreakpoint(address);
}

void Controller::SetPinStatus(std::string module, int pin, bool status) {
    // Try to get GPIO module by name
    auto port = dynamic_cast<modules::GPIO_Port *>(active_peripherals_[module]);
    if (port == nullptr) {
        return;
    }

    port->Set_Pin_Level(pin, status ? modules::GPIO_Pin_Level::HIGH : modules::GPIO_Pin_Level::LOW);
}

void Controller::SendUARTMessage(std::string uart_name, std::string message) {
    auto uart = dynamic_cast<modules::UART_Device *>(active_peripherals_[uart_name]);
    uart->TransmitToDevice(message);
}

void Controller::ResetPeripherals() {
    for (auto peripheral : active_peripherals_) {
        peripheral.second->Reset();
    }
}

void Controller::RegisterPeripherals() {
    emulator_unit_->RegisterPeripherals(active_peripherals_);
}
