#include <gtest/gtest.h>

#include "gpio.h"
#include "EventEmitter.h"


constexpr uint64_t kOffset_CTLO0 = 0x00;
constexpr uint64_t kOffset_CTLO1 = 0x04;
constexpr uint64_t kOffset_ISTAT = 0x08;
constexpr uint64_t kOffset_OCTL = 0x0C;
constexpr uint64_t kOffset_BOP = 0x10;
constexpr uint64_t kOffset_BC = 0x14;


class Test_GPIO_Port : public ::testing::Test {
    protected:    

        static constexpr uint64_t kStart_Address = 0x40010800;
        static constexpr uint64_t kEnd_Address = 0x4001081B;

        EventEmitter emitter;
        EventEmitter& rEmitter {emitter};

        modules::GPIO_Port *GPIO_Port;
};
