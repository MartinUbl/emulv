#include <fstream>
#include <iostream>
#include <libriscv/machine.hpp>
#include "EmulatorUnit.h"

using namespace riscv;

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << argv[0] << ": [program file] [arguments ...]" << std::endl;
		return -1;
	}

	// Take program arguments and make a new string vector, from 1..N
	std::vector<std::string> arguments { argv[1] };
	for (size_t i = 2; i < argc; i++)
		arguments.push_back(argv[i]);

    EmulatorUnit em{};
    em.LoadElfFile(argv[1]);
    em.Execute(arguments);

}
