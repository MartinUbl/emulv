//
// Created by xPC on 16.04.2023.
//

#pragma once

#include "PeripheralDevice.h"
#include "Events.h"

namespace modules {

    class ExampleDevice : public PeripheralDevice {
    public:
        void WriteByte(uint64_t address, uint8_t value) override;

        void WriteHalfword(uint64_t address, uint16_t value) override;

        void WriteWord(uint64_t address, uint32_t value) override;

        void WriteDoubleword(uint64_t address, uint64_t value) override;

        uint8_t ReadByte(uint64_t address) override;

        uint16_t ReadHalfword(uint64_t address) override;

        uint32_t ReadWord(uint64_t address) override;

        uint64_t ReadDoubleword(uint64_t address) override;

        uint64_t GetStartAddress();

        uint64_t GetEndAddress();

        ExampleDevice(const std::string &name, uint64_t start_address, uint64_t end_address);
    };

}
