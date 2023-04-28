#ifndef EMULV_EMULATORUNIT_H
#define EMULV_EMULATORUNIT_H

#include <vector>
#include <unordered_set>
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
<<<<<<< Updated upstream
        riscv::Machine<riscv::RISCV64> *activeMachine_;
        std::map<std::string, modules::PeripheralDevice *> *peripheral_devices_ = nullptr;
        std::map<uint64_t, std::vector<modules::PeripheralDevice *>> page_peripherals_;


=======
        riscv::Machine<riscv::RISCV64> *active_machine_ = nullptr;
        std::vector<std::tuple<std::string, uint32_t>> latest_register_values_;
        std::map<std::string, modules::PeripheralDevice *> *peripheral_devices_ = nullptr;
        std::map<uint64_t, std::vector<modules::PeripheralDevice *>> page_peripherals_;

>>>>>>> Stashed changes
        static std::string InstructonToString_(riscv::CPU<8> const &cpu, riscv::instruction_format format);

        uint64_t GetPageStart_(uint64_t address);

        void MapDeviceToPage_(modules::PeripheralDevice *device);

        modules::PeripheralDevice *GetRealDevice_(uint64_t address);

        void SetupMemoryTraps_(riscv::Machine<riscv::RISCV64> &machine);

    public:
        EmulatorUnit(EventEmitter &emitter) : emitter_(emitter), page_peripherals_() {
            for (int i = 0; i < 33; i++) {
                std::string reg_prefix = "x";
                latest_register_values_.emplace_back(reg_prefix + std::to_string(i), 0);
            }
            latest_register_values_.emplace_back("pc", 0);
        }

        void LoadElfFile(const std::string &file_path);

        int Execute(const std::vector<std::string> &machine_arguments);

        std::vector<std::string> Disassemble();

        void RegisterPeripherals(std::map<std::string, modules::PeripheralDevice *> &devices);

<<<<<<< Updated upstream
        std::vector<std::vector<uint8_t>> GetMemory(uint64_t from, uint64_t to);

        std::vector<std::tuple<std::string, uint32_t>> GetRegisters();
=======
        std::vector<std::tuple<std::string, uint32_t>> GetRegisters();

        std::vector<std::vector<uint8_t>> GetMemory(uint64_t from, uint64_t to);

        void Debug(const std::vector<std::string> &machine_arguments);

        bool DebugStep();

        uint64_t GetPc();

        bool DebugContinue(const std::unordered_set<int64_t>& breakpointAddresses);

        uint64_t GetMemoryStartAddress();

        uint64_t GetMemoryEndAddress();
>>>>>>> Stashed changes
    };

}

#endif
