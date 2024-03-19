#include <fstream>
#include <iostream>
#include <limits>
#include <filesystem>

#include <libriscv/machine.hpp>
#include "EmulatorUnit.h"
#include "libriscv/rv32i_instr.hpp"
#include "riscv-disas.h"
#include "PeripheralsApi.h"
#include <libriscv/elf.hpp>

namespace emulator {
//##################################################################################################################
//# Core control and initialisation methods
//##################################################################################################################
void EmulatorUnit::Execute(const std::vector<std::string> &machine_arguments) {
    CreateNewMachine_(machine_arguments);

    SetState_(kRunning);
    try {
        spdlog::info("EmulatorUnit is RUNNING and executing a program");

        while (!active_machine_->stopped()) {
            active_machine_->cpu.step_one();
        }
    } catch (const std::exception &e) {
        spdlog::error("A program error has occurred during it's execution: {0}", e.what());
        SetState_(kTerminated);
        return;
    }

    //This will save the last register values
    GetRegisters();

    SetState_(kTerminated);
}

void EmulatorUnit::Terminate() {
    spdlog::info("Emulator terminate method has been called");

    if (active_machine_ == nullptr) {
        return;
    }
    active_machine_->stop();

    SetState_(kTerminated);
}

void EmulatorUnit::LoadElfFile(const std::string &file_path) {
    // Check the file size
    // If bigger than MAX_FILE_SIZE then terminate
    if(std::filesystem::file_size(file_path) > MAX_FILE_SIZE) {
        const std::string& errMsg = std::string("EmulatorUnit::LoadElfFle: Error loading file! Exceeded max allowed size of ") + std::to_string(MAX_FILE_SIZE/ (1024 * 1024)) + " MB.";
        throw std::runtime_error(errMsg);
    }

    // Read the RISC-V program into a std::vector:
    std::ifstream stream(file_path, std::ios::in | std::ios::binary);
    if (!stream) {
        throw std::runtime_error("EmulatorUnit::LoadElfFle: File not found or is inaccessible!");
    }

    auto binary = std::vector<uint8_t>(
        (std::istreambuf_iterator<char>(stream)),
        std::istreambuf_iterator<char>()
        );
    spdlog::info("Potential binary ELF file has been loaded from filesystem");


    //Use this if ValidateElf_() stops working:
    //auto *pMachine = new riscv::Machine<riscv::RISCV64>{binary};
    //delete pMachine;
    ValidateElf_(binary);

    binary_ = binary;
    SetState_(kReady);
}

void EmulatorUnit::SetState_(emulator::EmulatorState state) {
    if (state != state_) {
        state_ = state;
        spdlog::info("Emulator unit's state has changed to: {0}", static_cast<int>(state));
        EventsLib::globalEmit(emulator::State_Changed_Event_Description);
    }
}

emulator::EmulatorState EmulatorUnit::GetState() {
    return state_;
}

//##################################################################################################################
//# Debugger methods
//##################################################################################################################
void EmulatorUnit::Debug(const std::vector<std::string> &machine_arguments) {
    spdlog::info("EmulatorUnit started a program in debug mode");
    CreateNewMachine_(machine_arguments);

    //Run until the first breakpoint is reached
    DebugContinue();
}

void EmulatorUnit::DebugStep() {
    if (active_machine_ == nullptr) {
        throw std::runtime_error("EmulatorUnit::debugStep: active_machine_ is equal to null!");
    }

    if (!active_machine_->stopped()) {
        active_machine_->cpu.step_one();
        spdlog::trace("Debug step has been performed");
    } else {
        SetState_(kTerminated);
    }

}

void EmulatorUnit::DebugContinue() {
    if (active_machine_ == nullptr) {
        throw std::runtime_error("EmulatorUnit::debugStep: active_machine_ is equal to null!");
    }

    try {
        //Handle special case - First line of the program has a breakpoint
        if (GetState() != kDebugPaused && breakpoints_.count(active_machine_->cpu.pc())) {
            SetState_(kDebugPaused);
            return;
        }

        //Run until the next breakpoint is reached
        SetState_(kRunningDebug);
        while (!active_machine_->stopped()) {
            active_machine_->cpu.step_one();
            if (breakpoints_.count(active_machine_->cpu.pc())) {
                SetState_(kDebugPaused);
                spdlog::trace("Debug continue has been performed");
                return;
            }
        }
    } catch (const std::exception &e) {
        spdlog::error("A program error has occurred during the debug execution: {0}", e.what());
        SetState_(kTerminated);
        return;
    }
    //Continues here if the emulated program has exited

    //This will save the last register values
    GetRegisters();
    SetState_(kTerminated);
}

void EmulatorUnit::AddBreakpoint(uint64_t address) {
    spdlog::trace("Breakpoint has been inserted at address: {0}", address);
    breakpoints_.insert(address);
}

void EmulatorUnit::RemoveBreakpoint(uint64_t address) {
    spdlog::trace("Breakpoint has been removed at address: {0}", address);
    breakpoints_.erase(address);
}

void EmulatorUnit::ClearBreakpoints() {
    spdlog::trace("All breakpoints have been created");
    breakpoints_.clear();
}

//##################################################################################################################
//# Memory controller methods
//##################################################################################################################

void EmulatorUnit::RegisterPeripherals(std::map<std::string, std::shared_ptr<peripherals::PeripheralsApi>> &devices) {
    spdlog::info("Peripherals have been registered.");
    peripheral_devices_ = &devices;
}

void EmulatorUnit::SetupMemoryTraps_(riscv::Machine<riscv::RISCV64> &machine) {
    page_peripherals_.clear();

    for (const auto &p: *peripheral_devices_) {
        spdlog::info("Setting up memory trap for device with name: {0}", p.first);

        peripherals::PeripheralsApi *pDevice = p.second.get();
        MapDeviceToPage_(pDevice);

        //Check if the address range isn't too big
        if ((pDevice->getStartAddress() + RISCV_PAGE_SIZE) < pDevice->getEndAddress()) {
            throw std::runtime_error(
                "EmulatorUnit::RegisterPeripheral: The address range of this peripheral device is greater than the maximal supported page size of 4096 bytes.");
        }

        uint64_t TRAP_PAGE = pDevice->getStartAddress();
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
            uint64_t page_start = GetPageStart_(pDevice->getStartAddress());
            uint64_t real_address = page_start + offset;
            peripherals::PeripheralsApi *real_device = GetRealDevice_(real_address);

            if (!real_device) {
                return;
            }

            // Real offset relative to the devices start address
            uint64_t real_offset = real_address - real_device->getStartAddress();

            const size_t size = riscv::Page::trap_size(mode);

            switch (riscv::Page::trap_mode(mode)) {
            case riscv::TRAP_WRITE:
                if (size == 8)
                    real_device->WriteDoubleword(real_offset, value);
                else if (size == 4)
                    real_device->WriteWord(real_offset, value);
                break;
            case riscv::TRAP_READ:
                //Will always read DoubleWord
                uint64_t mmio_value = real_device->ReadDoubleword(real_offset);

                page.page().template aligned_write<uint64_t>(offset, mmio_value);
                break;
            }
        });
    }
}

