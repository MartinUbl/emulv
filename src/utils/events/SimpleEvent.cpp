//
// Created by xPC on 26.04.2023.
//

#include "SimpleEvent.h"

eventType SimpleEvent::getType() {
    return eventType::SimpleEvent;
}

std::string SimpleEvent::getData() {
    return data_;
}

SimpleEvent::~SimpleEvent() = default;
