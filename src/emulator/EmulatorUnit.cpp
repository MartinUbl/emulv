#include <fstream>
#include <iostream>
#include <limits>

#include <libriscv/machine.hpp>
#include "EmulatorUnit.h"
#include "libriscv/rv32i_instr.hpp"
#include "riscv-disas.h"

namespace emulator {


    /**
     * Loads an executable risc-v ELF file into this emulator instance.
     * @param file_path Path of the file
     */
    void EmulatorUnit::LoadElfFile(const std::string &file_path) {
        // Read the RISC-V program into a std::vector:
        std::ifstream stream(file_path, std::ios::in | std::ios::binary);
        if (!stream) {
            throw std::runtime_error("EmulatorUnit::LoadElfFle: File not found or is inaccessible!");
        }

        binary_ = std::vector<uint8_t>(
                (std::istreambuf_iterator<char>(stream)),
                std::istreambuf_iterator<char>()
        );

        SetState_(kReady);
    }

    void EmulatorUnit::SetState_(emulator::EmulatorState state) {
        if (state != state_) {
            state_ = state;
            emitter_.Emit(emulator::State_Changed_Event_Description, nullptr);
        }
    }

    emulator::EmulatorState EmulatorUnit::GetState() {
        return state_;
    }

    /**
     * Debug the loaded program.
     * @param machine_arguments Arguments of the program
     * @return Exit code of the program
     */
    void EmulatorUnit::Debug(const std::vector<std::string> &machine_arguments) {
        //Delete old machine
        if (active_machine_ != nullptr) {
            delete active_machine_;
        }

        // Create a new 64-bit RISC-V machine
        active_machine_ = new riscv::Machine<riscv::RISCV64>{binary_};

        // Use string vector as arguments to the RISC-V program
        active_machine_->setup_linux(
                machine_arguments,
                {"LC_TYPE=C", "LC_ALL=C", "USER=root"});
        active_machine_->setup_linux_syscalls();
        active_machine_->set_max_instructions(std::numeric_limits<uint64_t>::max());

        // Setup memory traps for peripherals
        if (peripheral_devices_ != nullptr)
            SetupMemoryTraps_(*active_machine_);

        SetState_(kRunningDebug);
        try {
            while (!active_machine_->stopped()) {
                if (breakpoints_.count(active_machine_->cpu.pc())) {
                    SetState_(kDebugPaused);
                    return;
                }
                active_machine_->cpu.step_one();
            }
        } catch (const std::exception &e) {
            std::cout << "Program error: " << e.what() << std::endl;
            SetState_(kTerminated);
            return;
        }

        //This will save the last register values
        GetRegisters();
        SetState_(kTerminated);
    }

    void EmulatorUnit::DebugStep() {
        if (active_machine_ == nullptr) {
            throw std::runtime_error("EmulatorUnit::DebugStep: active_machine_ is equal to null!");
        }

        if (!active_machine_->stopped()) {
            active_machine_->cpu.step_one();
        }

        if (active_machine_->stopped()) {
            SetState_(kTerminated);
        }
    }

    void EmulatorUnit::DebugContinue() {
        if (active_machine_ == nullptr) {
            throw std::runtime_error("EmulatorUnit::DebugStep: active_machine_ is equal to null!");
        }

        SetState_(kRunningDebug);
        while (!active_machine_->stopped()) {
            active_machine_->cpu.step_one();
            if (breakpoints_.count(active_machine_->cpu.pc())) {
                SetState_(kDebugPaused);
                return;
            }
        }

        SetState_(kTerminated);
    }

    /**
     * Execute the loaded ELF file.
     * @param machine_arguments Arguments of the program
     * @return Exit code of the program
     */
    void EmulatorUnit::Execute(const std::vector<std::string> &machine_arguments) {
        //Delete old machine
        if (active_machine_ != nullptr) {
            delete active_machine_;
        }

        // Create a new 64-bit RISC-V machine
        active_machine_ = new riscv::Machine<riscv::RISCV64>{binary_};

        // Use string vector as arguments to the RISC-V program
        active_machine_->setup_linux(
                machine_arguments,
                {"LC_TYPE=C", "LC_ALL=C", "USER=root"});
        active_machine_->setup_linux_syscalls();
        active_machine_->set_max_instructions(std::numeric_limits<uint64_t>::max());

        // Setup memory traps for peripherals
        if (peripheral_devices_ != nullptr)
            SetupMemoryTraps_(*active_machine_);

        SetState_(kRunning);
        try {
            while (!active_machine_->stopped()) {
                active_machine_->cpu.step_one();
            }
        } catch (const std::exception &e) {
            std::cout << "Program error: " << e.what() << std::endl;
            SetState_(kTerminated);
            return;
        }
        //This will save the last register values
        GetRegisters();
        SetState_(kTerminated);
    }

