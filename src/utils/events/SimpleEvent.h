//
// Created by xPC on 26.04.2023.
//

#ifndef EMULV_SIMPLEEVENT_H
#define EMULV_SIMPLEEVENT_H


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


#endif //EMULV_SIMPLEEVENT_H
