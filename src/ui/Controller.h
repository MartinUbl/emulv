//
// Created by xPC on 04.04.2023.
//

#ifndef EMULV_CONTROLLER_H
#define EMULV_CONTROLLER_H

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
    std::map<std::string, modules::PeripheralDevice*> ActivePeripherals_;
public:
    Controller(int argc, char **argv);
    ~Controller();

    int ShowWindow();
    int RunProgram();
    void LoadFile(std::string file_path);
    bool IsFileLoaded();
    std::vector<std::string> GetDisassembly();
    std::unordered_map<std::string, uint32_t> GetRegisters();
};


#endif //EMULV_CONTROLLER_H
