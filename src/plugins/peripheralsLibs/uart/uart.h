#pragma once

#include <bitset>
#include <queue>
#include "PeripheralsApi.h"
#include <QtQmlIntegration>

namespace peripherals {

//##################################################################################################################
//# Utility classes for UART usage
//##################################################################################################################

enum UartRegistersOffset {
    uSTAT = 0x0, //Status register
    uDATA = 0x4, //Data register
    uBAUD = 0x08, //Baud rate register
    uCTL0 = 0x0C, //Control register 0
    uCTL1 = 0x10, //Control register 1
    uCTL2 = 0x14, //Control register 2
    uGP = 0x18 //Guard time and prescaler register
};

enum DisplayMode {
    ASCII,
    HEX,
    DEC,
    BIN
};

enum LineBreak {
    NONE,
    LF,
    CRLF
};

//##################################################################################################################
//# Core UART class
//##################################################################################################################

class UartDevice : public PeripheralsApi {
    Q_OBJECT
    QML_ELEMENT

public:

    //##################################################################################################################
    //# Constructors + destructors
    //##################################################################################################################

    UartDevice();

    //##################################################################################################################
    //# QML and GUI related members
    //##################################################################################################################

    QByteArray getQML() override;

    //##################################################################################################################
    //# UART state members (PUBLIC)
    //##################################################################################################################

    // Interface - PeripheralDevice
    virtual void WriteByte(uint64_t address, uint8_t value) override;

    virtual void WriteHalfword(uint64_t address, uint16_t value) override;

    /** Handles writing to UART port registers. */
    virtual void WriteWord(uint64_t address, uint32_t value) override;

    virtual void WriteDoubleword(uint64_t address, uint64_t value) override;

    virtual uint8_t ReadByte(uint64_t address) override;

    virtual uint16_t ReadHalfword(uint64_t address) override;

    /** Handles reading from UART port registers. */
    virtual uint32_t ReadWord(uint64_t address) override;

    virtual uint64_t ReadDoubleword(uint64_t address) override;

    virtual void Reset() override;

    //##################################################################################################################
    //# signals + slots
    //##################################################################################################################
public Q_SLOTS:
    void transmitToDevice(QString message);
    void changeDisplayMode(QString mode);
    void changeLineBreak(QString lineBreak);
    void clearDisplayText();
Q_SIGNALS:
    void displayTextUpdated(QString displayText);

private:
    //##################################################################################################################
    //# UART internal state members (PRIVATE)
    //##################################################################################################################

    static constexpr size_t kReg_Size = 32;
    static constexpr size_t kReset_Value_STAT = 0x000000C0;
    static constexpr size_t kReset_Value_OTHER = 0x00000000;

    /* Status register */
    std::bitset<kReg_Size> Reg_STAT;
    /* Data register */
    std::bitset<kReg_Size> Reg_DATA;
    /* Baud rate register */
    std::bitset<kReg_Size> Reg_BAUD;
    /* Control Register 0 */
    std::bitset<kReg_Size> Reg_CTL0;
    /* Control Register 1 */
    std::bitset<kReg_Size> Reg_CTL1;
    /* Control Register 2 */
    std::bitset<kReg_Size> Reg_CTL2;
    /* Guard time and prescaler register */
    std::bitset<kReg_Size> Reg_GP;

    void _handleDataWrite();

    void _handleDataRead();

    void _deviceReceivedFrame(unsigned long frame_data);

    void _transmitFrameToDevice(uint8_t frame_data);

    std::queue<uint32_t> _writeBuffer;

    // Members associated with displaying text in GUI
    QString _displayText;
    DisplayMode _displayMode = ASCII;
    LineBreak _lineBreak = NONE;
    void _sendDisplayTextUpate();

    std::string _stringToHex(const std::string &input);
    std::string _stringToBinary(const std::string &input);
    std::string _stringToDecimal(const std::string &input);
};


}
