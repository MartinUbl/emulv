//
// Created by xPC on 01.05.2023.
//

#ifndef EMULV_UART_EVENT_H
#define EMULV_UART_EVENT_H

#include <string>
#include "AbstractEvent.h"

const std::string UART_event_description {"uart_message_received"};

class uart_event : public AbstractEvent {
private:
    uint32_t data_;
public:
    eventType getType() override;
    ~uart_event() = default;
    uint32_t getData();

    uart_event(uint32_t data);
};


#endif //EMULV_UART_EVENT_H
