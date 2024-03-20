#pragma once

#include <QtQmlIntegration>
#include "GpioPort.h"
#include "PeripheralsApi.h"
#include <array>

namespace peripherals {

//##################################################################################################################
//# GPIO controller class
//##################################################################################################################
class GpioPort;
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
    //# PERIPHERAL DEVICE INTERFACE
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
    //# Interface with GpioPort
    //##################################################################################################################
    void changePinState(int pinNo, bool currentLevel, bool currentMode);

    //##################################################################################################################
    //# Slots + Signals
    //##################################################################################################################
public Q_SLOTS:
    void togglePinLevel(int pinNo);

Q_SIGNALS:
    void pinStateChanged(int pinNo, bool currentLevel, bool currentMode);

private:
    static constexpr int portCount = 3;
    static constexpr int portPinCount = 16;
    static constexpr int portAddressSpaceSize = 1024;

    std::array<std::unique_ptr<GpioPort>, portCount> _ports;

};

}
