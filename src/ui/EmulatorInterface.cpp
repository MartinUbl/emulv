//
// Created by xPC on 04.04.2023.
//
#include "EmulatorInterface.h"

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

EmulatorInterface::EmulatorInterface(int argc, char **argv) {
    spdlog::info("A controller instance has been created");
    argc_ = argc;
    argv_ = argv;
    emulatorUnit_ = std::make_unique<emulator::EmulatorUnit>();

    //Initialize the global emitter
    EventsLib::getGlobalEmitter();
}

EmulatorInterface::~EmulatorInterface() {
    clearActivePeripherals();
    spdlog::info("A controller instance has been destructed");
}

int EmulatorInterface::showWindow() {
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
    engine.loadFromModule("EmulvQt", "Main");

    //Monospace font
    const QFont monospaceFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    engine.rootContext()->setContextProperty("monospaceFont", monospaceFont);

    spdlog::trace("A Qt window has been shown");
    return app.exec();
}

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

//######################################################################################################################
//# UI interface methods
//######################################################################################################################

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
    for (auto peripheral: activePeripherals_) {
        peripheral.second->Reset();
    }
    spdlog::info("Active peripherals' state has been reset");
}