void EmulatorUnit::MapDeviceToPage_(peripherals::PeripheralsApi *device) {
    uint64_t page_start = GetPageStart_(device->getStartAddress());

    auto entry = page_peripherals_.find(page_start);
    if (entry != page_peripherals_.end()) {
        entry->second.push_back(device);
    } else {
        auto peripherals = std::vector<peripherals::PeripheralsApi *>();
        peripherals.push_back(device);
        page_peripherals_[page_start] = peripherals;
    }
}

peripherals::PeripheralsApi *EmulatorUnit::GetRealDevice_(uint64_t address) {
    uint64_t page_start = GetPageStart_(address);
    auto page_peripherals = this->page_peripherals_[page_start];

    for (auto p: page_peripherals) {
        if (p->getStartAddress() <= address && address <= p->getEndAddress()) {
            return p;
        }
    }
    return nullptr;
}

uint64_t EmulatorUnit::GetPageStart_(uint64_t address) {
    return address & 0xFFFFFFFFFFFFF000;
}

uint64_t EmulatorUnit::GetMemoryStartAddress() {
    if (active_machine_ == nullptr) {
        throw std::runtime_error("EmulatorUnit::getMemoryStartAddress: active_machine_ is equal to null!");
    }
    return active_machine_->memory.start_address();
}

