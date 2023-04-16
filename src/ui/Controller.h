//
// Created by xPC on 04.04.2023.
//

#ifndef EMULV_CONTROLLER_H
#define EMULV_CONTROLLER_H

#include "../utils/events/EventEmitter.h"
#include "../emulator/EmulatorUnit.h"
#include "../modules/PeripheralDevice.h"
#include "../modules/ExampleDevice.h"

class Controller {
private:
    int argc_;
    char **argv_;
    EventEmitter emitter_;
    emulator::EmulatorUnit *emulatorUnit_;
    std::string openedFile_;
    void CreatePeripherals_();
    void RegisterPeripherals_();
    std::map<std::string, PeripheralDevice*> ActivePeripherals_;
public:
    Controller(int argc, char **argv);
    int ShowWindow();
    int RunProgram();
    std::vector<std::string> GetDisassembly();
    void LoadFile(std::string file_path);
    bool IsFileLoaded();
    ~Controller();
};


#endif //EMULV_CONTROLLER_H
