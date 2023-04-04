//
// Created by xPC on 04.04.2023.
//

#ifndef EMULV_PERIPHERALDEVICE_H
#define EMULV_PERIPHERALDEVICE_H

#include <cstdint>

class PeripheralDevice {
public:
    virtual void WriteByte(uint64_t address, uint8_t value) = 0;
    virtual void WriteHalfword(uint64_t address, uint16_t value) = 0;
    virtual void WriteWord(uint64_t address, uint32_t value) = 0;
    virtual void WriteDoubleword(uint64_t address, uint64_t value) = 0;

    virtual uint8_t ReadByte(uint64_t address) = 0;
    virtual uint16_t ReadHalfword(uint64_t address) = 0;
    virtual uint32_t ReadWord(uint64_t address) = 0;
    virtual uint64_t ReadDoubleword(uint64_t address) = 0;
};
#endif //EMULV_PERIPHERALDEVICE_H