uint64_t EmulatorUnit::GetMemoryEndAddress() {
    if (active_machine_ == nullptr) {
        throw std::runtime_error("EmulatorUnit::getMemoryEndAddress: active_machine_ is equal to null!");
    }

    return active_machine_->memory.exit_address();
}

std::vector<uint8_t> EmulatorUnit::GetMemory(uint64_t from, uint64_t to) {
    std::vector<uint8_t> memory;

    if (active_machine_ == nullptr) {
        //Machine is inactive. Return empty memory.
        return memory;
    }

    //Get memory content from machine.
    for (uint64_t i = from; i <= to; ++i) {
        memory.push_back(active_machine_->memory.read<uint8_t>(i));
    }

    return memory;
}

//##################################################################################################################
//# Utility methods
//##################################################################################################################
std::tuple<std::vector<uint64_t>, std::vector<std::string>> EmulatorUnit::Disassemble() {
    riscv::Machine<riscv::RISCV64> disassembly_machine{binary_};

    disassembly_machine.setup_linux(
        {"myprogram"},
        {"LC_TYPE=C", "LC_ALL=C", "USER=root"});
    disassembly_machine.setup_linux_syscalls();

    // Instruction limit is used to keep running
    disassembly_machine.set_max_instructions(1'000'000UL);

    std::vector<uint64_t> outputAddresses;
    std::vector<std::string> outputText;

    while (!disassembly_machine.stopped()) {
        auto &cpu = disassembly_machine.cpu;

        //Stop the machine when end of executable segment is reached
        if (cpu.pc() == cpu.current_execute_segment().exec_end()) {
            break;
        }

        // Read next instruction
        const auto instruction = cpu.read_next_instruction();

        // Store the disassembled instruction
        outputAddresses.emplace_back(cpu.pc());
        outputText.emplace_back(InstructonToString_(cpu.pc(), instruction));

        // Increment PC to next instruction, and increment instruction counter
        cpu.increment_pc(instruction.length());
        disassembly_machine.increment_counter(1);
    }

    spdlog::info("Loaded ELF has been disassembled successfully");
    return {outputAddresses, outputText};
}

std::string EmulatorUnit::InstructonToString_(uint64_t pc_value, riscv::instruction_format format) {
    char buf[256] = {0};
    disasm_inst(buf, sizeof(buf), rv32, pc_value, (uint16_t) format.whole);

    auto result = std::string(buf);
    return result;
}

//##################################################################################################################
//# Internal helper methods
//##################################################################################################################
void EmulatorUnit::CreateNewMachine_(const std::vector<std::string> &machine_arguments) {
    //Delete old machine
    if (active_machine_ != nullptr) {
        delete active_machine_;
    }

    // Create a new 64-bit RISC-V machine
    active_machine_ = new riscv::Machine<riscv::RISCV64>{binary_};
    // Use string vector as arguments to the RISC-V program
    active_machine_->setup_linux(machine_arguments, {"LC_TYPE=C", "LC_ALL=C", "USER=root"});
    active_machine_->setup_linux_syscalls();
    active_machine_->set_max_instructions(std::numeric_limits<uint64_t>::max());

    // Setup memory traps for peripherals
    if (peripheral_devices_ != nullptr)
        SetupMemoryTraps_(*active_machine_);

    spdlog::trace("A new machine has been created");
}

