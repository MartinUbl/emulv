#include <stdexcept>
#include <limits>
#include "gpio.h"
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
DLLEXPORT peripherals::GpioController *allocator() {
    return new peripherals::GpioController();
}

DLLEXPORT void deleter(peripherals::GpioController *ptr) {
    delete ptr;
}
}


//##################################################################################################################
//# GpioController
//##################################################################################################################
namespace peripherals {

//##################################################################################################################
//# Constructors
//##################################################################################################################

GpioController::GpioController() {
    Name = "gpio";
}

//##################################################################################################################
//# QML and GUI related members
//##################################################################################################################

QByteArray GpioController::getQML() {
    QFile file(":/gpio/resources/panel.qml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QByteArray();
    return file.readAll();
}

//##################################################################################################################
//# GPIO state members (PUBLIC)
//##################################################################################################################

void GpioController::WriteByte(uint64_t address, uint8_t value) {
    //spdlog::info("Called the WRITE BYTE method of GPIO port to address {0} with value {1}", address, value);

    WriteWord(address, value);
}

void GpioController::WriteHalfword(uint64_t address, uint16_t value) {
    //spdlog::info("Called the WRITE HALFWORD method of GPIO port to address {0} with value {1}", address, value);

    WriteWord(address, value);
}

void GpioController::WriteDoubleword(uint64_t address, uint64_t value) {
    //spdlog::info("Called the WRITE DOUBLEWORD method of GPIO port to address {0} with value {1}", address, value);

    if (value > std::numeric_limits<uint32_t>::max())
        throw std::runtime_error("GPIO: Cannot write 64 bit value! Only writes up to 32 bits are supported.");

    WriteWord(address, value);
}

uint8_t GpioController::ReadByte(uint64_t address) {
    //spdlog::info("Called the READ BYTE method of GPIO port with address {0}", address);

    //GPIO doesn't support reading 8 bits at a time
    throw std::runtime_error("GPIO: Reading a byte is not supported. Only >32 bit read is supported.");
}

uint16_t GpioController::ReadHalfword(uint64_t address) {
    //spdlog::info("Called the READ HALFWORD method of GPIO port with address {0}", address);

    //GPIO doesn't support reading 16 bits at a time
    throw std::runtime_error("GPIO: Reading a halfword is not supported. Only >32 bit read is supported.");
}

uint64_t GpioController::ReadDoubleword(uint64_t address) {
    //spdlog::info("Called the READ DOUBLEWORD method of GPIO port with address {0}", address);

    //Will read 32 bit value and return it as a 64 bit value.
    return ReadWord(address);
}

void GpioController::WriteWord(uint64_t address, uint32_t value) {
    //spdlog::info("Called the WRITE WORD method of GPIO port to address {0} with value {1}", address, value);

    switch (address) {
    case GPIO_Port_Reg_Offset::CTL0:
        _handle_Reg_CTL_Write(_reg_CTL0, 0, value);
        break;

    case GPIO_Port_Reg_Offset::CTL1:
        _handle_Reg_CTL_Write(_reg_CTL1, 1, value);
        break;

    case GPIO_Port_Reg_Offset::OCTL: {
        uint32_t bits = value;
        for (size_t i = 0; i < kReg_Size / 2; i++) {
            bool isToSet = bits & 0b1;
            GPIO_Pin_Level previousLevel = getPinLevel(i);

            _reg_OCTL.set(i, isToSet);
            GPIO_Pin_Level currentLevel = getPinLevel(i);

            if(previousLevel != currentLevel)
                Q_EMIT pinStateChanged(i, currentLevel, getPinMode(i));
            bits >>= 1;
        }
    }
    break;

    case GPIO_Port_Reg_Offset::BOP: {
        uint32_t bits = value;
        for (size_t i = 0; i < kReg_Size; i++) {
            if (bits & 0b1) {
                bool isToSet = i < kReg_Size / 2;
                size_t pinNo = (isToSet) ? i : i - kReg_Size / 2;
                GPIO_Pin_Level previousLevel = getPinLevel(i);

                if (isToSet) {
                    _reg_OCTL.set(pinNo);
                } else {
                    _reg_OCTL.reset(pinNo);
                }
                GPIO_Pin_Level currentLevel = getPinLevel(pinNo);

                if(previousLevel != currentLevel)
                    Q_EMIT pinStateChanged(i, currentLevel, getPinMode(i));
            }
            bits >>= 1;
        }
    }
    break;

    case GPIO_Port_Reg_Offset::BC: {
        uint32_t bits = value;
        for (size_t i = 0; i < kReg_Size / 2; i++) {
            if (bits & 0b1) {
                GPIO_Pin_Level previousLevel = getPinLevel(i);

                _reg_OCTL.reset(i);
                GPIO_Pin_Level currentLevel = getPinLevel(i);

                if(previousLevel != currentLevel)
                    Q_EMIT pinStateChanged(i, currentLevel, getPinMode(i));
            }
            bits >>= 1;
        }
    }
    break;

    default: // write to LOCK register or outside GPIO registers memory area
        return;
    }
}

uint32_t GpioController::ReadWord(uint64_t address) {
    //spdlog::info("Called the READ WORD method of GPIO port with address {0}", address);

    std::bitset<kReg_Size> reg;
    switch (address) {
    case GPIO_Port_Reg_Offset::CTL0:
        reg = _reg_CTL0;
        break;

    case GPIO_Port_Reg_Offset::CTL1:
        reg = _reg_CTL1;
        break;

    case GPIO_Port_Reg_Offset::ISTAT:
        reg = _reg_ISTAT;
        break;

    case GPIO_Port_Reg_Offset::OCTL:
        reg = _reg_OCTL;
        break;

    default:
        return 0;
    }

    return static_cast<uint32_t>(reg.to_ulong());
}

void GpioController::Reset() {
    //spdlog::info("Resetting registers of GPIO port to default values...");

    _reg_CTL0 = std::bitset<kReg_Size>{kReg_CTL_RESET_VALUE};
    _reg_CTL1 = std::bitset<kReg_Size>{kReg_CTL_RESET_VALUE};
    _reg_ISTAT = std::bitset<kReg_Size>{};
    _reg_OCTL = std::bitset<kReg_Size>{};

    // Reset all pins in GUI
    for (int pinNo = 0; pinNo < kPin_Count; ++pinNo) {
        Q_EMIT pinStateChanged(pinNo, false, false);
    }
}

//##################################################################################################################
//# GPIO internal state members (PRIVATE)
//##################################################################################################################

void GpioController::_handle_Reg_CTL_Write(std::bitset<kReg_Size> &reg, const size_t regIndex, const uint32_t value) {
    const size_t pinNoOffset = regIndex * kReg_CTL_Pin_Count;

    for (size_t i = 0; i < kReg_CTL_Pin_Count; i++) {
        size_t offset = kPin_Mode_Bits_Count * i;
        GPIO_Pin_Mode previousMode = getPinMode(pinNoOffset + i);

        for (size_t j = 0; j < kPin_Mode_Bits_Count; j++) {
            reg.set(offset + j, (value >> (offset + j)) & 0b1);
        }

        GPIO_Pin_Mode currentMode = getPinMode(pinNoOffset + i);
        GPIO_Pin_Level currentLevel = getPinLevel(pinNoOffset + i);

        if(previousMode != currentMode)
            Q_EMIT pinStateChanged(pinNoOffset + i, currentLevel, currentMode);
    }
}

//##################################################################################################################
//# Get + Set
//##################################################################################################################

GPIO_Pin_Mode GpioController::getPinMode(const size_t pinNo) const {
    // pin control reg CTL0 or CTL1
    // first half of the pins controlled by CTL0, second by CTL1
    const auto regCtl = (pinNo < kReg_CTL_Pin_Count) ? _reg_CTL0 : _reg_CTL1;
    // bit offset to MDi
    // CTL(0,1) - CTLz[1:0] MDz[1:0] CTLy[1:0] MDy[1:0] ... CTLx[1:0] MDx[1:0]
    const size_t bitOff = (pinNo % kReg_CTL_Pin_Count) * 4;

    // MDi[1:0] == 00 => INPUT
    if (!regCtl[bitOff] && !regCtl[bitOff + 1]) {
        return GPIO_Pin_Mode::INPUT;
    }
    // MDi[1:0] == 01|10|11 => OUTPUT
    else {
        return GPIO_Pin_Mode::OUTPUT;
    }
}

GPIO_Pin_Level GpioController::getPinLevel(const size_t pinNo) const {
    if (getPinMode(pinNo) == GPIO_Pin_Mode::INPUT) {
        if (_reg_ISTAT[pinNo]) {
            return GPIO_Pin_Level::HIGH;
        } else {
            return GPIO_Pin_Level::LOW;
        }
    } else {
        if (_reg_OCTL[pinNo]) {
            return GPIO_Pin_Level::HIGH;
        } else {
            return GPIO_Pin_Level::LOW;
        }
    }
}

void GpioController::setPinLevel(const int pinNo, GPIO_Pin_Level level) {
    //spdlog::info("Setting GPIO pin {0} to level {1}", pinNo, level);

    if (getPinMode(pinNo) == GPIO_Pin_Mode::OUTPUT) {
        return; // cannot set pin output value through interface
    }

    _reg_ISTAT.set(pinNo, level);

    Q_EMIT pinStateChanged(pinNo, level, getPinMode(pinNo));
}

void GpioController::togglePinLevel(const int pinNo)
{
    setPinLevel(pinNo, (getPinLevel(pinNo) == HIGH) ? LOW : HIGH);
}

}
