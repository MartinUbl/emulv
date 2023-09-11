//
// Created by xPC on 01.05.2023.
//
#pragma once

#include <string>
#include "AbstractEvent.h"
#include "PeripheralDeviceEvent.h"
#include "uart.h"

const std::string UART_event_description{"uart_message_received"};

class uart_event : public PeripheralDeviceEvent {
private:
    uint32_t data_;
public:
    uart_event(const modules::UART_Device &uart, uint32_t data) :
            PeripheralDeviceEvent(uart),
            data_(data) {}

    ~uart_event() = default;

    eventType getType() override {
        return eventType::UART_event;
    }

    uint32_t getData() {
        return data_;
    }
};
