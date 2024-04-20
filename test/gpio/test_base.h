#pragma once

#include <gtest/gtest.h>

#include "gpio.h"
#include "PeripheralsApi.h"


constexpr uint64_t kOffset_CTLO0 = 0x00;
constexpr uint64_t kOffset_CTLO1 = 0x04;
constexpr uint64_t kOffset_ISTAT = 0x08;
constexpr uint64_t kOffset_OCTL = 0x0C;

class Test_GPIO_Port : public ::testing::Test {
protected:
    std::shared_ptr<peripherals::PeripheralsApi> GPIO;
};
