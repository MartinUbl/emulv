//
// Created by xPC on 04.04.2023.
//

#ifndef EMULV_CONTROLLER_H
#define EMULV_CONTROLLER_H

#include <unordered_set>
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
    void CreatePeripherals_();
    void RegisterPeripherals_();
    std::map<std::string, modules::PeripheralDevice*> activePeripherals_;
public:
    Controller(int argc, char **argv);
    ~Controller();

    int ShowWindow();
    int RunProgram();
    EventEmitter &GetEventEmitter();
    std::vector<std::string> GetDisassembly();
    std::map<std::string, modules::PeripheralDevice*> GetPeripherals();
    void SetPinStatus(std::string module, int pin, bool status);
    void LoadFile(std::string file_path);
    bool IsFileLoaded();
    std::vector<std::vector<uint8_t>> GetMemory(uint64_t from, uint64_t to);
    std::vector<std::tuple<std::string, uint32_t>> GetRegisters();

    void DebugProgram();

    bool DebugStep();

    uint64_t GetPc();

    bool DebugContinue(const std::unordered_set<int64_t>& breakpointAddresses);

    uint64_t GetMemoryStartAddress();

    uint64_t GetMemoryEndAddress();
};


#endif //EMULV_CONTROLLER_H
