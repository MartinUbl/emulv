//
// Created by Hynek on 29.04.2023.
//

#pragma once

#include <string>
#include "AbstractEvent.h"

const std::string Breakpoint_Added_Event_Description = "breakpoint-added";
const std::string Breakpoint_Removed_Event_Description = "breakpoint-removed";

class BreakpointAreaWidgetEvent : public AbstractEvent {
public:
    const int Line;
    BreakpointAreaWidgetEvent(int line) : Line(line) {}

    // interface AbstractEvent
    virtual eventType getType() override {
        return eventType::Breakpoint_Area_Widget_Event;
    }
};

