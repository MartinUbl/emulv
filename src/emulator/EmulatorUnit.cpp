#include <fstream>
#include <iostream>

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
            throw std::runtime_error("LoadElfFle: File not found or is inaccessible!");
        }

        binary_ = std::vector<uint8_t>(
                (std::istreambuf_iterator<char>(stream)),
                std::istreambuf_iterator<char>()
        );

    }

    /**
     * Execute the loaded ELF file.
     * @param machine_arguments Arguments of the program
     * @return Exit code of the program
     */
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

        return machine.return_value<long>();
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


}
