//
// Created by xPC on 01.05.2023.
//

#include "uart_event.h"

eventType uart_event::getType() {
    return eventType::UART_event;
}

uint32_t uart_event::getData() {
    return data_;
}

uart_event::uart_event(uint32_t data) {
    data_ = data;
}

