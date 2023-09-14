#pragma once

#include <gtest/gtest.h>

#include "uart.h"
#include "EventEmitter.h"

constexpr uint64_t kOffset_STAT = 0x00;
constexpr uint64_t kOffset_DATA = 0x04;
constexpr uint64_t kOffset_CTL0 = 0x0C;
constexpr uint64_t kOffset_CTL1 = 0x10;
constexpr uint64_t kOffset_CTL2 = 0x14;


class Test_UART_Device : public ::testing::Test {
protected:

    static constexpr uint64_t kStart_Address = 0x40013800;
    static constexpr uint64_t kEnd_Address = 0x40013BFF;

    peripherals::UART_Device *UART_Device;
};
