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
        std::map<uint64_t, std::vector<modules::PeripheralDevice *>> page_peripherals_;
        static std::string InstructonToString_(riscv::CPU<8> const &cpu, riscv::instruction_format format);

        uint64_t GetPageStart_(uint64_t address);
        void MapDeviceToPage_(modules::PeripheralDevice *device);
        modules::PeripheralDevice *GetRealDevice_(uint64_t address);
        void SetupMemoryTraps_(riscv::Machine<riscv::RISCV64> &machine);

    public:
        EmulatorUnit(EventEmitter &emitter) : emitter_(emitter), page_peripherals_() {}

        void LoadElfFile(const std::string &file_path);

        int Execute(const std::vector<std::string> &machine_arguments);

        std::vector<std::string> Disassemble();

        void RegisterPeripherals(std::map<std::string, modules::PeripheralDevice *> &devices);
    };

}

#endif
