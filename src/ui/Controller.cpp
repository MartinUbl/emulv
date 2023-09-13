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
    ClearActivePeripherals();
    spdlog::info("A controller instance has been destructed");
}

int Controller::ShowWindow() {
    QApplication a(argc_, argv_);
    MainWindow w(nullptr, this);
    w.show();
    spdlog::trace("A Qt window has been shown");
    return a.exec();
}

void Controller::ClearActivePeripherals() {
    for (auto const &peripheral: active_peripherals_) {
        delete peripheral.second;
    }
    active_peripherals_.clear();
    spdlog::info("Active peripheral map has been cleared");

    RegisterPeripherals();
}

void Controller::ConfigureEmulator(const std::string &path) {
    ClearActivePeripherals();

    spdlog::info("Configuring the EmulatorUnit instance...");

    configCollector(path);

    RegisterPeripherals();
}

void Controller::configCollector(const std::string &path) {
    const nlohmann::json &config = loadConfig(path);

    spdlog::info("Parsing peripherals configuration...");
    for (const auto &item: config.items()) {
        if (item.key() == "device") {
            //The device element
            spdlog::info("Found a new 'device' config entry");

            for (const auto &val: item.value().items()) {
                if (val.key() == "ram") {
                    //Information about ram
                    uint64_t ramSize = val.value()["size"].get<uint64_t>();
                    uint64_t ramStartAddress = strtoull(val.value()["start-address"].get<std::string>().c_str(),
                                                             nullptr, 16);
                    emulator_unit_->SetRamSize(ramSize);
                    emulator_unit_->SetRamStartAddress(ramStartAddress);

                    spdlog::info("Set the RamSize to {0} and RamStartAddress to {1}", ramSize, ramStartAddress);
                } else if (val.key() == "program-arguments") {
                    //The program arguments list
                    program_arguments_ = val.value().get<std::vector<std::string>>();

                    spdlog::info("Set the program_arguments");
                }
            }
        } else if (item.key() == "peripherals") {
            //The peripherals element
            spdlog::info("Found a new 'peripherals' config entry");

            //Iterate over the peripherals list
            for (const auto &val: item.value().items()) {
                std::string type = val.value()["type"].get<std::string>();
                std::string name = val.value()["name"].get<std::string>();
                uint64_t startAddress = strtoull(
                        val.value()["mapping"]["start-address"].get<std::string>().c_str(), nullptr, 16);
                uint64_t endAddress = strtoull(val.value()["mapping"]["end-address"].get<std::string>().c_str(),
                                                    nullptr, 16);

                spdlog::info("Found a peripheral of type: {0} name: {1} startAddress: {2} endAddress: {3}", type, name,
                             startAddress, endAddress);
                //A GPIO_Port element
                if (type == "GPIO_Port") {
                    active_peripherals_[name] = new modules::GPIO_Port(name, startAddress, endAddress);
                }

                //An UART_Device element
                if (type == "UART_Device") {
                    active_peripherals_[name] = new modules::UART_Device(name, startAddress, endAddress);
                }
            }
        }
    }
}

//######################################################################################################################
//# UI interface methods
//######################################################################################################################

void Controller::LoadFile(std::string file_path) {
    emulator_unit_->LoadElfFile(file_path);
    emulator_unit_->ClearBreakpoints();
    spdlog::trace("A new ELF file has been loaded");
}

void Controller::RunProgram() {
    emulator_unit_->Execute(program_arguments_);
    spdlog::trace("A program was executed in the emulator unit");
}

void Controller::DebugProgram() {
    emulator_unit_->Debug(program_arguments_);
    spdlog::trace("A program was executed in the emulator unit in debug mode");
}

void Controller::DebugStep() {
    emulator_unit_->DebugStep();
    spdlog::trace("A debug step was performed");
}

void Controller::DebugContinue() {
    emulator_unit_->DebugContinue();
    spdlog::trace("A debug continue was performed");
}

void Controller::TerminateProgram() {
    emulator_unit_->Terminate();
    spdlog::trace("A program was terminated");
}

void Controller::AddBreakpoint(uint64_t address) {
    emulator_unit_->AddBreakpoint(address);
    spdlog::trace("A breakpoint was added at address {0}", address);
}

void Controller::RemoveBreakpoint(uint64_t address) {
    emulator_unit_->RemoveBreakpoint(address);
    spdlog::trace("A breakpoint was removed at address {0}", address);
}

void Controller::SetPinStatus(std::string module, int pin, bool status) {
    // Try to get GPIO module by name
    auto port = dynamic_cast<modules::GPIO_Port *>(active_peripherals_[module]);
    if (port == nullptr) {
        return;
    }

    port->Set_Pin_Level(pin, status ? modules::GPIO_Pin_Level::HIGH : modules::GPIO_Pin_Level::LOW);
    spdlog::trace("The pin {0} for module {1} was set to {2}", pin, module, status);
}

void Controller::SendUARTMessage(std::string uart_name, std::string message) {
    auto uart = dynamic_cast<modules::UART_Device *>(active_peripherals_[uart_name]);
    uart->TransmitToDevice(message);
    spdlog::trace("A UART message {0} was transmitter to device {1}", uart_name, message);
}

void Controller::ResetPeripherals() {
    for (auto peripheral: active_peripherals_) {
        peripheral.second->Reset();
    }
    spdlog::info("Active peripherals' state has been reset");
}

void Controller::RegisterPeripherals() {
    emulator_unit_->RegisterPeripherals(active_peripherals_);
    spdlog::info("The active peripherals map has been registered with emulator unit");
}
