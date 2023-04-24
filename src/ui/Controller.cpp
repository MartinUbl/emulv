//
// Created by xPC on 04.04.2023.
//

#include <iostream>
#include "Controller.h"
#include "mainwindow.h"
#include "../utils/events/EventEmitter.h"
#include <QApplication>

int Controller::ShowWindow() {
    QApplication a(argc_, argv_);
    MainWindow w(nullptr, this);

    //TODO: Remove
    emitter_.On("data", [&](auto res) {
        std::cout << res << std::endl;
    });
    emitter_.Emit("data", "Hello world");

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
