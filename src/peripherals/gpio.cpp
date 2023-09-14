/**
 * Represents implementation of declarations in gpio.h header file related to GPIO peripheral device.
 * 
 * @author Stanislav Kafara
 * @version 2023-04-27
 */


#include <stdexcept>
#include <limits>
#include "gpio.h"
#include "spdlog/spdlog.h"


namespace peripherals {

    GPIO_Port::GPIO_Port(const std::string &name, uint64_t start_address,
                         uint64_t end_address) :
            PeripheralDevice(name, start_address, end_address) {
        Reset();
    }

    void GPIO_Port::WriteByte(uint64_t address, uint8_t value) {
        spdlog::info("Called the WRITE BYTE method of GPIO port to address {0} with value {1}", address, value);
        WriteWord(address, value);
    }

    void GPIO_Port::WriteHalfword(uint64_t address, uint16_t value) {
        spdlog::info("Called the WRITE HALFWORD method of GPIO port to address {0} with value {1}", address, value);
        WriteWord(address, value);
    }

    void GPIO_Port::WriteDoubleword(uint64_t address, uint64_t value) {
        spdlog::info("Called the WRITE DOUBLEWORD method of GPIO port to address {0} with value {1}", address, value);
        if (value > std::numeric_limits<uint32_t>::max())
            throw std::runtime_error("GPIO: Cannot write 64 bit value! Only writes up to 32 bits are supported.");

        WriteWord(address, value);
    }

    uint8_t GPIO_Port::ReadByte(uint64_t address) {
        spdlog::info("Called the READ BYTE method of GPIO port with address {0}", address);
        //GPIO doesn't support reading 8 bits at a time
        throw std::runtime_error("GPIO: Reading a byte is not supported. Only >32 bit read is supported.");
    }

    uint16_t GPIO_Port::ReadHalfword(uint64_t address) {
        spdlog::info("Called the READ HALFWORD method of GPIO port with address {0}", address);
        //GPIO doesn't support reading 16 bits at a time
        throw std::runtime_error("GPIO: Reading a halfword is not supported. Only >32 bit read is supported.");
    }

    uint64_t GPIO_Port::ReadDoubleword(uint64_t address) {
        spdlog::info("Called the READ DOUBLEWORD method of GPIO port with address {0}", address);
        //Will read 32 bit value and return it as a 64 bit value.
        return ReadWord(address);
    }