void EmulatorUnit::ValidateElf_(std::vector<uint8_t> &binary) {
    std::basic_string_view<char> m_binary(reinterpret_cast<char *>(binary.data()), binary.size());

    // Taken from libriscv-src\lib\libriscv\memory.cpp - binary_loader()
    //##################################################################
    if (m_binary.size() < sizeof(typename riscv::Elf<8>::Header)) {
        throw std::runtime_error("ELF program too short");
    }
    if (!riscv::Elf<8>::validate(m_binary)) {
        throw std::runtime_error("Invalid ELF header! Expected a RISC-V ELF binary");
    }

    const auto* elf = (typename riscv::Elf<8>::Header*) m_binary.data();

    static constexpr uint32_t ELFHDR_FLAGS_RVC = 0x1;
    static constexpr uint32_t ELFHDR_FLAGS_RVE = 0x8;
    const bool is_static = elf->e_type == riscv::Elf<8>::Header::ET_EXEC;
    const bool m_is_dynamic = elf->e_type == riscv::Elf<8>::Header::ET_DYN;
    if (!is_static && !m_is_dynamic) {
        throw std::runtime_error("ELF program is not an executable type. Trying to load an object file?");
    }
    if (elf->e_machine != riscv::Elf<8>::Header::EM_RISCV) {
        throw std::runtime_error("ELF program is not a RISC-V executable. Wrong architecture.");
    }
    if ((elf->e_flags & ELFHDR_FLAGS_RVC) != 0 && !riscv::compressed_enabled) {
        throw std::runtime_error("ELF is a RISC-V RVC executable, however C-extension is not enabled.");
    }
    if ((elf->e_flags & ELFHDR_FLAGS_RVE) != 0) {
        throw std::runtime_error("ELF is a RISC-V RVE executable, however E-extension is not supported.");
    }

    // Enumerate & validate loadable segments
    const auto program_headers = elf->e_phnum;
    if (program_headers <= 0) {
        throw std::runtime_error("ELF with no program-headers");
    }
    if (program_headers >= 16) {
        throw std::runtime_error("ELF with too many program-headers");
    }
    if (elf->e_phoff > 0x4000) {
        throw std::runtime_error("ELF program-headers have bogus offset");
    }
    if (elf->e_phoff + program_headers * sizeof(typename riscv::Elf<8>::ProgramHeader) > m_binary.size()) {
        throw std::runtime_error("ELF program-headers are outside the binary");
    }

    spdlog::info("ELF file has been validated successfully, no defects were found");
}

//##################################################################################################################
//# Remaining set + get
//##################################################################################################################

std::vector<std::tuple<std::string, uint32_t>> EmulatorUnit::GetRegisters() {
    if (active_machine_ == nullptr) {
        //Machine is inactive. Return the last saved register values.
        return latest_register_values_;
    }

    std::vector<std::tuple<std::string, uint32_t>> registers;
    latest_register_values_.clear();

    for (int reg_num = 0; reg_num < X_REGISTER_COUNT; reg_num++) {
        std::string reg_prefix = "x";
        registers.emplace_back(reg_prefix + std::to_string(reg_num), active_machine_->cpu.reg(reg_num));
    }
    registers.emplace_back("pc", active_machine_->cpu.pc());

    latest_register_values_ = registers;
    return registers;
}

uint64_t EmulatorUnit::GetPc() {
    if (active_machine_ == nullptr) {
        throw std::runtime_error("EmulatorUnit::getPc: active_machine_ is equal to null!");
    }

    return active_machine_->cpu.pc();
}

int EmulatorUnit::GetReturnValue() {
    if (active_machine_ == nullptr) {
        return -1;
    }

    return active_machine_->return_value<long>();
}

void EmulatorUnit::SetRamStartAddress(uint64_t ramStartAddress) {
    ramStartAddress_ = ramStartAddress;
}

void EmulatorUnit::SetRamSize(uint64_t size) {
    ramSize_ = size;
}

uint64_t EmulatorUnit::GetRamStartAddress() {
    return ramStartAddress_;
}

uint64_t EmulatorUnit::GetRamSize() {
    return ramSize_;
}

uint64_t EmulatorUnit::GetRamEndAddress() {
    return ramStartAddress_ + ramSize_ - 1;
}
}
