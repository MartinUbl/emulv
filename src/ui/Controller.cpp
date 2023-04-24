//
// Created by xPC on 04.04.2023.
//

#include <iostream>
#include "Controller.h"
#include "mainwindow.h"
#include "../utils/events/EventEmitter.h"
#include "../utils/events/SimpleEvent.h"
#include <QApplication>

int Controller::ShowWindow() {
    QApplication a(argc_, argv_);
    MainWindow w(nullptr, this);

    //TODO: Remove
    emitter_.On("SimpleEventHello1", [](AbstractEvent *res) {
        SimpleEvent *simpleEvent = dynamic_cast< SimpleEvent *>(res);

        std::cout << "An event has been captured: " << std::endl << "Event Name: SimpleEventHello1" << std::endl
                  << "The data of this object is: " << simpleEvent->getData() << std::endl;

        //Don't forget to free the event object after using it
        delete res;
    });
    emitter_.Emit("SimpleEventHello1",new SimpleEvent("Hello world. SIMPLE EVENT DATA."));

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
    activePeripherals_["PORT_A"] = new modules::GPIO_Port(emitter_, 0x40010800, 0x40010BFF);
    activePeripherals_["PORT_B"] = new modules::GPIO_Port(emitter_, 0x40010C00, 0x40010FFF);
}

void Controller::RegisterPeripherals_() {
    emulatorUnit_->RegisterPeripherals(activePeripherals_);
}

modules::PeripheralDevice* Controller::GetPeripheral(std::string name) {
    auto entry = ActivePeripherals_.find(name);
    if (entry != ActivePeripherals_.end()) {
        return entry->second;
    }
    return nullptr;
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

int Controller::RunProgram() {
    std::vector<std::string> arguments{"Program", "30"}; //TODO: remove
    std::cout << std::endl << "Running program..." << std::endl;
    int exitCode = emulatorUnit_->Execute(arguments);
    return exitCode;
}

void Controller::DebugProgram() {
    std::vector<std::string> arguments{"Program", "30"}; //TODO: remove
    std::cout << std::endl << "Running program in debug mode..." << std::endl;
    emulatorUnit_->Debug(arguments);
}


std::vector<std::string> Controller::GetDisassembly() {
    return this->emulatorUnit_->Disassemble();
}

void Controller::LoadFile(std::string file_path) {
    this->openedFile_ = file_path;
    this->emulatorUnit_->LoadElfFile(file_path);
}

bool Controller::IsFileLoaded() {
    return !this->openedFile_.empty();
}

std::vector<std::vector<uint8_t>> Controller::GetMemory(const uint64_t from, const uint64_t to) {
    return emulatorUnit_->GetMemory(from, to);
}

std::vector<std::tuple<std::string, uint32_t>> Controller::GetRegisters() {
    return emulatorUnit_->GetRegisters();
}

bool Controller::DebugStep() {
    return emulatorUnit_->DebugStep();
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

bool Controller::DebugContinue(const std::unordered_set<int64_t>& breakpointAddresses) {
    return emulatorUnit_->DebugContinue(breakpointAddresses);
}
void Controller::SetPinStatus(std::string module, int pin, bool status) {
    auto port = dynamic_cast<modules::GPIO_Port*>(GetPeripheral(module));
    if (port == nullptr) {
        return;
    }

    port->Set_Pin_Level(pin, status ? modules::GPIO_Pin_Level::HIGH : modules::GPIO_Pin_Level::LOW);
}

