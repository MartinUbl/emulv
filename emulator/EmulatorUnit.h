//
// Created by xPC on 05.03.2023.
//

#ifndef EMULATOR_EMULATORUNIT_H
#define EMULATOR_EMULATORUNIT_H

#include <string>
#include <vector>

class EmulatorUnit {
private:
    std::vector<uint8_t> binary_;
public:
    void LoadElfFile(const std::string& file_path);
    int Execute(const std::vector<std::string>& machine_arguments);
};


#endif //EMULATOR_EMULATORUNIT_H
