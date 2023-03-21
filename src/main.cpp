#include <fstream>
#include <iostream>

#include <libriscv/machine.hpp>

#include "ui/mainwindow.h"
#include <QApplication>


int main(int argc, char **argv) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    if (argc < 2) {
        std::cout << argv[0] << ": [program file] [arguments ...]" << std::endl;
        return EXIT_FAILURE;
    }

    // Read the RISC-V program into a std::vector:
    std::ifstream stream(argv[1], std::ios::in | std::ios::binary);
    if (!stream) {
        std::cout << argv[1] << ": File not found?" << std::endl;
        return -1;
    }
    const std::vector<uint8_t> binary(
            (std::istreambuf_iterator<char>(stream)),
            std::istreambuf_iterator<char>()
    );

    // Take program arguments and make a new string vector, from 1..N
    std::vector<std::string> arguments{argv[1]};
    for (size_t i = 2; i < argc; i++)
        arguments.push_back(argv[i]);

    // Create a new 64-bit RISC-V machine
    riscv::Machine<riscv::RISCV64> machine{binary};

    // Use string vector as arguments to the RISC-V program
    machine.setup_linux(
            arguments,
            {"LC_TYPE=C", "LC_ALL=C", "USER=root"});
    machine.setup_linux_syscalls();

    try {
        // Run the program, but timeout after 128bn instructions
        machine.simulate(128'000'000'000ull);
    } catch (const std::exception &e) {
        std::cout << "Program error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Program exited with status: " << machine.return_value<long>() << std::endl;
    return a.exec();
}
