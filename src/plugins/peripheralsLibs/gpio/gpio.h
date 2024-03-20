#pragma once

#include <cstddef>
#include <bitset>
#include <QtQmlIntegration>

#include "PeripheralsApi.h"

namespace peripherals {

//##################################################################################################################
//# Utility classes for GPIO usage
//##################################################################################################################

enum GPIO_Pin_Mode {
    INPUT,
    OUTPUT
};

enum GPIO_Pin_Level : bool {
    LOW = 0,
    HIGH = 1
};

/**
* Represents GPIO registers address offsets.
*/
enum GPIO_Port_Reg_Offset : size_t {
    CTL0 = 0, /** Port Control Register 0 */
    CTL1 = 4, /** Port Control Register 1 */
    ISTAT = 8, /** Port Input Status Register */
    OCTL = 12, /** Port Output Control Register */
    BOP = 16, /** Port Bit Operate Register */
    BC = 20, /** Port Bit Clear Register */
    LOCK = 24 /** Port Configuration Lock Register (not used) */
};

//##################################################################################################################
//# GPIO port class
//##################################################################################################################

class GpioPort {
public:
    //##################################################################################################################
    //# Get + Set
    //##################################################################################################################
    GPIO_Pin_Mode getPinMode(const size_t pinNo) const;
    GPIO_Pin_Level getPinLevel(const size_t pinNo) const;
    /** Sets (input) pin level. */
    void setPinLevel(const int pinNo, GPIO_Pin_Level level);


private:
    //##################################################################################################################
    //# GPIO internal state members (PRIVATE)
    //##################################################################################################################

    static constexpr size_t kPin_Count = 16;
    static constexpr size_t kReg_Size = 32;
    static constexpr uint32_t kReg_CTL_RESET_VALUE = 0x44444444;
    static constexpr size_t kReg_CTL_Pin_Count = kPin_Count / 2;
    static constexpr size_t kPin_Mode_Bits_Count = kReg_Size / kReg_CTL_Pin_Count;

    /** Port Control Register 0 */
    std::bitset<kReg_Size> _reg_CTL0;
    /** Port Control Register 1 */
    std::bitset<kReg_Size> _reg_CTL1;
    /** Port Input Status Register */
    std::bitset<kReg_Size> _reg_ISTAT;
    /** Port Output Control Register */
    std::bitset<kReg_Size> _reg_OCTL;

    void _handle_Reg_CTL_Write(std::bitset<kReg_Size> &reg, const size_t regIndex, const uint32_t value);

};

//##################################################################################################################
//# GPIO controller class
//##################################################################################################################

/**
* GPIO Controller consists of several GPIO ports
*/
class GpioController : public PeripheralsApi {
    Q_OBJECT

    QML_ELEMENT

public:
    //##################################################################################################################
    //# Constructors + destructors
    //##################################################################################################################

    GpioController();

    //##################################################################################################################
    //# QML and GUI related members
    //##################################################################################################################

    QByteArray getQML() override;

    //##################################################################################################################
    //# GPIO state members (PUBLIC)
    //##################################################################################################################

    // Interface - PeripheralDevice
    virtual void WriteByte(uint64_t address, uint8_t value) override;

    virtual void WriteHalfword(uint64_t address, uint16_t value) override;

    /** Handles writing to GPIO port registers. */
    virtual void WriteWord(uint64_t address, uint32_t value) override;

    virtual void WriteDoubleword(uint64_t address, uint64_t value) override;

    virtual uint8_t ReadByte(uint64_t address) override;

    virtual uint16_t ReadHalfword(uint64_t address) override;

    /** Handles reading from GPIO port registers. */
    virtual uint32_t ReadWord(uint64_t address) override;

    virtual uint64_t ReadDoubleword(uint64_t address) override;

    virtual void Reset() override;

    //##################################################################################################################
    //# Slots + Signals
    //##################################################################################################################
public Q_SLOTS:
    void togglePinLevel(const int pinNo);

Q_SIGNALS:
    void pinStateChanged(const int pinNo, const bool currentLevel, const bool currentMode);

};

}
