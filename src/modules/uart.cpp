//
// Created by xPC on 30.04.2023.
//

#include <limits>
#include <stdexcept>
#include <iostream>
#include "uart.h"
#include "../utils/events/uart_event.h"

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
            case uSTAT:
                Reg_STAT = value;

                break;

            case uDATA:
                Reg_DATA = value;
                HandleDataWrite();
                break;

            case uBAUD:
                Reg_BAUD = value;

                break;

            case uCTL0:
                Reg_CTL0 = value;

                break;

            case uCTL1:
                Reg_CTL1 = value;

                break;

            case uCTL2:
                Reg_CTL2 = value;

                break;

            case uGP:
                Reg_GP = value;

                break;
        }

    }

    uint32_t UART_Device::ReadWord(uint64_t address) {
        std::bitset<kReg_Size> reg;
        switch (address) {
            case uSTAT:
                reg = Reg_STAT;
                break;

            case uDATA:
                reg = Reg_DATA;
                HandleDataRead();
                break;

            case uBAUD:
                reg = Reg_BAUD;
                break;

            case uCTL0:
                reg = Reg_CTL0;
                //Check if REN bit is enabled (receiver enable bit)
                if (Reg_CTL0.test(2)) {
                    //Push next data from the buffer (if there is any data)
                    if (!write_buffer.empty()) {
                        TransmitFrameToDevice(write_buffer.front());
                        write_buffer.pop();
                    }
                }
                break;

            case uCTL1:
                reg = Reg_CTL1;
                break;

            case uCTL2:
                reg = Reg_CTL2;
                break;

            case uGP:
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

    void UART_Device::HandleDataWrite() {
        //Check if TEN bit is enabled (transmitter enable bit)
        if (Reg_CTL0.test(3)) {
            //Handle idle and stop frame?

            //New data has been written to the DATA register

            //Set TBE bit to 0 (transmitter buffer is NOT empty) - program waits until it is set to 1, so it can send more data
            Reg_STAT.set(7, 0);

            //Send a new frame with this data
            DeviceReceivedFrame(Reg_DATA.to_ulong());

            //Set the TC bit to signal that transmission of frame has been completed
            Reg_STAT.set(6, 1);
            //Set the TBE bit to 1 (transmitter buffer IS empty) - program can write new data
            Reg_STAT.set(7, 1);

            return;
        }

        //Check if REN bit is enabled (receiver enable bit)
        if (Reg_CTL0.test(2)) {
            //Set the RBNE bit to 1 (read buffer is NOT empty) - will allow program to read DATA register
            Reg_STAT.set(5, 1);
            return;
        }

    }

    void UART_Device::HandleDataRead() {

        //Check if receiver mode is enabled (REN bit)
        if (Reg_CTL0.test(2)) {
            //Set the RBNE bit to 0 (read buffer IS empty)
            Reg_STAT.set(5, 0);

            //Push next data from the buffer
            if (!write_buffer.empty()) {
                TransmitFrameToDevice(write_buffer.front());
                write_buffer.pop();
            }
        }

    }

    void UART_Device::DeviceReceivedFrame(unsigned long frame_data) {
        //frame_data can contain either 8 bits of data, or 9 bits of data.
        Emitter.Emit(UART_event_description, new uart_event(*this, frame_data));
    }

    void UART_Device::TransmitFrameToDevice(uint8_t frame_data) {
        WriteWord(uDATA, frame_data);
    }

    void UART_Device::TransmitToDevice(std::string message) {
        for (char c: message) {
            write_buffer.emplace(c);
        }

        //Check if REN bit is enabled (receiver enable bit)
        if (Reg_CTL0.test(2)) {
            TransmitFrameToDevice(write_buffer.front());
            write_buffer.pop();
        }

    }

}
