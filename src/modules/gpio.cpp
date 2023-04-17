/**
 * Represents implementation of declarations in gpio.h header file related to GPIO peripheral device.
 * 
 * @author Stanislav Kafara
 * @version 2023-04-18
 */


#include "gpio.h"


namespace modules {

    GPIO_Port::GPIO_Port(EventEmitter &emitter, uint64_t start_address, uint64_t end_address) :
        PeripheralDevice(emitter, start_address, end_address)
    {
        Reg_CTL0 = std::bitset<kReg_Size> {kReg_CTL_RESET_VALUE};
        Reg_CTL1 = std::bitset<kReg_Size> {kReg_CTL_RESET_VALUE};
    }

    void GPIO_Port::WriteByte(uint64_t address, uint8_t value) {
        // Not Implemented
    }

    void GPIO_Port::WriteHalfword(uint64_t address, uint16_t value) {
        // Not Implemented
    }

    void GPIO_Port::WriteDoubleword(uint64_t address, uint64_t value) {
        // Not Implemented
    }

    uint8_t GPIO_Port::ReadByte(uint64_t address) {
        // Not Implemented
        return 0;
    }

    uint16_t GPIO_Port::ReadHalfword(uint64_t address) {
        // Not Implemented
        return 0;
    }

    uint64_t GPIO_Port::ReadDoubleword(uint64_t address) {
        // Not Implemented
        return 0;
    }

    void GPIO_Port::WriteWord(uint64_t address, uint32_t value) {
        switch (address) {
            case GPIO_Port_Reg_Offset::CTL0:
                Reg_CTL0 = std::bitset<kReg_Size> {value};
                break;
            
            case GPIO_Port_Reg_Offset::CTL1:
                Reg_CTL1 = std::bitset<kReg_Size> {value};
                break;

            case GPIO_Port_Reg_Offset::OCTL:
                Reg_OCTL = std::bitset<kReg_Size> {value};
                break;

            case GPIO_Port_Reg_Offset::BOP: {
                uint32_t bits = value;
                size_t i = 0;
                for (; i < kReg_Size / 2; i++) {
                    if (bits & 0b1) {
                        Reg_OCTL.set(i);
                    }
                    bits >>= 1;
                }
                for (; i < kReg_Size; i++) {
                    if (bits & 0b1) {
                        Reg_OCTL.reset(i);
                    }
                    bits >>= 1;
                }
            }
                break;

            case GPIO_Port_Reg_Offset::BC: {
                uint32_t bits = value;
                for (size_t i = 0; i < kReg_Size / 2; i++) {
                    if (bits & 0b1) {
                        Reg_OCTL.reset(i);
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

    GPIO_Pin_Mode GPIO_Port::Get_Pin_Mode(const size_t pinNo) const {
        // pin control reg CTL0 or CTL1
        // first half of the pins controlled by CTL0, second by CTL1
        const auto regCtl = (pinNo < (kPin_Count / 2)) ? Reg_CTL0 : Reg_CTL1;
        // bit offset to MDi
        // CTL(0,1) - CTLz[1:0] MDz[1:0] CTLy[1:0] MDy[1:0] ... CTLx[1:0] MDx[1:0]
        const size_t bitOff = (pinNo % (kPin_Count / 2)) * 4;

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
            }
            else {
                return GPIO_Pin_Level::LOW;
            }
        }
        else {
            if (Reg_OCTL[pinNo]) {
                return GPIO_Pin_Level::HIGH;
            }
            else {
                return GPIO_Pin_Level::LOW;
            }
        }
    }

    void GPIO_Port::Set_Pin_Level(const size_t pinNo, GPIO_Pin_Level level) {
        if (Get_Pin_Mode(pinNo) == GPIO_Pin_Mode::OUTPUT) {
            return; // cannot set pin output value through interface
        }

        Reg_ISTAT.set(pinNo, level);
    }
    
}
