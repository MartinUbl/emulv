#include <fstream>
#include <iostream>

#include <libriscv/machine.hpp>

#include "emulator/EmulatorUnit.h"
#include "ui/Controller.h"

int main(int argc, char **argv) {
    Controller c(argc,argv);

    return c.ShowWindow();
}