    void GPIO_Port::WriteWord(uint64_t address, uint32_t value) {
        spdlog::info("Called the WRITE WORD method of GPIO port to address {0} with value {1}", address, value);

        switch (address) {
            case GPIO_Port_Reg_Offset::CTL0:
                Handle_Reg_CTL_Write(Reg_CTL0, 0, value);
                break;

            case GPIO_Port_Reg_Offset::CTL1:
                Handle_Reg_CTL_Write(Reg_CTL1, 1, value);
                break;

            case GPIO_Port_Reg_Offset::OCTL: {
                uint32_t bits = value;
                for (size_t i = 0; i < kReg_Size / 2; i++) {
                    bool isToSet = bits & 0b1;
                    GPIO_Pin_Level previousLevel = Get_Pin_Level(i);
                    Reg_OCTL.set(i, isToSet);
                    GPIO_Pin_Level currentLevel = Get_Pin_Level(i);
                    Announce_Pin_Level_Change(i, previousLevel, currentLevel);
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
                        GPIO_Pin_Level previousLevel = Get_Pin_Level(i);
                        if (isToSet) {
                            Reg_OCTL.set(pinNo);
                        } else {
                            Reg_OCTL.reset(pinNo);
                        }
                        GPIO_Pin_Level currentLevel = Get_Pin_Level(pinNo);
                        Announce_Pin_Level_Change(i, previousLevel, currentLevel);
                    }
                    bits >>= 1;
                }
            }
                break;

            case GPIO_Port_Reg_Offset::BC: {
                uint32_t bits = value;
                for (size_t i = 0; i < kReg_Size / 2; i++) {
                    if (bits & 0b1) {
                        GPIO_Pin_Level previousLevel = Get_Pin_Level(i);
                        Reg_OCTL.reset(i);
                        GPIO_Pin_Level currentLevel = Get_Pin_Level(i);
                        Announce_Pin_Level_Change(i, previousLevel, currentLevel);
                    }
                    bits >>= 1;
                }
            }
                break;

            default: // write to LOCK register or outside GPIO registers memory area
                return;
        }
    }

    uint32_t GPIO_Port::ReadWord(uint64_t address) {
        spdlog::info("Called the READ WORD method of GPIO port with address {0}", address);

        std::bitset<kReg_Size> reg;
        switch (address) {
            case GPIO_Port_Reg_Offset::CTL0:
                reg = Reg_CTL0;
                break;

            case GPIO_Port_Reg_Offset::CTL1:
                reg = Reg_CTL1;
                break;

            case GPIO_Port_Reg_Offset::ISTAT:
                reg = Reg_ISTAT;
                break;

            case GPIO_Port_Reg_Offset::OCTL:
                reg = Reg_OCTL;
                break;

            default:
                return 0;
        }

        return static_cast<uint32_t>(reg.to_ulong());
    }

    void GPIO_Port::Reset() {
        spdlog::info("Resetting registers of GPIO port to default values...");

        Reg_CTL0 = std::bitset<kReg_Size>{kReg_CTL_RESET_VALUE};
        Reg_CTL1 = std::bitset<kReg_Size>{kReg_CTL_RESET_VALUE};
        Reg_ISTAT = std::bitset<kReg_Size>{};
        Reg_OCTL = std::bitset<kReg_Size>{};
    }

    GPIO_Pin_Mode GPIO_Port::Get_Pin_Mode(const size_t pinNo) const {
        // pin control reg CTL0 or CTL1
        // first half of the pins controlled by CTL0, second by CTL1
        const auto regCtl = (pinNo < kReg_CTL_Pin_Count) ? Reg_CTL0 : Reg_CTL1;
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

    GPIO_Pin_Level GPIO_Port::Get_Pin_Level(const size_t pinNo) const {
        if (Get_Pin_Mode(pinNo) == GPIO_Pin_Mode::INPUT) {
            if (Reg_ISTAT[pinNo]) {
                return GPIO_Pin_Level::HIGH;
            } else {
                return GPIO_Pin_Level::LOW;
            }
        } else {
            if (Reg_OCTL[pinNo]) {
                return GPIO_Pin_Level::HIGH;
            } else {
                return GPIO_Pin_Level::LOW;
            }
        }
    }

    void GPIO_Port::Set_Pin_Level(const size_t pinNo, GPIO_Pin_Level level) {
        spdlog::info("Setting GPIO pin {0} to level {1}", pinNo, level);

        if (Get_Pin_Mode(pinNo) == GPIO_Pin_Mode::OUTPUT) {
            return; // cannot set pin output value through interface
        }

        Reg_ISTAT.set(pinNo, level);
    }

    void GPIO_Port::Handle_Reg_CTL_Write(std::bitset<kReg_Size> &reg, const size_t regIndex, const uint32_t value) {
        const size_t pinNoOffset = regIndex * kReg_CTL_Pin_Count;
        for (size_t i = 0; i < kReg_CTL_Pin_Count; i++) {
            size_t offset = kPin_Mode_Bits_Count * i;
            GPIO_Pin_Mode previousMode = Get_Pin_Mode(pinNoOffset + i);
            for (size_t j = 0; j < kPin_Mode_Bits_Count; j++) {
                reg.set(offset + j, (value >> (offset + j)) & 0b1);
            }
            GPIO_Pin_Mode currentMode = Get_Pin_Mode(pinNoOffset + i);
            Announce_Pin_Mode_Change(pinNoOffset + i, previousMode, currentMode);
        }
    }

    void GPIO_Port::Announce_Pin_Level_Change(const size_t pinNo, const GPIO_Pin_Level previousLevel,
                                              const GPIO_Pin_Level currentLevel) const {
        if (currentLevel == previousLevel) {
            return;
        }

        EventsLib::globalEmit("gpio-pin-level-changed", EventsLib::EventData{
                {"gpioPort",      *this},
                {"pinNo",         pinNo},
                {"previousLevel", previousLevel},
                {"currentLevel",  currentLevel}});
    }

    void GPIO_Port::Announce_Pin_Mode_Change(const size_t pinNo, const GPIO_Pin_Mode previousMode,
                                             const GPIO_Pin_Mode currentMode) const {
        if (currentMode == previousMode) {
            return;
        }

        EventsLib::globalEmit("gpio-pin-mode-changed", EventsLib::EventData{
                {"gpioPort",     *this},
                {"pinNo",        pinNo},
                {"previousMode", previousMode},
                {"currentMode",  currentMode}});
    }
}
