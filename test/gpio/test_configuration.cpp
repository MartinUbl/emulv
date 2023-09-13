#include "test_base.h"


class Test_GPIO_Port_Configuration : public Test_GPIO_Port {
    protected:

        void SetUp() override {
            GPIO_Port = new modules::GPIO_Port {"GPIO", kStart_Address, kEnd_Address};
        }

        void TearDown() override {
            delete GPIO_Port;
        }
};

TEST_F(Test_GPIO_Port_Configuration, Is_Reset_CTLO0) {
    ASSERT_EQ(GPIO_Port->ReadWord(kOffset_CTLO0), 0x44444444);
}

TEST_F(Test_GPIO_Port_Configuration, Is_Reset_CTLO1) {
    ASSERT_EQ(GPIO_Port->ReadWord(kOffset_CTLO0), 0x44444444);
}

TEST_F(Test_GPIO_Port_Configuration, Configure_Pin_Out_0) {
    constexpr uint32_t bits = 0b00000000000000000000000000000001;
    GPIO_Port->WriteWord(kOffset_CTLO0, bits);

    ASSERT_EQ(GPIO_Port->Get_Pin_Mode(0), modules::GPIO_Pin_Mode::OUTPUT);
}

TEST_F(Test_GPIO_Port_Configuration, Configure_Pin_Out_7) {
    constexpr uint32_t bits = 0b00010000000000000000000000000000;
    GPIO_Port->WriteWord(kOffset_CTLO0, bits);

    ASSERT_EQ(GPIO_Port->Get_Pin_Mode(7), modules::GPIO_Pin_Mode::OUTPUT);
}

TEST_F(Test_GPIO_Port_Configuration, Configure_Pin_Out_8) {
    constexpr uint32_t bits = 0b00000000000000000000000000000001;
    GPIO_Port->WriteWord(kOffset_CTLO1, bits);

    ASSERT_EQ(GPIO_Port->Get_Pin_Mode(8), modules::GPIO_Pin_Mode::OUTPUT);
}

TEST_F(Test_GPIO_Port_Configuration, Configure_Pin_Out_15) {
    constexpr uint32_t bits = 0b00010000000000000000000000000000;
    GPIO_Port->WriteWord(kOffset_CTLO1, bits);

    ASSERT_EQ(GPIO_Port->Get_Pin_Mode(15), modules::GPIO_Pin_Mode::OUTPUT);
}

TEST_F(Test_GPIO_Port_Configuration, Configure_Pin_In_0) {
    constexpr uint32_t bits = 0b00000000000000000000000000001000;
    GPIO_Port->WriteWord(kOffset_CTLO0, bits);

    ASSERT_EQ(GPIO_Port->Get_Pin_Mode(0), modules::GPIO_Pin_Mode::INPUT);
}

TEST_F(Test_GPIO_Port_Configuration, Configure_Pin_In_7) {
    constexpr uint32_t bits = 0b10000000000000000000000000000000;
    GPIO_Port->WriteWord(kOffset_CTLO0, bits);

    ASSERT_EQ(GPIO_Port->Get_Pin_Mode(7), modules::GPIO_Pin_Mode::INPUT);
}

TEST_F(Test_GPIO_Port_Configuration, Configure_Pin_In_8) {
    constexpr uint32_t bits = 0b00000000000000000000000000001000;
    GPIO_Port->WriteWord(kOffset_CTLO1, bits);

    ASSERT_EQ(GPIO_Port->Get_Pin_Mode(8), modules::GPIO_Pin_Mode::INPUT);
}

TEST_F(Test_GPIO_Port_Configuration, Configure_Pin_In_15) {
    constexpr uint32_t bits = 0b10000000000000000000000000000000;
    GPIO_Port->WriteWord(kOffset_CTLO1, bits);

    ASSERT_EQ(GPIO_Port->Get_Pin_Mode(15), modules::GPIO_Pin_Mode::INPUT);
}
