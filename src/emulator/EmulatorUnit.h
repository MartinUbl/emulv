#ifndef EMULV_EMULATORUNIT_H
#define EMULV_EMULATORUNIT_H
#include <vector>
#include "../utils/events/EventEmitter.h"
#include "../modules/PeripheralDevice.h"
#include "libriscv/types.hpp"

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//! IMPORTANT
//! QT macros are breaking library code from include
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#ifdef foreach
#undef foreach
#endif
#ifdef signals
#undef signals
#endif
#ifdef emit
#undef emit
#endif

#include "libriscv/machine.hpp"

namespace emulator {

    class EmulatorUnit {
    private:
        EventEmitter &emitter_;
        std::vector<uint8_t> binary_;
        std::map<std::string, modules::PeripheralDevice *> *peripheral_devices_ = nullptr;

        static std::string InstructonToString_(riscv::CPU<8> const &cpu, riscv::instruction_format format);

        void SetupMemoryTraps_(riscv::Machine<riscv::RISCV64> &machine) const;

    public:
        EmulatorUnit(EventEmitter &emitter) : emitter_(emitter) {}

        void LoadElfFile(const std::string &file_path);

        int Execute(const std::vector<std::string> &machine_arguments);

        std::vector<std::string> Disassemble();

        void RegisterPeripherals(std::map<std::string, modules::PeripheralDevice *> &devices);
    };

}

#endif
