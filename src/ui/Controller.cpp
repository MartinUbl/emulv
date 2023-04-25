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
    ActivePeripherals_["PORT_A"] = new modules::GPIO_Port(emitter_, 0x40010800, 0x40010BFF);
    ActivePeripherals_["PORT_B"] = new modules::GPIO_Port(emitter_, 0x40010C00, 0x40010FFF);
}

void Controller::RegisterPeripherals_() {
    emulatorUnit_->RegisterPeripherals(ActivePeripherals_);
}

Controller::~Controller() {
    delete emulatorUnit_;
    //Delete peripheral objects
    for (auto const &x: ActivePeripherals_) {
        delete x.second;
    }
}

//######################################################################################################################
//# UI interface methods
//######################################################################################################################

int Controller::RunProgram() {
    std::vector<std::string> arguments{"Program", "30"};
    std::cout << std::endl << "Running program..." << std::endl;
    int exitCode = emulatorUnit_->Execute(arguments);
    return exitCode;
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
    // TODO: get memory from EmulatorUnit
    std::vector<std::vector<uint8_t>> memory;

    // TODO: remove
    for (uint64_t i = from; i <= to; ++i) {
        memory.push_back({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    }
    return memory;
}

std::vector<std::tuple<std::string, uint32_t>> Controller::GetRegisters() {
    // TODO: get registers from EmulatorUnit
    std::vector<std::tuple<std::string, uint32_t>> registers;

    // TODO: remove
    registers.emplace_back("x0", 0);
    registers.emplace_back("x1", 0x5f880900);
    registers.emplace_back("x2", 0x5f880900);
    registers.emplace_back("x3", 0x5f880900);
    registers.emplace_back("x4", 0x5f880900);
    registers.emplace_back("x5", 0x5f880900);
    registers.emplace_back("x6", 0x5f880900);
    registers.emplace_back("x20", 0x5f880900);
    registers.emplace_back("x31", 0x5f880900);
    registers.emplace_back("pc", 0x00037a40);

    return registers;
}
