//
// Created by xPC on 16.04.2023.
//

#include <iostream>
#include "ExampleDevice.h"

namespace modules {

    void ExampleDevice::WriteHalfword(uint64_t address, uint16_t value) {
        std::cout << "Writing half word to address: " << address << " with value: " << value  << std::endl;
    }

    void ExampleDevice::WriteByte(uint64_t address, uint8_t value) {
        std::cout << "Writing byte to address: " << address << " with value: " << value  << std::endl;

    }

    void ExampleDevice::WriteWord(uint64_t address, uint32_t value) {
        std::cout << "Writing word to address: " << address << " with value: " << value  << std::endl;

    }

    void ExampleDevice::WriteDoubleword(uint64_t address, uint64_t value) {
        std::cout << "Writing double word to address: " << address << " with value: " << value  << std::endl;

    }

    uint8_t ExampleDevice::ReadByte(uint64_t address) {
        std::cout << "Reading byte from address: " << address << std::endl;
        return 123;
    }

    uint16_t ExampleDevice::ReadHalfword(uint64_t address) {
        std::cout << "Reading half word from address: " << address << std::endl;

        return 123;
    }

    uint32_t ExampleDevice::ReadWord(uint64_t address) {
        std::cout << "Reading word from address: " << address << std::endl;
        return 123;
    }

    uint64_t ExampleDevice::ReadDoubleword(uint64_t address) {
        std::cout << "Reading double word from address: " << address << std::endl;
        return 123;
    }

    ExampleDevice::ExampleDevice(const std::string &name, uint64_t start_address, uint64_t end_address) :
        PeripheralDevice(name, start_address, end_address)
    {}

}
