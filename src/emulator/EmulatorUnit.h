#ifndef EMULV_EMULATORUNIT_H
#define EMULV_EMULATORUNIT_H

#include <vector>
#include "libriscv/types.hpp"
#include "../utils/events/EventEmitter.h"

namespace emulator {

    class EmulatorUnit {
    private:
        EventEmitter& emitter_;
        std::vector<uint8_t> binary_;
        static std::string InstructonToString_(riscv::CPU<8> const &cpu, riscv::instruction_format format);
    public:
        EmulatorUnit(EventEmitter& emitter) : emitter_(emitter) {}

        void LoadElfFile(const std::string &file_path);

        int Execute(const std::vector<std::string> &machine_arguments);

        std::vector<std::string> Disassemble();
    };

}

#endif
