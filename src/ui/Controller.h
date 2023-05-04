//
// Created by xPC on 04.04.2023.
//

#ifndef EMULV_CONTROLLER_H
#define EMULV_CONTROLLER_H

#include <unordered_set>
#include <limits>
#include "../utils/events/EventEmitter.h"
#include "../emulator/EmulatorUnit.h"
#include "../modules/PeripheralDevice.h"
#include "../modules/ExampleDevice.h"
#include "../modules/gpio.h"

class Controller {
private:
    int argc_;
    char **argv_;
    EventEmitter emitter_;
    emulator::EmulatorUnit *emulatorUnit_;
    std::string openedFile_;
    std::vector<std::string> programArguments_;

    void RegisterPeripherals_();

    std::map<std::string, modules::PeripheralDevice *> activePeripherals_;

    void ConfigureEmulator_(const std::string& path);
public:
    Controller(int argc, char **argv);

    ~Controller();

    int ShowWindow();

    void RunProgram();

    EventEmitter &GetEventEmitter();

    std::vector<std::tuple<uint64_t, std::string>> GetDisassembly();

    std::map<std::string, modules::PeripheralDevice *> GetPeripherals();

    void SetPinStatus(std::string module, int pin, bool status);

    void LoadFile(std::string file_path);

    bool IsFileLoaded();

    std::vector<uint8_t> GetMemory(uint64_t from, uint64_t to);

    std::vector<std::tuple<std::string, uint32_t>> GetRegisters();

    emulator::EmulatorState GetProgramState();

    void DebugProgram();

    void DebugStep();

    uint64_t GetPc();

    void DebugContinue();

    void Terminate();

    int GetProgramReturnValue();

    void AddBreakpoint(uint64_t address);

    void RemoveBreakpoint(uint64_t address);

    uint64_t GetMemoryStartAddress();

    uint64_t GetMemoryEndAddress();

    void SendUartMessage(std::string uart_name, std::string message);

    void ResetPeripherals();
};


#endif //EMULV_CONTROLLER_H
