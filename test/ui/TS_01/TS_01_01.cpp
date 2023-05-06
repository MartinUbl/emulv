//
// Created by Hynek on 02.05.2023.
//
#include <QtWidgets>
#include <QTest>

#include "../../../src/ui/mainwindow.h"
#include "../../../src/ui/peripherals/uart/UARTWidget.h"

class TS_01_01 : public QObject {
    Q_OBJECT
private:
    const std::string kMemoryTestElf = "../ui/elf/memory_experiment.elf";
    const std::string kGPIOTestElf = "../ui/elf/gpio_test.elf";
    const std::string kUARTTransmitTestElf = "../ui/elf/uart_test_transmitter.elf";
    const std::string kUARTReceiveTestElf = "../ui/elf/uart_test_receiver.elf";
    const std::string kConfigFile = "../ui/config.json";

    Controller *controller_{};
    MainWindow *main_window_{};
    RegistersWidget *registers_widget_{};
    MemoryWidget *memory_widget_{};

private slots:
    void initTestCase() {
        controller_ = new Controller(0, nullptr);
        main_window_ = new MainWindow(nullptr, controller_);
        registers_widget_ = main_window_->registers_widget_;
        memory_widget_ = main_window_->memory_widget_;
        main_window_->show();
    }

    void cleanupTestCase() {
        delete main_window_;
        delete controller_;
    }

    void TC_01_01_01() {
        main_window_->OpenFile(kMemoryTestElf);
        QTest::mouseClick(main_window_->btn_run_, Qt::LeftButton);
        QTest::qWait(300);

        QCOMPARE(main_window_->registers_widget_->main_text_edit_->toPlainText(),
                 QString("  x0 (zero)   00 00 00 00 \n"
                        "  x1   (ra)   00 01 43 46 \n"
                        "  x2   (sp)   01 17 AA B0 \n"
                        "  x3   (gp)   00 07 5B C0 \n"
                        "  x4   (tp)   00 07 B7 60 \n"
                        "  x5   (t0)   00 00 00 01 \n"
                        "  x6   (t1)   00 00 00 01 \n"
                        "  x7   (t2)   00 00 00 00 \n"
                        "  x8   (fp)   00 07 4F 70 \n"
                        "  x9   (s1)   00 07 4F 70 \n"
                        " x10   (a0)   00 00 00 6F \n"
                        " x11   (a1)   00 00 00 6F \n"
                        " x12   (a2)   00 07 61 F8 \n"
                        " x13   (a3)   00 00 00 00 \n"
                        " x14   (a4)   00 00 00 6F \n"
                        " x15   (a5)   00 00 00 00 \n"
                        " x16   (a6)   00 07 5D C8 \n"
                        " x17   (a7)   00 00 00 5E \n"
                        " x18   (s2)   00 00 00 04 \n"
                        " x19   (s3)   00 00 00 03 \n"
                        " x20   (s4)   00 00 00 01 \n"
                        " x21   (s5)   00 00 00 00 \n"
                        " x22   (s6)   00 00 00 6F \n"
                        " x23   (s7)   00 07 30 08 \n"
                        " x24   (s8)   00 00 00 01 \n"
                        " x25   (s9)   00 07 5D C8 \n"
                        " x26  (s10)   00 01 06 46 \n"
                        " x27  (s11)   00 00 00 00 \n"
                        " x28   (t3)   00 00 00 00 \n"
                        " x29   (t4)   00 07 3F 80 \n"
                        " x30   (t5)   00 06 E9 B4 \n"
                        " x31   (t6)   00 01 00 00 \n"
                        "  pc          00 02 0A 86 \n"));

        bool memory_empty = memory_widget_->text_edit_memory_->document()->isEmpty();
        QVERIFY(!memory_empty);

        main_window_->memory_widget_->spinbox_memory_to_->setValue(0x2000002F);
        QTest::mouseClick(main_window_->memory_widget_->btn_search_, Qt::LeftButton);
        QTest::mouseClick(main_window_->memory_widget_->rb_dec_, Qt::LeftButton);

        QCOMPARE(memory_widget_->text_edit_memory_->toPlainText(),
                 QString::fromStdString("20000000 000 000 000 000 000 000 000 000 000 000 000 000 000 000 000 000    ................\n"
                                        "20000010 111 000 000 000 000 000 000 000 000 000 000 000 000 000 000 000    o...............\n"
                                        "20000020 000 000 000 000 000 000 000 000 000 000 000 000 000 000 000 000    ................"));
    }

    void TC_01_01_02() {
        main_window_->SelectConfig(kConfigFile);
        main_window_->OpenFile(kUARTTransmitTestElf);

        QTest::mouseClick(main_window_->btn_run_, Qt::LeftButton);
        QTest::qWait(500);

        auto uartWidget = dynamic_cast<UARTWidget *>(main_window_->peripherals_tab_widget_->widgets_["UART1"]);
        QCOMPARE(uartWidget->textEditMessages_->toPlainText(),
                 QString::fromStdString("aaaa"));
    }
};

QTEST_MAIN(TS_01_01)
#include "TS_01_01.moc"
