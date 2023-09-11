//
// Created by xPC on 26.04.2023.
//

#pragma once

#include <string>
#include "AbstractEvent.h"

class SimpleEvent : public AbstractEvent {
private:
    std::string data_;

public:
    SimpleEvent(std::string data): data_(data){};

    std::string getData();

    eventType getType() override;

    virtual ~SimpleEvent();
};