    void EmulatorUnit::Terminate() {
        if (active_machine_ == nullptr) {
            return;
        }
        active_machine_->stop();

        SetState_(kTerminated);
    }

    /**
     * Disassembles the loaded ELF file.
     * @return A string vector containing lines of the disassembled program
     */
    std::vector<std::string> EmulatorUnit::Disassemble() {
        riscv::Machine<riscv::RISCV64> machine{binary_};

        machine.setup_linux(
                {"myprogram"},
                {"LC_TYPE=C", "LC_ALL=C", "USER=root"});
        machine.setup_linux_syscalls();

        // Instruction limit is used to keep running
        machine.set_max_instructions(1'000'000UL);

        std::vector<std::string> output;

        while (!machine.stopped()) {
            auto &cpu = machine.cpu;

            //Stop the machine when end of executable segment is reached
            if (cpu.pc() == cpu.current_execute_segment()->exec_end()) {
                break;
            }

            // Read next instruction
            const auto instruction = cpu.read_next_instruction();

            // Store the disassembled instruction
            output.push_back(std::to_string(cpu.pc()) + " " + InstructonToString_(cpu, instruction));

            // Increment PC to next instruction, and increment instruction counter
            cpu.increment_pc(instruction.length());
            machine.increment_counter(1);
        }

        return output;
    }

    /**
     * Loads a map of peripheral devices into the peripheral_devices_ class attribute,
     * key of the map is a name of the peripheral, value is the instance of the PeripheralDevice class
     * @param devices A map of peripheral devices
     */
    void EmulatorUnit::RegisterPeripherals(std::map<std::string, modules::PeripheralDevice *> &devices) {
        peripheral_devices_ = &devices;
    }

    std::vector<std::vector<uint8_t>> EmulatorUnit::GetMemory(uint64_t from, uint64_t to) {
        std::vector<std::vector<uint8_t>> memory;

        if (active_machine_ == nullptr) {
            //Machine is inactive. Return empty memory.
            return memory;
        }

        std::vector<uint8_t> v;
        int v_cap = 0;

        //Get memory content from machine.
        for (uint64_t i = from; i <= to; ++i) {
            v.push_back(active_machine_->memory.read<uint8_t>(i));
            v_cap++;

            if (v_cap >= 16) {
                memory.push_back(v);
                v_cap = 0;
                v.clear();
            }
        }

        return memory;
    }

    std::vector<std::tuple<std::string, uint32_t>> EmulatorUnit::GetRegisters() {
        if (active_machine_ == nullptr) {
            //Machine is inactive. Return the last saved register values.
            return latest_register_values_;
        }

        std::vector<std::tuple<std::string, uint32_t>> registers;
        latest_register_values_.clear();

        for (int i = 0; i < 33; i++) {
            std::string reg_prefix = "x";
            registers.emplace_back(reg_prefix + std::to_string(i), active_machine_->cpu.reg(i));
        }
        registers.emplace_back("pc", active_machine_->cpu.pc());

        latest_register_values_ = registers;
        return registers;
    }

    uint64_t EmulatorUnit::GetPc() {
        if (active_machine_ == nullptr) {
            throw std::runtime_error("EmulatorUnit::GetPc: active_machine_ is equal to null!");
        }

        return active_machine_->cpu.pc();
    }

    uint64_t EmulatorUnit::GetMemoryStartAddress() {
        if (active_machine_ == nullptr) {
            throw std::runtime_error("EmulatorUnit::GetMemoryStartAddress: active_machine_ is equal to null!");
        }
        //TODO what is "start_address"?
        return active_machine_->memory.start_address();
    }

    uint64_t EmulatorUnit::GetMemoryEndAddress() {
        if (active_machine_ == nullptr) {
            throw std::runtime_error("EmulatorUnit::GetMemoryEndAddress: active_machine_ is equal to null!");
        }

        return active_machine_->memory.exit_address();
    }

    /**
     * Disassembles instructions via the disassembler library
     * @param cpu riscv CPU object
     * @param format Instruction format
     * @return Disassembled instruction
     */
    std::string EmulatorUnit::InstructonToString_(riscv::CPU<8> const &cpu, riscv::instruction_format format) {
        char buf[256] = {0};
        disasm_inst(buf, sizeof(buf), rv32, cpu.pc(), (uint16_t) format.whole);

        auto result = std::string(buf);
        return result;
    }

    uint64_t EmulatorUnit::GetPageStart_(uint64_t address) {
        return address & 0xFFFFFFFFFFFFF000;
    }

