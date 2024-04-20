#include "test_base.h"
#include "../../src/utils/config/include/ConfigLoader.h"


class Test_GPIO_Port_Configuration : public Test_GPIO_Port {
    protected:

        void SetUp() override {
            const configLoader::ConfigData data = configLoader::parser::loadJSON("./testGpioConfig.json");
            auto devices = data.peripheralDevices;
            GPIO = devices["gpio"];
        }
};

TEST_F(Test_GPIO_Port_Configuration, Is_Reset_CTLO0) {
    ASSERT_EQ(GPIO->ReadWord(kOffset_CTLO0), 0x44444444);
}

TEST_F(Test_GPIO_Port_Configuration, Is_Reset_CTLO1) {
    ASSERT_EQ(GPIO->ReadWord(kOffset_CTLO0), 0x44444444);
}

TEST_F(Test_GPIO_Port_Configuration, Configure_Pin_Out_0) {
    constexpr uint32_t bits = 0b00000000000000000000000000000001;
    GPIO->WriteWord(kOffset_CTLO0, bits);

    uint32_t val = GPIO->ReadWord(kOffset_CTLO0);

    // Test if the bit which was altered by WriteWord is set
    ASSERT_EQ(val, bits);
}

TEST_F(Test_GPIO_Port_Configuration, Configure_Pin_Out_7) {
    constexpr uint32_t bits = 0b00010000000000000000000000000000;
    GPIO->WriteWord(kOffset_CTLO0, bits);

    uint32_t val = GPIO->ReadWord(kOffset_CTLO0);
    ASSERT_EQ(val, bits);
}

TEST_F(Test_GPIO_Port_Configuration, Configure_Pin_Out_8) {
    constexpr uint32_t bits = 0b00000000000000000000000000000001;
    GPIO->WriteWord(kOffset_CTLO1, bits);

    uint32_t val = GPIO->ReadWord(kOffset_CTLO1);
    ASSERT_EQ(val, bits);
}

TEST_F(Test_GPIO_Port_Configuration, Configure_Pin_Out_15) {
    constexpr uint32_t bits = 0b00010000000000000000000000000000;
    GPIO->WriteWord(kOffset_CTLO1, bits);

    uint32_t val = GPIO->ReadWord(kOffset_CTLO1);
    ASSERT_EQ(val, bits);
}

TEST_F(Test_GPIO_Port_Configuration, Configure_Pin_In_0) {
    constexpr uint32_t bits = 0b00000000000000000000000000001000;
    GPIO->WriteWord(kOffset_CTLO0, bits);

    uint32_t val = GPIO->ReadWord(kOffset_CTLO0);
    ASSERT_EQ(val, bits);
}

TEST_F(Test_GPIO_Port_Configuration, Configure_Pin_In_7) {
    constexpr uint32_t bits = 0b10000000000000000000000000000000;
    GPIO->WriteWord(kOffset_CTLO0, bits);

    uint32_t val = GPIO->ReadWord(kOffset_CTLO0);
    ASSERT_EQ(val, bits);
}

TEST_F(Test_GPIO_Port_Configuration, Configure_Pin_In_8) {
    constexpr uint32_t bits = 0b00000000000000000000000000001000;
    GPIO->WriteWord(kOffset_CTLO1, bits);

    uint32_t val = GPIO->ReadWord(kOffset_CTLO1);
    ASSERT_EQ(val, bits);
}

TEST_F(Test_GPIO_Port_Configuration, Configure_Pin_In_15) {
    constexpr uint32_t bits = 0b10000000000000000000000000000000;
    GPIO->WriteWord(kOffset_CTLO1, bits);

    uint32_t val = GPIO->ReadWord(kOffset_CTLO1);
    ASSERT_EQ(val, bits);
}
