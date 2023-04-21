#include "test_base.h"


class Test_GPIO_Port_Input : public Test_GPIO_Port {
    protected:

        static constexpr size_t Tested_Pin_No = 9;

        void SetUp() override {
            GPIO_Port = new modules::GPIO_Port {rEmitter, kStart_Address, kEnd_Address};
            constexpr uint32_t bits = 0b00000000000000000000000010000000;
            GPIO_Port->WriteWord(kOffset_CTLO1, bits);
        }

        void TearDown() override {
            delete GPIO_Port;
        }

};

TEST_F(Test_GPIO_Port_Input, Is_Set_Input) {
    ASSERT_EQ(GPIO_Port->Get_Pin_Mode(Tested_Pin_No), modules::GPIO_Pin_Mode::INPUT);
}

TEST_F(Test_GPIO_Port_Input, Interface_Set_LOW_ISTAT) {
    GPIO_Port->Set_Pin_Level(Tested_Pin_No, modules::GPIO_Pin_Level::LOW);
    ASSERT_EQ(GPIO_Port->ReadWord(kOffset_ISTAT), 0b00000000000000000000000000000000);
}

TEST_F(Test_GPIO_Port_Input, Interface_Set_HIGH_ISTAT) {
    GPIO_Port->Set_Pin_Level(Tested_Pin_No, modules::GPIO_Pin_Level::HIGH);
    ASSERT_EQ(GPIO_Port->ReadWord(kOffset_ISTAT), 0b00000000000000000000001000000000);
}

TEST_F(Test_GPIO_Port_Input, Interface_Set_LOW_LOW) {
    GPIO_Port->Set_Pin_Level(Tested_Pin_No, modules::GPIO_Pin_Level::LOW);
    ASSERT_EQ(GPIO_Port->Get_Pin_Level(Tested_Pin_No), modules::GPIO_Pin_Level::LOW);
    GPIO_Port->Set_Pin_Level(Tested_Pin_No, modules::GPIO_Pin_Level::LOW);
    ASSERT_EQ(GPIO_Port->Get_Pin_Level(Tested_Pin_No), modules::GPIO_Pin_Level::LOW);
}

TEST_F(Test_GPIO_Port_Input, Interface_Set_LOW_HIGH) {
    GPIO_Port->Set_Pin_Level(Tested_Pin_No, modules::GPIO_Pin_Level::LOW);
    ASSERT_EQ(GPIO_Port->Get_Pin_Level(Tested_Pin_No), modules::GPIO_Pin_Level::LOW);
    GPIO_Port->Set_Pin_Level(Tested_Pin_No, modules::GPIO_Pin_Level::HIGH);
    ASSERT_EQ(GPIO_Port->Get_Pin_Level(Tested_Pin_No), modules::GPIO_Pin_Level::HIGH);
}

TEST_F(Test_GPIO_Port_Input, Interface_Set_HIGH_LOW) {
    GPIO_Port->Set_Pin_Level(Tested_Pin_No, modules::GPIO_Pin_Level::HIGH);
    ASSERT_EQ(GPIO_Port->Get_Pin_Level(Tested_Pin_No), modules::GPIO_Pin_Level::HIGH);
    GPIO_Port->Set_Pin_Level(Tested_Pin_No, modules::GPIO_Pin_Level::LOW);
    ASSERT_EQ(GPIO_Port->Get_Pin_Level(Tested_Pin_No), modules::GPIO_Pin_Level::LOW);
}

TEST_F(Test_GPIO_Port_Input, Interface_Set_HIGH_HIGH) {
    GPIO_Port->Set_Pin_Level(Tested_Pin_No, modules::GPIO_Pin_Level::HIGH);
    ASSERT_EQ(GPIO_Port->Get_Pin_Level(Tested_Pin_No), modules::GPIO_Pin_Level::HIGH);
    GPIO_Port->Set_Pin_Level(Tested_Pin_No, modules::GPIO_Pin_Level::HIGH);
    ASSERT_EQ(GPIO_Port->Get_Pin_Level(Tested_Pin_No), modules::GPIO_Pin_Level::HIGH);
}
