//
// Created by xPC on 26.04.2023.
//

#pragma once

/**
 * An enum specifying the various event types.
 */
enum class eventType {
    SimpleEvent,
    Breakpoint_Area_Widget_Event,
    GPIO_Pin_Mode_Changed_Event,
    GPIO_Pin_Level_Changed_Event,
    UART_event
};

class AbstractEvent {
public:
    /**
     * Returns an event type.
     * @return The type of this event.
     */
    virtual eventType getType() = 0;
};
