#include <fstream>
#include <iostream>

#include <libriscv/machine.hpp>

#include "EmulatorUnit.h"
#include "libriscv/rv32i_instr.hpp"
#include "riscv-disas.h"

namespace emulator {

    void EmulatorUnit::LoadElfFile(const std::string &file_path) {
        // Read the RISC-V program into a std::vector:
        std::ifstream stream(file_path, std::ios::in | std::ios::binary);
        if (!stream) {
            throw std::runtime_error("LoadElfFle: File not found or is inaccessible!");
        }

        binary_ = std::vector<uint8_t>(
                (std::istreambuf_iterator<char>(stream)),
                std::istreambuf_iterator<char>()
        );

    }

    int EmulatorUnit::Execute(const std::vector<std::string> &machine_arguments) {
        // Create a new 64-bit RISC-V machine
        riscv::Machine<riscv::RISCV64> machine{binary_};

        // Use string vector as arguments to the RISC-V program
        machine.setup_linux(
                machine_arguments,
                {"LC_TYPE=C", "LC_ALL=C", "USER=root"});
        machine.setup_linux_syscalls();

        try {
            // Run the program, but timeout after 128bn instructions
            machine.simulate(128'000'000'000uLL);
        } catch (const std::exception &e) {
            std::cout << "Program error: " << e.what() << std::endl;
            return EXIT_FAILURE;
        }

        std::cout << "Program exited with status: " << machine.return_value<long>() << std::endl;
        return 0;
    }

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
            output.push_back(InstructonToString_(cpu, instruction));

            // Increment PC to next instruction, and increment instruction counter
            cpu.increment_pc(instruction.length());
            machine.increment_counter(1);
        }

        return output;
    }

    std::string EmulatorUnit::InstructonToString_(riscv::CPU<8> const &cpu, riscv::instruction_format format) {
        char buf[256] = { 0 };
        disasm_inst(buf, sizeof(buf), rv32, cpu.pc(), (uint16_t) format.whole);

        auto result = std::string(buf);
        return result;
    }

}
