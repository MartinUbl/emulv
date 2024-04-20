#include "test_base.h"
#include "ConfigLoader.h"


class Test_GPIO_Port_Input : public Test_GPIO_Port {
protected:

    static constexpr uint32_t bits = 0b00000000000000000000000010000000;

    void SetUp() override {
        const configLoader::ConfigData data = configLoader::parser::loadJSON("./testGpioConfig.json");
        auto devices = data.peripheralDevices;
        GPIO = devices["gpio"];

    }

};

TEST_F(Test_GPIO_Port_Input, Interface_Set_CTLO0) {
    GPIO->WriteWord(kOffset_CTLO0, bits);
    ASSERT_EQ(GPIO->ReadWord(kOffset_CTLO0), bits);
}

TEST_F(Test_GPIO_Port_Input, Interface_Set_CTLO1) {
    GPIO->WriteWord(kOffset_CTLO1, bits);
    ASSERT_EQ(GPIO->ReadWord(kOffset_CTLO1), bits);
}

TEST_F(Test_GPIO_Port_Input, Interface_Set_ISTAT) {
    GPIO->WriteWord(kOffset_ISTAT, bits);
    ASSERT_EQ(GPIO->ReadWord(kOffset_ISTAT), bits);
}

TEST_F(Test_GPIO_Port_Input, Interface_Set_OCTL) {
    GPIO->WriteWord(kOffset_OCTL, bits);
    ASSERT_EQ(GPIO->ReadWord(kOffset_OCTL), bits);
}
