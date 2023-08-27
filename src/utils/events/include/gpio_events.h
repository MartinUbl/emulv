/**
 * Represents a header file related to GPIO state change events.
 * 
 * @author Stanislav Kafara
 * @version 2023-04-27
 */

#pragma once

#include <stddef.h>
#include <string>

#include "PeripheralDeviceEvent.h"
#include "gpio.h"

const std::string GPIO_Pin_Mode_Changed_Event_Description {"gpio-pin-mode-changed"};
const std::string GPIO_Pin_Level_Changed_Event_Description {"gpio-pin-level-changed"};


class AGPIO_Event : public PeripheralDeviceEvent {

    public:
        const size_t Pin_No;

        AGPIO_Event(const modules::GPIO_Port &gpioPort, const size_t pinNo) :
            PeripheralDeviceEvent(gpioPort),
            Pin_No(pinNo)
        {}

        // interface AbstractEvent
        virtual eventType getType() = 0;
};

class GPIO_Pin_Mode_Changed_Event : public AGPIO_Event {

    public:

        const modules::GPIO_Pin_Mode Previous_Mode;
        const modules::GPIO_Pin_Mode Current_Mode;

        GPIO_Pin_Mode_Changed_Event(const modules::GPIO_Port &gpioPort, const size_t pinNo, const modules::GPIO_Pin_Mode previousMode, const modules::GPIO_Pin_Mode currentMode) :
            AGPIO_Event(gpioPort, pinNo),
            Previous_Mode(previousMode),
            Current_Mode(currentMode)
        {}

        // interface AbstractEvent
        virtual eventType getType() override {
            return eventType::GPIO_Pin_Mode_Changed_Event;
        }

};

class GPIO_Pin_Level_Changed_Event : public AGPIO_Event {

    public:

        const modules::GPIO_Pin_Level Previous_Level;
        const modules::GPIO_Pin_Level Current_Level;

        GPIO_Pin_Level_Changed_Event(const modules::GPIO_Port &gpioPort, const size_t pinNo, const modules::GPIO_Pin_Level previousLevel, const modules::GPIO_Pin_Level currentLevel) :
            AGPIO_Event(gpioPort, pinNo),
            Previous_Level(previousLevel),
            Current_Level(currentLevel)
        {}

        // interface AbstractEvent
        virtual eventType getType() override {
            return eventType::GPIO_Pin_Level_Changed_Event;
        }

};
