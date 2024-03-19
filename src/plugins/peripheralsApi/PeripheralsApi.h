#pragma once

#include <cstdint>
#include <string>
#include <QByteArray>
#include <QObject>

namespace peripherals {
    class PeripheralsApi : public QObject {

    public:

        //##################################################################################################################
        //# Constructors + destructors
        //##################################################################################################################

        virtual ~PeripheralsApi() = default;

        //##################################################################################################################
        //# QML and GUI related members
        //##################################################################################################################

        virtual QByteArray getQML() = 0;

        //##################################################################################################################
        //# Peripheral device internal state members
        //##################################################################################################################

        virtual void WriteByte(uint64_t address, uint8_t value) = 0;

        virtual void WriteHalfword(uint64_t address, uint16_t value) = 0;

        virtual void WriteWord(uint64_t address, uint32_t value) = 0;

        virtual void WriteDoubleword(uint64_t address, uint64_t value) = 0;

        virtual uint8_t ReadByte(uint64_t address) = 0;

        virtual uint16_t ReadHalfword(uint64_t address) = 0;

        virtual uint32_t ReadWord(uint64_t address) = 0;

        virtual uint64_t ReadDoubleword(uint64_t address) = 0;

        virtual void Reset() = 0;

        //##################################################################################################################
        //# Get + Set
        //##################################################################################################################

        virtual std::string getName() {
            return Name;
        }

        virtual uint64_t getStartAddress() {
            return Start_Address;
        };

        virtual uint64_t getEndAddress() {
            return End_Address;
        };

        virtual void setName(const std::string &name) {
            Name = name;
        }

        virtual void setStartAddress(uint64_t startAddress) {
            Start_Address = startAddress;
        }

        virtual void setEndAddress(uint64_t endAddress) {
            End_Address = endAddress;
        }

    protected:
        //##################################################################################################################
        //# Protected members
        //##################################################################################################################

        std::string Name;
        uint64_t Start_Address;
        uint64_t End_Address;
    };


}
