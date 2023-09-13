#include "test_base.h"


class Test_UART_Device_Transmitter : public Test_UART_Device {
    protected:

        static constexpr uint32_t CTL0 = 0b00000000000000000010000000001000;
        static constexpr uint32_t CTL1 = 0b00000000000000000000000000000000;

        void SetUp() override {
            UART_Device = new modules::UART_Device {"UART", kStart_Address, kEnd_Address};
            UART_Device->WriteWord(kOffset_CTL0, CTL0);
            UART_Device->WriteWord(kOffset_CTL1, CTL1);
        }

        void TearDown() override {
            delete UART_Device;
        }
};

TEST_F(Test_UART_Device_Transmitter, Is_Reset_TBE) {
    // TBE[7]
    ASSERT_EQ(UART_Device->ReadWord(kOffset_STAT) & (1 << 7), 1 << 7);
}

TEST_F(Test_UART_Device_Transmitter, Is_Reset_TC) {
    // TC[6]
    ASSERT_EQ(UART_Device->ReadWord(kOffset_STAT) & (1 << 6), 1 << 6);
}

TEST_F(Test_UART_Device_Transmitter, Write_DATA) {
    // ord(6) == 54
    constexpr uint32_t data = static_cast<uint32_t> ('6');
    UART_Device->WriteWord(kOffset_DATA, data);
    ASSERT_EQ(UART_Device->ReadWord(kOffset_DATA), data);
}

TEST_F(Test_UART_Device_Transmitter, Transmit_Char_TBE) {
    // ord(6) == 54
    // TBE[7]
    constexpr uint32_t data = static_cast<uint32_t> ('6');
    UART_Device->WriteWord(kOffset_DATA, data);
    ASSERT_EQ(UART_Device->ReadWord(kOffset_STAT) & (1 << 7), 1 << 7);
}

TEST_F(Test_UART_Device_Transmitter, Transmit_Char_TC) {
    // ord(6) == 54
    // TC[6]
    constexpr uint32_t data = static_cast<uint32_t> ('6');
    UART_Device->WriteWord(kOffset_DATA, data);
    ASSERT_EQ(UART_Device->ReadWord(kOffset_STAT) & (1 << 6), 1 << 6);
}

TEST_F(Test_UART_Device_Transmitter, Transmit_String) {
    // TBE[7]
    // TC[6]
    const std::string message = "Hello World!";
    for (size_t i = 0; i < message.length(); i++) {
        ASSERT_EQ(UART_Device->ReadWord(kOffset_STAT) & (1 << 7), 1 << 7); // TBE 1
        const char writeByte = message.at(i);
        UART_Device->WriteWord(kOffset_DATA, writeByte);
    }
    ASSERT_EQ(UART_Device->ReadWord(kOffset_STAT) & (1 << 7), 1 << 7); // TBE 1
    ASSERT_EQ(UART_Device->ReadWord(kOffset_STAT) & (1 << 6), 1 << 6); // TC 1
}
