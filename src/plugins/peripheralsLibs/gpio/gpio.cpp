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


namespace peripherals {

//##################################################################################################################
//# Constructors
//##################################################################################################################

    GpioController::GpioController() {
        Name = "gpio";
        _ports[0] = std::make_unique<GpioPort>(0, *this);
        _ports[1] = std::make_unique<GpioPort>(portPinCount * 1, *this);
        _ports[2] = std::make_unique<GpioPort>(portPinCount * 2, *this);

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
//# PERIPHERAL DEVICE INTERFACE
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

    void GpioController::Reset() {
        for (const auto &_port: _ports) {
            _port->reset();
        }
    }

    void GpioController::WriteWord(uint64_t address, uint32_t value) {
        //spdlog::info("Called the WRITE WORD method of GPIO port to address {0} with value {1}", address, value);

        if (address < portAddressSpaceSize) {
            _ports[0]->write(address, value);
        } else if (address < portAddressSpaceSize * 2) {
            _ports[1]->write(address - portAddressSpaceSize, value);
        } else if (address < portAddressSpaceSize * 3) {
            _ports[2]->write(address - portAddressSpaceSize * 2, value);
        }
    }

    uint32_t GpioController::ReadWord(uint64_t address) {
        //spdlog::info("Called the READ WORD method of GPIO port with address {0}", address);

        if (address < portAddressSpaceSize) {
            return _ports[0]->read(address);
        } else if (address < portAddressSpaceSize * 2) {
            return _ports[1]->read(address - portAddressSpaceSize);
        } else if (address < portAddressSpaceSize * 3) {
            return _ports[2]->read(address - portAddressSpaceSize * 2);
        }
        return 0;
    }

    void GpioController::changePinState(int pinNo, bool currentLevel, bool currentMode) {
        Q_EMIT pinStateChanged(pinNo, currentLevel, currentMode);
    }

//##################################################################################################################
//# Get + Set
//##################################################################################################################

    void GpioController::togglePinLevel(int pinNo) {
        if (pinNo < portPinCount) {
            _ports[0]->togglePinLevel(pinNo);
        } else if (pinNo < portPinCount * 2) {
            _ports[1]->togglePinLevel(pinNo - portPinCount);
        } else if (pinNo < portPinCount * 3) {
            _ports[2]->togglePinLevel(pinNo - portPinCount * 2);
        }
    }

}
