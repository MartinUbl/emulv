//
// Created by xPC on 26.04.2023.
//

#ifndef EMULV_ABSTRACTEVENT_H
#define EMULV_ABSTRACTEVENT_H

/**
 * An enum specifying the various event types.
 */
enum class eventType {
    SimpleEvent,
    Breakpoint_Area_Widget_Event,
    GPIO_Pin_Mode_Changed_Event,
    GPIO_Pin_Level_Changed_Event
};

class AbstractEvent {
public:
    /**
     * Returns an event type.
     * @return The type of this event.
     */
    virtual eventType getType() = 0;
};

#endif //EMULV_ABSTRACTEVENT_H
