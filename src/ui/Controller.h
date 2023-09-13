//
// Created by xPC on 04.04.2023.
//

#pragma once

#include <unordered_set>
#include <limits>
#include "Events.h"
#include "EmulatorUnit.h"
#include "EmulvApi.h"

class Controller : public EmulvApi {
private:
    int argc_;
    char **argv_;

    emulator::EmulatorUnit *emulator_unit_;
    std::vector<std::string> program_arguments_;
    std::map<std::string, modules::PeripheralDevice *> active_peripherals_;

    void registerPeripherals_();

    void loadConfig_(const std::string &path);

public:
    Controller(int argc, char **argv);

    ~Controller();

    int showWindow();

    //###########################################################################
    //# API METHODS
    //###########################################################################

    void clearActivePeripherals() override;

    void configureEmulator(const std::string &path) override;

    void loadFile(std::string filePath) override;

    void resetPeripherals() override;

    void runProgram() override;

    void debugProgram() override;

    void debugStep() override;

    void debugContinue() override;

    void terminateProgram() override;

    void addBreakpoint(uint64_t address) override;

    void removeBreakpoint(uint64_t address) override;

    uint64_t getPc() override { return emulator_unit_->GetPc(); }

    int getProgramReturnValue() override { return emulator_unit_->GetReturnValue(); }

    emulator::EmulatorState getProgramState() override { return emulator_unit_->GetState(); }

    std::vector<uint8_t> getMemory(uint64_t from, uint64_t to) override { return emulator_unit_->GetMemory(from, to); }

    std::vector<std::tuple<uint64_t, std::string>>
    getDisassembly() override { return this->emulator_unit_->Disassemble(); }

    std::vector<std::tuple<std::string, uint32_t>> getRegisters() override { return emulator_unit_->GetRegisters(); }

    std::map<std::string, modules::PeripheralDevice *> getPeripherals() override { return active_peripherals_; }

    uint64_t getMemoryStartAddress() override { return emulator_unit_->GetMemoryStartAddress(); }

    uint64_t getMemoryEndAddress() override { return emulator_unit_->GetMemoryEndAddress(); }

    uint64_t getRamStartAddress() override { return emulator_unit_->GetRamStartAddress(); }

    uint64_t getRamEndAddress() override { return emulator_unit_->GetRamEndAddress(); }

    uint64_t getRamSize() override { return emulator_unit_->GetRamSize(); }

    void setPinStatus(std::string moduleName, int pin, bool status) override;

    void sendUartMessage(std::string uartName, std::string message) override;

};
