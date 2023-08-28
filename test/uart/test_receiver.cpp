#include "test_base.h"


class Test_UART_Device_Receiver : public Test_UART_Device {
    protected:

        static constexpr uint32_t CTL0 = 0b00000000000000000010000000000100;
        static constexpr uint32_t CTL1 = 0b00000000000000000000000000000000;

        void SetUp() override {
            UART_Device = new modules::UART_Device {"UART", rEmitter, kStart_Address, kEnd_Address};
            UART_Device->WriteWord(kOffset_CTL0, CTL0);
            UART_Device->WriteWord(kOffset_CTL1, CTL1);
        }

        void TearDown() override {
            delete UART_Device;
        }
};

TEST_F(Test_UART_Device_Receiver, TransmitToDevice) {
    // RBNE[5]
    const std::string message = "Hello World!";
    UART_Device->TransmitToDevice(message);
    for (size_t i = 0; i < message.length(); i++) {
        ASSERT_EQ(UART_Device->ReadWord(kOffset_STAT) & (1 << 5), 1 << 5); // RBNE 1
        const char readByte = static_cast<char> (UART_Device->ReadWord(kOffset_DATA));
        ASSERT_EQ(readByte, message.at(i)); // expected read byte
    }
    ASSERT_EQ(UART_Device->ReadWord(kOffset_STAT) & (1 << 5), 0); // RBNE 0
}
