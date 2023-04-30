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
    //Number of CPU's "X-registers"
#define X_REGISTER_COUNT 32

    /**
     * States of the emulator. These are set internally using the SetState_() method.
     * Use the public GetState() method to obtain the current emulator state.
     */
    enum EmulatorState {
        kDefault, kReady, kRunning, kRunningDebug, kDebugPaused, kTerminated
    };

    /**
     * A name of the event which is emitted when the emulator changes state.
     */
    const std::string State_Changed_Event_Description{"emulator-state-changed"};

    class EmulatorUnit {
    private:
        //Emitter passed from controller
        EventEmitter &emitter_;

        //Current state of the emulator
        EmulatorState state_ = kDefault;

        //A binary ELF file, loaded by the LoadElfFile() method
        std::vector<uint8_t> binary_;

        //Pointer to the current active machine, will be equal to nullptr if no machine is running
        riscv::Machine<riscv::RISCV64> *active_machine_ = nullptr;

        //Vector saving the last known register values, is set in the GetRegisters() method, is filled with 0 values by default
        std::vector<std::tuple<std::string, uint32_t>> latest_register_values_;

        //A set containing breakpoint addresses, is controlled by the AddBreakpoint() and RemoveBreakpoint() methods
        std::unordered_set<uint64_t> breakpoints_;

        //Pointer to the peripheral devices map, is passed from controller using the RegisterPeripherals() method
        std::map<std::string, modules::PeripheralDevice *> *peripheral_devices_ = nullptr;

        //A map assigning peripheral devices to memory page start addresses
        std::map<uint64_t, std::vector<modules::PeripheralDevice *>> page_peripherals_;

        /**
         * Converts a single instruction to string using the disassembler library.
         * @param pc_value the current CPU program counter value
         * @param format Instruction format object
         * @return Disassembled instruction
         */
        static std::string InstructonToString_(uint64_t pc_value, riscv::instruction_format format);

        /**
         * Gets the start address of the memory page which contains this address.
         * @param address A memory address
         * @return Memory page start address
         */
        uint64_t GetPageStart_(uint64_t address);

        /**
         * Assigns a peripheral device to the memory page which contains it's start address, results are saved into page_peripherals_.
         * @param device A peripheral device object
         */
        void MapDeviceToPage_(modules::PeripheralDevice *device);

        /**
         * Gets a peripheral device which is mapped to this address. Uses the page_peripherals_ map to find the device.
         * @param address A memory address
         * @return A peripheral device assigned to this address. Returns nullptr if no device is found.
         */
        modules::PeripheralDevice *GetRealDevice_(uint64_t address);

        /**
         * Setups the required memory traps of peripheral devices on this machine instance.
         * PeripheralDevices must first be registered using the RegisterPeripherals() method.
         * @param machine An active machine instance
         */
        void SetupMemoryTraps_(riscv::Machine<riscv::RISCV64> &machine);

        /**
         * Sets state of the emulator.
         * @param state The new emulator state
         */
        void SetState_(EmulatorState state);

        /**
         * Creates a new machine. Reference to it is saved in the active_machine_ attribute.
         * First the old active_machine_ object is deleted, then a new machine is created,
         * and lastly the peripheral device traps are set up on it.
         * @param machine_arguments Arguments of the program
         */
        void CreateNewMachine_(const std::vector<std::string> &machine_arguments);

    public:

        /**
         * Constructs the EmulatorUnit instance.
         * @param emitter Emulator state changed events will be emitted using this emitter.
         */
        EmulatorUnit(EventEmitter &emitter) : emitter_(emitter), page_peripherals_() {
            for (int reg_num = 0; reg_num < X_REGISTER_COUNT; reg_num++) {
                std::string reg_prefix = "x";
                latest_register_values_.emplace_back(reg_prefix + std::to_string(reg_num), 0);
            }
            latest_register_values_.emplace_back("pc", 0);
        }

        /**
         * Loads a binary ELF file into this emulator unit. The ELF file must be executable, and compiled for the RISC-V ISA.
         * @param file_path Path of this file
         */
        void LoadElfFile(const std::string &file_path);

        /**
         * Gets the current state of this emulator unit.
         * @return A value of the EmulatorState enum
         */
        EmulatorState GetState();

        /**
         * Executes the loaded ELF file. Exit code of the program can be obtained using the GetReturnValue() method.
         * @param machine_arguments Arguments of the program
         */
        void Execute(const std::vector<std::string> &machine_arguments);

        /**
         * Disassembles the loaded ELF file.
         * @return A string vector containing lines of the disassembled program
         */
        std::vector<std::string> Disassemble();

        /**
         * Loads a map of peripheral devices into the peripheral_devices_ class attribute.
         * Key of the map is a name of the peripheral, value is an instance of the PeripheralDevice class.
         * @param devices A map of peripheral devices
         */
        void RegisterPeripherals(std::map<std::string, modules::PeripheralDevice *> &devices);

        /**
         * Gets values of the currently active machine's registers. Or gets the last known register values if machine is inactive.
         * @return A vector containing register values
         */
        std::vector<std::tuple<std::string, uint32_t>> GetRegisters();

        /**
         * Reads memory of the currently active machine.
         * @param from Will start reading from this address
         * @param to Will stop reading at this address
         * @return A vector containing memory bytes. Or an empty vector if machine is inactive or invalid values were entered.
         */
        std::vector<std::vector<uint8_t>> GetMemory(uint64_t from, uint64_t to);

        /**
         * Executes the loaded ELF file in debug mode. Stops at first breakpoint.
         * @param machine_arguments Arguments of the program
         */
        void Debug(const std::vector<std::string> &machine_arguments);

        /**
         * Will execute a single instruction. Use this method after starting debug mode by Debug().
         */
        void DebugStep();

        /**
         * Gets program counter value of the currently active machine.
         * @return A PC value. Throws an exception if there is no active machine.
         */
        uint64_t GetPc();

        /**
         * Will continue until the next breakpoint. Use this method after starting debug mode by Debug().
         */
        void DebugContinue();

        /**
         * Will stop the currently active machine and terminate this emulator unit.
         */
        void Terminate();

        /**
         * Will get the last program return value.
         * @return A program return value
         */
        int GetReturnValue();

        /**
         * Adds a new breakpoint by its address.
         * @param address Memory address of this breakpoint
         */
        void AddBreakpoint(uint64_t address);

        /**
         * Removes a breakpoint by its address.
         * @param address Memory address of this breakpoint
         */
        void RemoveBreakpoint(uint64_t address);

        /**
         * Removes all set breakpoints.
         */
        void ClearBreakpoints();

        /**
         * Gets the first memory address of the machine's memory area.
         * @return Memory start address
         */
        uint64_t GetMemoryStartAddress();

        /**
         * Gets the last memory address of the machine's memory area.
         * @return Memory end address
         */
        uint64_t GetMemoryEndAddress();

    };

}

#endif
