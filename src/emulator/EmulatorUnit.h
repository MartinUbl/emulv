#ifndef EMULV_EMULATORUNIT_H
#define EMULV_EMULATORUNIT_H

#include <vector>

namespace emulator {

    class EmulatorUnit {
    private:
        std::vector<uint8_t> binary_;
        static std::string InstructonToString_(riscv::CPU<8> const &cpu, riscv::instruction_format format);
    public:
        void LoadElfFile(const std::string &file_path);

        int Execute(const std::vector<std::string> &machine_arguments);

        std::vector<std::string> Disassemble();


    };

}

#endif
