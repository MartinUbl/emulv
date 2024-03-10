//
// Created by xPC on 04.04.2023.
//
#include "Controller.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStandardPaths>
#include <QFontDatabase>
#include <QQmlContext>
#include <QIcon>
#include <QWKQuick/qwkquickglobal.h>
#include "spdlog/spdlog.h"

#include "ConfigLoader.h"
#include "uart.h"

Controller::Controller(int argc, char **argv) {
    spdlog::info("A controller instance has been created");
    argc_ = argc;
    argv_ = argv;
    emulatorUnit_ = std::make_unique<emulator::EmulatorUnit>();

    //Initialize the global emitter
    EventsLib::getGlobalEmitter();
}

Controller::~Controller() {
    clearActivePeripherals();
    spdlog::info("A controller instance has been destructed");
}

int Controller::showWindow() {
    QGuiApplication app(argc_, argv_);

    QGuiApplication::setOrganizationName("jonas");
    QGuiApplication::setApplicationName("emulv");
    QGuiApplication::setApplicationVersion(QT_VERSION_STR);
    QGuiApplication::setWindowIcon(QIcon(":/assets/ev_square.svg"));

    // ENABLE OPENGL
    qputenv("QSG_RHI_BACKEND", "opengl");
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QQmlApplicationEngine engine;

    // Qt Borderless window library
    QWK::registerTypes(&engine);

    QObject::connect(
            &engine,
            &QQmlApplicationEngine::objectCreationFailed,
            &app,
            []() { QCoreApplication::exit(-1); },
            Qt::QueuedConnection);
    engine.loadFromModule("Borderless_lib_test", "Main");

    //Monospace font
    const QFont monospaceFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    engine.rootContext()->setContextProperty("monospaceFont", monospaceFont);

    spdlog::trace("A Qt window has been shown");
    return app.exec();
}

void Controller::clearActivePeripherals() {
    for (auto const &peripheral: activePeripherals_) {
        delete peripheral.second;
    }
    activePeripherals_.clear();
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

    emulatorUnit_->SetRamSize(data.ramSize);
    emulatorUnit_->SetRamStartAddress(data.ramStartAddress);
    programArguments_ = data.programArgs;
    activePeripherals_ = data.peripheralDevices;
}

void Controller::registerPeripherals_() {
    emulatorUnit_->RegisterPeripherals(activePeripherals_);
    spdlog::info("The active peripherals map has been registered with emulator unit");
}

//######################################################################################################################
//# UI interface methods
//######################################################################################################################

void Controller::loadFile(std::string filePath) {
    emulatorUnit_->LoadElfFile(filePath);
    emulatorUnit_->ClearBreakpoints();
    spdlog::trace("A new ELF file has been loaded");
}

void Controller::runProgram() {
    emulatorUnit_->Execute(programArguments_);
    spdlog::trace("A program was executed in the emulator unit");
}

void Controller::debugProgram() {
    emulatorUnit_->Debug(programArguments_);
    spdlog::trace("A program was executed in the emulator unit in debug mode");
}

void Controller::debugStep() {
    emulatorUnit_->DebugStep();
    spdlog::trace("A debug step was performed");
}

void Controller::debugContinue() {
    emulatorUnit_->DebugContinue();
    spdlog::trace("A debug continue was performed");
}

void Controller::terminateProgram() {
    emulatorUnit_->Terminate();
    spdlog::trace("A program was terminated");
}

void Controller::addBreakpoint(uint64_t address) {
    emulatorUnit_->AddBreakpoint(address);
    spdlog::trace("A breakpoint was added at address {0}", address);
}

void Controller::removeBreakpoint(uint64_t address) {
    emulatorUnit_->RemoveBreakpoint(address);
    spdlog::trace("A breakpoint was removed at address {0}", address);
}

void Controller::setPinStatus(std::string moduleName, int pin, bool status) {
    // Try to get GPIO module by name
//    auto port = dynamic_cast<peripherals::GPIO_Port *>(activePeripherals_[moduleName]);
//    if (port == nullptr) {
//        return;
//    }
//
//    port->Set_Pin_Level(pin, status ? peripherals::GPIO_Pin_Level::HIGH : peripherals::GPIO_Pin_Level::LOW);
//    spdlog::trace("The pin {0} for module {1} was set to {2}", pin, moduleName, status);
}

void Controller::sendUartMessage(std::string uartName, std::string message) {
    auto uart = dynamic_cast<peripherals::UART_Device *>(activePeripherals_[uartName]);
    uart->TransmitToDevice(message);
    spdlog::trace("A UART message {0} was transmitted to device {1}", uartName, message);
}

void Controller::resetPeripherals() {
    for (auto peripheral: activePeripherals_) {
        peripheral.second->Reset();
    }
    spdlog::info("Active peripherals' state has been reset");
}