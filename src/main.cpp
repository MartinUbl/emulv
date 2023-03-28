#include <fstream>
#include <iostream>

#include <libriscv/machine.hpp>

#include "emulator/EmulatorUnit.h"
#include "ui/mainwindow.h"
#include <QApplication>


int main(int argc, char **argv) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    emulator::EmulatorUnit e;
    e.LoadElfFile(argv[1]);

    const std::vector<std::string> &disassembled_elf = e.Disassemble();
    for (const std::string& instruction_string: disassembled_elf) {
        std::cout << instruction_string << std::endl;
    }

    std::cout << std::endl << "Running program: " << std::endl;
    std::vector<std::string> arguments{argv[1]};
    for (size_t i = 2; i < argc; i++)
        arguments.push_back(argv[i]);

    e.Execute(arguments);

    return a.exec();
}

