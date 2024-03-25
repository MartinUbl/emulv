
#pragma once

#include <string>
#include "EmulatorUnit.h"

/**
 * This abstract class contains all methods which are available for the GUI to use.
 * A GUI controller should have these methods always implemented.
 */
class EmulvApi {
public:
    virtual ~EmulvApi() {}

    virtual void clearActivePeripherals() = 0;

    virtual void configureEmulator(const std::string &path) = 0;

    virtual void loadFile(std::string filePath) = 0;

    virtual void resetPeripherals() = 0;

    virtual void runProgram() = 0;

    virtual void debugProgram() = 0;

    virtual void debugStep() = 0;

    virtual void debugContinue() = 0;

    virtual void terminateProgram() = 0;

    virtual void addBreakpoint(uint64_t address) = 0;

    virtual void removeBreakpoint(uint64_t address) = 0;

    virtual uint64_t getPc() = 0;

    virtual int getProgramReturnValue() = 0;

    virtual emulator::EmulatorState getProgramState() = 0;

    virtual const emulator::PagesMap* getMemoryPages() = 0;

    virtual std::tuple<std::vector<uint64_t>, std::vector<std::string>> getDisassembly() = 0;

    virtual std::vector<std::tuple<std::string, uint32_t>> getRegisters() = 0;

    virtual std::map<std::string, std::shared_ptr<peripherals::PeripheralsApi>>& getPeripherals() = 0;

    virtual uint64_t getMemoryStartAddress() = 0;

    virtual uint64_t getMemoryEndAddress() = 0;

    virtual uint64_t getRamStartAddress() = 0;

    virtual uint64_t getRamEndAddress() = 0;

    virtual uint64_t getRamSize() = 0;

};