    void EmulatorUnit::MapDeviceToPage_(modules::PeripheralDevice *device) {
        uint64_t page_start = GetPageStart_(device->GetStartAddress());

        auto entry = page_peripherals_.find(page_start);
        if (entry != page_peripherals_.end()) {
            entry->second.push_back(device);
        } else {
            auto peripherals = std::vector<modules::PeripheralDevice *>();
            peripherals.push_back(device);
            page_peripherals_[page_start] = peripherals;
        }
    }

    modules::PeripheralDevice *EmulatorUnit::GetRealDevice_(uint64_t address) {
        uint64_t page_start = GetPageStart_(address);
        auto page_peripherals = this->page_peripherals_[page_start];

        for (auto p: page_peripherals) {
            if (p->GetStartAddress() <= address && address <= p->GetEndAddress()) {
                return p;
            }
        }
        return nullptr;
    }

    /**
     * Setups the required memory traps of peripheral devices on this machine instance.
     * PeripheralDevices must first be registered using the RegisterPeripherals() method.
     * @param machine The current emulated machine instance
     */
    void EmulatorUnit::SetupMemoryTraps_(riscv::Machine<riscv::RISCV64> &machine) {
        for (const auto &p: *peripheral_devices_) {
            //TODO: Remove print
            std::cout << "Setting up memory trap for device with name: " << p.first << std::endl;

            modules::PeripheralDevice *pDevice = p.second;
            MapDeviceToPage_(pDevice);

            //Check if the address range isn't too big
            if ((pDevice->GetStartAddress() + RISCV_PAGE_SIZE) < pDevice->GetEndAddress()) {
                throw std::runtime_error(
                        "EmulatorUnit::RegisterPeripheral: The address range of this peripheral device is greater than the maximal supported page size of 4096 bytes.");
            }

            uint64_t TRAP_PAGE = pDevice->GetStartAddress();
            //Create a trap page - the default size is 4096 bytes
            auto const &trap_page = machine.memory.create_writable_pageno(
                    riscv::Memory<riscv::RISCV64>::page_number(TRAP_PAGE));

            //Sets a callback on this trap page
            trap_page.set_trap([pDevice, this](auto &page, uint32_t offset, int mode, int64_t value) {
                //int mode --> a bitfield containing MODE and SIZE
                //Page::trap_mode(mode) --> Extracts the MODE (read / write) from the mode bitfield's upper 4 bits
                //Page::trap_size(mode) --> Extracts the SIZE from the mode bitfield's lower 12 bits
                //SIZE --> in this context means the size of the read / write operation in bytes (4 bytes = 32bit processor / 8 bytes = 64bit processor)
                //int64_t value --> is in case of write the value which is to be written to memory
                //auto &page --> is the page object, on which this trap occurred

                // Find the real device for which this callback was called for
                uint64_t page_start = GetPageStart_(pDevice->GetStartAddress());
                uint64_t real_address = page_start + offset;
                modules::PeripheralDevice *real_device = GetRealDevice_(real_address);

                if (!real_device) {
                    return;
                }

                // Real offset relative to the devices start address
                uint64_t real_offset = real_address - real_device->GetStartAddress();

                const size_t size = riscv::Page::trap_size(mode);

                switch (riscv::Page::trap_mode(mode)) {
                    case riscv::TRAP_WRITE:
//                        std::cout << "TRAPPED WRITE" << std::endl << "Page size: " << size << std::endl
//                                  << "Offset: " << offset << std::endl << "Mode: " << mode << std::endl
//                                  << "Value: " << value << std::endl << std::endl;

                        if (size == 8)
                            real_device->WriteDoubleword(real_offset, value);
                        else if (size == 4)
                            real_device->WriteWord(real_offset, value);
                        break;
                    case riscv::TRAP_READ:
//                        std::cout << "TRAPPED READ" << std::endl << "Page size: " << size << std::endl
//                                  << "Offset: " << offset << std::endl << "Mode: " << mode << std::endl
//                                  << "Value: " << value << std::endl << std::endl;

                        //Will always read DoubleWord
                        uint64_t mmio_value = real_device->ReadDoubleword(real_offset);

                        page.page().template aligned_write<uint64_t>(offset, mmio_value);
                        break;
                }
            });
        }
    }

    void EmulatorUnit::AddBreakpoint(uint64_t address) {
        breakpoints_.insert(address);
    }

    void EmulatorUnit::RemoveBreakpoint(uint64_t address) {
        breakpoints_.erase(address);
    }

    int EmulatorUnit::GetReturnValue() {
        if (active_machine_ == nullptr) {
            return -1;
        }

        return active_machine_->return_value<long>();
    }
}
