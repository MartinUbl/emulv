//
// Created by Hynek on 27.04.2023.
//

#pragma once

#include "AbstractEvent.h"
#include "PeripheralDevice.h"

class PeripheralDeviceEvent : public AbstractEvent {
protected:
    const modules::PeripheralDevice &peripheralDevice_;

public:
    PeripheralDeviceEvent(const modules::PeripheralDevice &peripheralDevice) :
        AbstractEvent(),
        peripheralDevice_(peripheralDevice) {}

    // interface AbstractEvent
    virtual eventType getType() = 0;

    modules::PeripheralDevice &getPeripheralDevice() {
        return const_cast<modules::PeripheralDevice &>(peripheralDevice_);
    }

};

