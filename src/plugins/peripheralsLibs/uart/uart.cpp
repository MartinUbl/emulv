#include <limits>
#include <stdexcept>
#include "uart.h"
//#include "spdlog/spdlog.h"

//##################################################################################################################
//# Dynamic library allocator + deleter
//##################################################################################################################

#ifdef WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

extern "C"
{
DLLEXPORT peripherals::UartDevice *allocator() {
    return new peripherals::UartDevice();
}

DLLEXPORT void deleter(peripherals::UartDevice *ptr) {
    delete ptr;
}
}

//##################################################################################################################
//# UartDevice
//##################################################################################################################
namespace peripherals {

//##################################################################################################################
//# Constructors
//##################################################################################################################

    UartDevice::UartDevice() {
        Name = "uart";
        Reset();
    }

//##################################################################################################################
//# QML and GUI related members
//##################################################################################################################

    QByteArray UartDevice::getQML() {
        QFile file(":/uart/resources/panel.qml");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return QByteArray();
        return file.readAll();
    }

//##################################################################################################################
//# UART state members (PUBLIC)
//##################################################################################################################

    void UartDevice::WriteWord(uint64_t address, uint32_t value) {
        //spdlog::info("Called the WRITE WORD method of UART to address {0} with value {1}", address, value);

        switch (address) {
            case uSTAT:
                Reg_STAT = value;

                break;

            case uDATA:
                Reg_DATA = value;
                _handleDataWrite();
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

    uint32_t UartDevice::ReadWord(uint64_t address) {
        //spdlog::info("Called the READ WORD method of UART with address {0}", address);

        std::bitset<kReg_Size> reg;
        switch (address) {
            case uSTAT:
                reg = Reg_STAT;
                break;

            case uDATA:
                _handleDataRead();
                reg = Reg_DATA;
                break;

            case uBAUD:
                reg = Reg_BAUD;
                break;

            case uCTL0:
                reg = Reg_CTL0;
                //Check if REN bit is enabled (receiver enable bit)
                if (Reg_CTL0.test(2)) {
                    //Push next data from the buffer (if there is any data)
                    if (!_writeBuffer.empty()) {
                        _transmitFrameToDevice(_writeBuffer.front());
                        _writeBuffer.pop();
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

    void UartDevice::WriteByte(uint64_t address, uint8_t value) {
        WriteWord(address, value);
    }

    void UartDevice::WriteHalfword(uint64_t address, uint16_t value) {
        WriteWord(address, value);
    }

    void UartDevice::WriteDoubleword(uint64_t address, uint64_t value) {
        if (value > std::numeric_limits<uint32_t>::max())
            throw std::runtime_error("UART: Cannot write 64 bit value! Only writes up to 32 bits are supported.");

        WriteWord(address, value);
    }

    uint8_t UartDevice::ReadByte(uint64_t address) {
        //spdlog::info("Called the READ BYTE method of UART with address {0}", address);

        //UART doesn't support reading 8 bits at a time
        throw std::runtime_error("UART: Reading a byte is not supported. Only >32 bit read is supported.");
    }

    uint16_t UartDevice::ReadHalfword(uint64_t address) {
        //spdlog::info("Called the READ HALFWORD method of UART with address {0}", address);

        //UART doesn't support reading 16 bits at a time
        throw std::runtime_error("UART: Reading a halfword is not supported. Only >32 bit read is supported.");
    }

    uint64_t UartDevice::ReadDoubleword(uint64_t address) {
        //spdlog::info("Called the READ DOUBLEWORD method of UART with address {0}", address);

        //Will read 32 bit value and return it as a 64 bit value.
        return ReadWord(address);
    }

    void UartDevice::Reset() {
        //spdlog::info("Resetting registers of UART to default values...");

        _displayText = "";
        _sendDisplayTextUpate();
        _writeBuffer = {};

        Reg_STAT = std::bitset<kReg_Size>{kReset_Value_STAT};
        Reg_DATA = std::bitset<kReg_Size>{kReset_Value_OTHER};
        Reg_BAUD = std::bitset<kReg_Size>{kReset_Value_OTHER};
        Reg_CTL0 = std::bitset<kReg_Size>{kReset_Value_OTHER};
        Reg_CTL1 = std::bitset<kReg_Size>{kReset_Value_OTHER};
        Reg_CTL2 = std::bitset<kReg_Size>{kReset_Value_OTHER};
        Reg_GP = std::bitset<kReg_Size>{kReset_Value_OTHER};
    }

    void UartDevice::_handleDataWrite() {
        //Check if TEN bit is enabled (transmitter enable bit)
        if (Reg_CTL0.test(3)) {
            //Handle idle and stop frame?

            //New data has been written to the DATA register

            //Set TBE bit to 0 (transmitter buffer is NOT empty) - program waits until it is set to 1, so it can send more data
            Reg_STAT.set(7, 0);

            //Send a new frame with this data
            _deviceReceivedFrame(Reg_DATA.to_ulong());

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

    void UartDevice::_handleDataRead() {

        //Check if receiver mode is enabled (REN bit)
        if (Reg_CTL0.test(2)) {

            //Push next data from the buffer
            if (!_writeBuffer.empty()) {
                Reg_DATA = _writeBuffer.front();
                _writeBuffer.pop();
            }

            if (_writeBuffer.empty()) {
                //Set the RBNE bit to 0 (read buffer IS empty)
                Reg_STAT.set(5, 0);
            }
        }

    }

    void UartDevice::_deviceReceivedFrame(unsigned long frame_data) {
        //spdlog::info("UART has received a new frame {0}", frame_data);

        //frame_data can contain either 8 bits of data, or 9 bits of data.

        _displayText += static_cast<char>(frame_data);
        // Optimize?
        _sendDisplayTextUpate();
    }

    void UartDevice::_transmitFrameToDevice(uint8_t frame_data) {
        //spdlog::info("UART transmitting (writing to emulator memory) a frame {0}", frame_data);
        WriteWord(uDATA, frame_data);
    }

    void UartDevice::transmitToDevice(QString message) {
        for (QChar c: message) {
            _writeBuffer.emplace(c.toLatin1());
        }

        if (_lineBreak == LF) {
            // LF
            _writeBuffer.emplace(0xA);
        } else if (_lineBreak == CRLF) {
            // CR
            _writeBuffer.emplace(0xD);
            // LF
            _writeBuffer.emplace(0xA);
        }

        //Set RBNE bit to 0 (Read data buffer not empty)
        Reg_STAT.set(5, 1);

    }

    void UartDevice::changeDisplayMode(QString mode) {
        if (mode == "ASCII") {
            _displayMode = ASCII;
        } else if (mode == "HEX") {
            _displayMode = HEX;
        } else if (mode == "DEC") {
            _displayMode = DEC;
        } else if (mode == "BIN") {
            _displayMode = BIN;
        }
        _sendDisplayTextUpate();
    }

    void UartDevice::changeLineBreak(QString lineBreak) {
        if (lineBreak == "NONE") {
            _lineBreak = NONE;
        } else if (lineBreak == "LF") {
            _lineBreak = LF;
        } else if (lineBreak == "CRLF") {
            _lineBreak = CRLF;
        }
    }

    void UartDevice::_sendDisplayTextUpate() {
        switch (_displayMode) {
            case ASCII:
                Q_EMIT displayTextUpdated(_displayText);
                break;
            case HEX:
                Q_EMIT displayTextUpdated(QString::fromStdString(_stringToHex(_displayText.toStdString())));
                break;
            case DEC:
                Q_EMIT displayTextUpdated(QString::fromStdString(_stringToDecimal(_displayText.toStdString())));
                break;
            case BIN:
                Q_EMIT displayTextUpdated(QString::fromStdString(_stringToBinary(_displayText.toStdString())));
                break;
        }
    }

    void UartDevice::clearDisplayText() {
        _displayText = "";
        _sendDisplayTextUpate();
    }
//##################################################################################################################
//# Utilities
//##################################################################################################################

    std::string UartDevice::_stringToHex(const std::string &input) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (size_t i = 0; i < input.length(); ++i) {
            if (input[i] == '\n') {
                ss << "\\n";
            } else {
                ss << std::setw(2) << static_cast<int>(static_cast<unsigned char>(input[i])) << " ";
            }
        }
        return ss.str();
    }

    std::string UartDevice::_stringToBinary(const std::string &input) {
        std::stringstream ss;
        for (size_t i = 0; i < input.length(); ++i) {
            if (input[i] == '\n') {
                ss << "\\n";
            } else {
                ss << std::bitset<8>(input[i]) << " ";
            }
        }
        return ss.str();
    }

    std::string UartDevice::_stringToDecimal(const std::string &input) {
        std::stringstream ss;
        for (size_t i = 0; i < input.length(); ++i) {
            if (input[i] == '\n') {
                ss << "\\n";
            } else {
                ss << static_cast<int>(static_cast<unsigned char>(input[i])) << " ";
            }
        }
        return ss.str();
    }

}


