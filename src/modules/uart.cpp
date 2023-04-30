//
// Created by xPC on 30.04.2023.
//

#include <limits>
#include <stdexcept>
#include "uart.h"

namespace modules {

    UART_Device::UART_Device(const std::string &name, EventEmitter &emitter, uint64_t start_address,
                             uint64_t end_address) : PeripheralDevice(name, emitter, start_address, end_address) {
        Reg_STAT = std::bitset<kReg_Size>{kReset_Value_STAT};
        Reg_DATA = std::bitset<kReg_Size>{kReset_Value_OTHER};
        Reg_BAUD = std::bitset<kReg_Size>{kReset_Value_OTHER};
        Reg_CTL0 = std::bitset<kReg_Size>{kReset_Value_OTHER};
        Reg_CTL1 = std::bitset<kReg_Size>{kReset_Value_OTHER};
        Reg_CTL2 = std::bitset<kReg_Size>{kReset_Value_OTHER};
        Reg_GP = std::bitset<kReg_Size>{kReset_Value_OTHER};
    }

    void UART_Device::WriteWord(uint64_t address, uint32_t value) {
        switch (address) {
            case STAT:
                Reg_STAT = value;

                break;

            case DATA:
                Reg_DATA = value;

                break;

            case BAUD:
                Reg_BAUD = value;

                break;

            case CTL0:
                Reg_CTL0 = value;

                break;

            case CTL1:
                Reg_CTL1 = value;

                break;

            case CTL2:
                Reg_CTL2 = value;

                break;

            case GP:
                Reg_GP = value;

                break;
        }

    }

    uint32_t UART_Device::ReadWord(uint64_t address) {
        std::bitset<kReg_Size> reg;
        switch (address) {
            case STAT:
                reg = Reg_STAT;
                break;

            case DATA:
                reg = Reg_DATA;
                break;

            case BAUD:
                reg = Reg_BAUD;
                break;

            case CTL0:
                reg = Reg_CTL0;
                break;

            case CTL1:
                reg = Reg_CTL1;
                break;

            case CTL2:
                reg = Reg_CTL2;
                break;

            case GP:
                reg = Reg_GP;
                break;

            default:
                return 0;
        }

        return static_cast<uint32_t>(reg.to_ulong());
    }

    void UART_Device::WriteByte(uint64_t address, uint8_t value) {
        WriteWord(address, value);
    }

    void UART_Device::WriteHalfword(uint64_t address, uint16_t value) {
        WriteWord(address, value);
    }

    void UART_Device::WriteDoubleword(uint64_t address, uint64_t value) {
        if (value > std::numeric_limits<uint32_t>::max())
            throw std::runtime_error("UART: Cannot write 64 bit value! Only writes up to 32 bits are supported.");

        WriteWord(address, value);
    }

    uint8_t UART_Device::ReadByte(uint64_t address) {
        //UART doesn't support reading 8 bits at a time
        throw std::runtime_error("UART: Reading a byte is not supported. Only >32 bit read is supported.");
    }

    uint16_t UART_Device::ReadHalfword(uint64_t address) {
        //UART doesn't support reading 16 bits at a time
        throw std::runtime_error("UART: Reading a halfword is not supported. Only >32 bit read is supported.");
    }

    uint64_t UART_Device::ReadDoubleword(uint64_t address) {
        //Will read 32 bit value and return it as a 64 bit value.
        return ReadWord(address);
    }


}
