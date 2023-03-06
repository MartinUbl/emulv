//
// Created by xPC on 05.03.2023.
//

#include "EmulatorUnit.h"
#include <libriscv/machine.hpp>
#include <iostream>
#include <fstream>

using namespace riscv;

void EmulatorUnit::LoadElfFile(const std::string& file_path) {
    // Read the RISC-V program into a std::vector:
    std::ifstream stream(file_path, std::ios::in | std::ios::binary);
    if (!stream) {
        std::cout << file_path << ": File not found?" << std::endl;
        return; //TODO: Throw exception
    }

    binary_ = std::vector<uint8_t>(
            (std::istreambuf_iterator<char>(stream)),
            std::istreambuf_iterator<char>()
    );
}

int EmulatorUnit::Execute(const std::vector<std::string>& machine_arguments) {
    // Create a new 64-bit RISC-V machine
    Machine<RISCV64> machine{binary_};

    // Use string vector as arguments to the RISC-V program
    machine.setup_linux(
            machine_arguments,
            {"LC_TYPE=C", "LC_ALL=C", "USER=root"});
    machine.setup_linux_syscalls();

    try {
        // Run the program, but timeout after 128bn instructions
        machine.simulate(128'000'000'000uLL);
    } catch (const std::exception& e) {
        std::cout << "Program error: " << e.what() << std::endl;
        return -1;
    }

    std::cout << "Program exited with status: " << machine.return_value<long>() << std::endl;
    return 0;
}
