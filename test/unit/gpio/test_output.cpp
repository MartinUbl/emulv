#include "test_base.h"


class Test_GPIO_Port_Output : public Test_GPIO_Port {
    protected:

        static constexpr size_t Tested_Pin_No = 1;

        static constexpr uint32_t Bits_OCTL_High = 0b00000000000000000000000000000010;
        static constexpr uint32_t Bits_OCTL_Low = 0b00000000000000000000000000000000;

        static constexpr uint32_t Bits_BOP_High = 0b00000000000000000000000000000010;
        static constexpr uint32_t Bits_BC_Low = 0b00000000000000000000000000000010;
        static constexpr uint32_t Bits_BOP_BC_Low = 0b00000000000000100000000000000000;

        void SetUp() override {
            GPIO_Port = new modules::GPIO_Port {"GPIO", rEmitter, kStart_Address, kEnd_Address};
            constexpr uint32_t bits = 0b00000000000000000000000000010000;
            GPIO_Port->WriteWord(kOffset_CTLO0, bits);
        }

        void TearDown() override {
            delete GPIO_Port;
        }

};

TEST_F(Test_GPIO_Port_Output, Is_Set_Output) {
    ASSERT_EQ(GPIO_Port->Get_Pin_Mode(Tested_Pin_No), modules::GPIO_Pin_Mode::OUTPUT);
}

TEST_F(Test_GPIO_Port_Output, Is_LOW) {
    ASSERT_EQ(GPIO_Port->Get_Pin_Level(Tested_Pin_No), modules::GPIO_Pin_Level::LOW);
}

TEST_F(Test_GPIO_Port_Output, Output_OCTL_LOW_HIGH) {
    GPIO_Port->WriteWord(kOffset_OCTL, Bits_OCTL_High);
    ASSERT_EQ(GPIO_Port->Get_Pin_Level(Tested_Pin_No), modules::GPIO_Pin_Level::HIGH);
}

TEST_F(Test_GPIO_Port_Output, Output_OCTL_HIGH_HIGH) {
    GPIO_Port->WriteWord(kOffset_OCTL, Bits_OCTL_High);
    GPIO_Port->WriteWord(kOffset_OCTL, Bits_OCTL_High);
    ASSERT_EQ(GPIO_Port->Get_Pin_Level(Tested_Pin_No), modules::GPIO_Pin_Level::HIGH);
}

TEST_F(Test_GPIO_Port_Output, Output_OCTL_LOW_LOW) {
    GPIO_Port->WriteWord(kOffset_OCTL, Bits_OCTL_Low);
    ASSERT_EQ(GPIO_Port->Get_Pin_Level(Tested_Pin_No), modules::GPIO_Pin_Level::LOW);
}

TEST_F(Test_GPIO_Port_Output, Output_OCTL_HIGH_LOW) {
    GPIO_Port->WriteWord(kOffset_OCTL, Bits_OCTL_High);
    GPIO_Port->WriteWord(kOffset_OCTL, Bits_OCTL_Low);
    ASSERT_EQ(GPIO_Port->Get_Pin_Level(Tested_Pin_No), modules::GPIO_Pin_Level::LOW);
}

TEST_F(Test_GPIO_Port_Output, Output_BOP_LOW_HIGH) {
    GPIO_Port->WriteWord(kOffset_BOP, Bits_BOP_High);
    ASSERT_EQ(GPIO_Port->Get_Pin_Level(Tested_Pin_No), modules::GPIO_Pin_Level::HIGH);
}

TEST_F(Test_GPIO_Port_Output, Output_BOP_HIGH_HIGH) {
    GPIO_Port->WriteWord(kOffset_OCTL, Bits_OCTL_High);
    GPIO_Port->WriteWord(kOffset_BOP, Bits_BOP_High);
    ASSERT_EQ(GPIO_Port->Get_Pin_Level(Tested_Pin_No), modules::GPIO_Pin_Level::HIGH);
}

TEST_F(Test_GPIO_Port_Output, Output_BC_LOW_LOW) {
    GPIO_Port->WriteWord(kOffset_BC, Bits_BC_Low);
    ASSERT_EQ(GPIO_Port->Get_Pin_Level(Tested_Pin_No), modules::GPIO_Pin_Level::LOW);
}

TEST_F(Test_GPIO_Port_Output, Output_BC_HIGH_LOW) {
    GPIO_Port->WriteWord(kOffset_OCTL, Bits_OCTL_High);
    GPIO_Port->WriteWord(kOffset_BC, Bits_BC_Low);
    ASSERT_EQ(GPIO_Port->Get_Pin_Level(Tested_Pin_No), modules::GPIO_Pin_Level::LOW);
}

TEST_F(Test_GPIO_Port_Output, Output_BOP_BC_LOW_LOW) {
    GPIO_Port->WriteWord(kOffset_BOP, Bits_BOP_BC_Low);
    ASSERT_EQ(GPIO_Port->Get_Pin_Level(Tested_Pin_No), modules::GPIO_Pin_Level::LOW);
}

TEST_F(Test_GPIO_Port_Output, Output_BOP_BC_HIGH_LOW) {
    GPIO_Port->WriteWord(kOffset_OCTL, Bits_OCTL_High);
    ASSERT_EQ(GPIO_Port->Get_Pin_Level(Tested_Pin_No), modules::GPIO_Pin_Level::HIGH);
    GPIO_Port->WriteWord(kOffset_BOP, Bits_BOP_BC_Low);
    ASSERT_EQ(GPIO_Port->Get_Pin_Level(Tested_Pin_No), modules::GPIO_Pin_Level::LOW);
}
