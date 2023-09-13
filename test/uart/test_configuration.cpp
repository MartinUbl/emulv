#include "test_base.h"


class Test_UART_Device_Configuration : public Test_UART_Device {
    protected:

        void SetUp() override {
            UART_Device = new modules::UART_Device {"UART", kStart_Address, kEnd_Address};
        }

        void TearDown() override {
            delete UART_Device;
        }
};

TEST_F(Test_UART_Device_Configuration, Is_Reset_CTLx) {
    ASSERT_EQ(UART_Device->ReadWord(kOffset_CTL0), 0x0);
    ASSERT_EQ(UART_Device->ReadWord(kOffset_CTL1), 0x0);
    ASSERT_EQ(UART_Device->ReadWord(kOffset_CTL2), 0x0);
}

TEST_F(Test_UART_Device_Configuration, Is_Reset_STAT) {
    ASSERT_EQ(UART_Device->ReadWord(kOffset_STAT), 0x000000C0);
}

TEST_F(Test_UART_Device_Configuration, Setup_Transmitter) {
    // UEN[13] 1, WL[12] 0, TEN[3] 1
    // USART Enable, Transmitter Enable
    constexpr uint32_t bits = 0b00000000000000000010000000001000;
    UART_Device->WriteWord(kOffset_CTL0, bits);
    ASSERT_EQ(UART_Device->ReadWord(kOffset_CTL0), bits);
}

TEST_F(Test_UART_Device_Configuration, Setup_Receiver) {
    // UEN[13] 1, WL[12] 0, REN[2] 1
    // USART Enable, Receiver Enable
    constexpr uint32_t bits = 0b00000000000000000010000000000100;
    UART_Device->WriteWord(kOffset_CTL0, bits);
    ASSERT_EQ(UART_Device->ReadWord(kOffset_CTL0), bits);
}

TEST_F(Test_UART_Device_Configuration, Setup_Transmitter_Word_Length_9) {
    // UEN[13] 1, WL[12] 0, REN[2] 1
    // USART Enable, Transmitter Enable
    constexpr uint32_t bits = 0b00000000000000000011000000001000;
    UART_Device->WriteWord(kOffset_CTL0, bits);
    ASSERT_EQ(UART_Device->ReadWord(kOffset_CTL0), bits);
}

TEST_F(Test_UART_Device_Configuration, Setup_Receiver_Word_Length_9) {
    // UEN[13] 1, WL[12] 0, REN[2] 1
    // USART Enable, Receiver Enable
    constexpr uint32_t bits = 0b00000000000000000011000000000100;
    UART_Device->WriteWord(kOffset_CTL0, bits);
    ASSERT_EQ(UART_Device->ReadWord(kOffset_CTL0), bits);
}

TEST_F(Test_UART_Device_Configuration, Setup_Stop_Bits_1) {
    // STB[13:12] 00
    // 1 Stop Bit
    constexpr uint32_t bits = 0b00000000000000000000000000000000;
    UART_Device->WriteWord(kOffset_CTL1, bits);
    ASSERT_EQ(UART_Device->ReadWord(kOffset_CTL1), bits);
}

TEST_F(Test_UART_Device_Configuration, Setup_Stop_Bits_2) {
    // STB[13:12] 10
    // 2 Stop Bits
    constexpr uint32_t bits = 0b00000000000000000010000000000000;
    UART_Device->WriteWord(kOffset_CTL1, bits);
    ASSERT_EQ(UART_Device->ReadWord(kOffset_CTL1), bits);
}
