//
// Created by xPC on 04.04.2023.
//

#pragma once

#include <unordered_set>
#include <limits>
#include "Events.h"
#include "EmulatorUnit.h"

class Controller {
public:
    Controller(int argc, char **argv);

    ~Controller();

    int ShowWindow();

    void ConfigureEmulator(const std::string &path);

    void LoadFile(std::string file_path);

    void RunProgram();

    void DebugProgram();

    void DebugStep();

    void DebugContinue();

    void TerminateProgram();

    void AddBreakpoint(uint64_t address);

    void RemoveBreakpoint(uint64_t address);

    uint64_t GetPc() { return emulator_unit_->GetPc(); }

    int GetProgramReturnValue() { return emulator_unit_->GetReturnValue(); }

    emulator::EmulatorState GetProgramState() { return emulator_unit_->GetState(); }

    std::vector<uint8_t> GetMemory(uint64_t from, uint64_t to) { return emulator_unit_->GetMemory(from, to); }

    std::vector<std::tuple<uint64_t, std::string>> GetDisassembly() { return this->emulator_unit_->Disassemble(); }

    std::vector<std::tuple<std::string, uint32_t>> GetRegisters() { return emulator_unit_->GetRegisters(); }

    std::map<std::string, modules::PeripheralDevice *> GetPeripherals() { return active_peripherals_; }

    uint64_t GetMemoryStartAddress() { return emulator_unit_->GetMemoryStartAddress(); }

    uint64_t GetMemoryEndAddress() { return emulator_unit_->GetMemoryEndAddress(); }

    uint64_t GetRamStartAddress() { return emulator_unit_->GetRamStartAddress(); }

    uint64_t GetRamEndAddress() { return emulator_unit_->GetRamEndAddress(); }

    uint64_t GetRamSize() { return emulator_unit_->GetRamSize(); }

    void SetPinStatus(std::string module, int pin, bool status);

    void SendUARTMessage(std::string uart_name, std::string message);

    void ClearActivePeripherals();

    void ResetPeripherals();

private:
    emulator::EmulatorUnit *emulator_unit_;

    int argc_;
    char **argv_;

    std::vector<std::string> program_arguments_;
    std::map<std::string, modules::PeripheralDevice *> active_peripherals_;

    void RegisterPeripherals();

    void configCollector(const std::string &path);
};
