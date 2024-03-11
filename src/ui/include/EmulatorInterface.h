#pragma once

#include <unordered_set>
#include <limits>
#include "Events.h"
#include "EmulatorUnit.h"
#include "EmulvApi.h"

class EmulatorInterface : public EmulvApi {
private:
    /**
     * Pointer to the active emulatorUnit_ instance
     */
    std::unique_ptr<emulator::EmulatorUnit> emulatorUnit_;

    /**
     * A list of emulatorUnit program arguments, loaded from the configuration file
     */
    std::vector<std::string> programArguments_;

    /**
     * A map containing all active peripherals, loaded from the configuration file
     */
    std::map<std::string, peripherals::PeripheralDevice *> activePeripherals_;

    void registerPeripherals_();

    void loadConfig_(const std::string &path);

public:
    EmulatorInterface();

    ~EmulatorInterface();

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

    uint64_t getPc() override { return emulatorUnit_->GetPc(); }

    int getProgramReturnValue() override { return emulatorUnit_->GetReturnValue(); }

    emulator::EmulatorState getProgramState() override { return emulatorUnit_->GetState(); }

    std::vector<uint8_t> getMemory(uint64_t from, uint64_t to) override { return emulatorUnit_->GetMemory(from, to); }

    std::vector<std::tuple<uint64_t, std::string>>
    getDisassembly() override { return this->emulatorUnit_->Disassemble(); }

    std::vector<std::tuple<std::string, uint32_t>> getRegisters() override { return emulatorUnit_->GetRegisters(); }

    std::map<std::string, peripherals::PeripheralDevice *> getPeripherals() override { return activePeripherals_; }

    uint64_t getMemoryStartAddress() override { return emulatorUnit_->GetMemoryStartAddress(); }

    uint64_t getMemoryEndAddress() override { return emulatorUnit_->GetMemoryEndAddress(); }

    uint64_t getRamStartAddress() override { return emulatorUnit_->GetRamStartAddress(); }

    uint64_t getRamEndAddress() override { return emulatorUnit_->GetRamEndAddress(); }

    uint64_t getRamSize() override { return emulatorUnit_->GetRamSize(); }

    void setPinStatus(std::string moduleName, int pin, bool status) override;

    void sendUartMessage(std::string uartName, std::string message) override;

};
