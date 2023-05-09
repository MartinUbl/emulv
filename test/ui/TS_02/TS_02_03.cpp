//
// Created by Hynek on 02.05.2023.
//
#include <QtWidgets>
#include <QTest>

#include "../../../src/ui/mainwindow.h"

class TS_02_03 : public QObject {
    Q_OBJECT
private:
    const std::string kMemoryTestElf = "../ui/elf/memory_experiment.elf";

    Controller *controller_{};
    MainWindow *main_window_{};
    RegistersWidget *registers_widget_{};

private slots:
    void initTestCase() {
        controller_ = new Controller(0, nullptr);
        main_window_ = new MainWindow(nullptr, controller_);
        registers_widget_ = main_window_->registers_widget_;
        main_window_->show();

        main_window_->OpenFile(kMemoryTestElf);
        QTest::mouseClick(main_window_->btn_run_, Qt::LeftButton);
        QTest::qWait(300);
    }

    void cleanupTestCase() {
        delete main_window_;
        delete controller_;
    }

    void TC_02_03_01() {
        QCOMPARE(registers_widget_->main_text_edit_->toPlainText(),
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
    }

    void TC_02_03_02() {
        QTest::mouseClick(main_window_->registers_widget_->rb_dec_, Qt::LeftButton);

        QCOMPARE(registers_widget_->main_text_edit_->toPlainText(),
                 QString(
                        "  x0 (zero)   000 000 000 000 (0)\n"
                        "  x1   (ra)   000 001 067 070 (82758)\n"
                        "  x2   (sp)   001 023 170 176 (18328240)\n"
                        "  x3   (gp)   000 007 091 192 (482240)\n"
                        "  x4   (tp)   000 007 183 096 (505696)\n"
                        "  x5   (t0)   000 000 000 001 (1)\n"
                        "  x6   (t1)   000 000 000 001 (1)\n"
                        "  x7   (t2)   000 000 000 000 (0)\n"
                        "  x8   (fp)   000 007 079 112 (479088)\n"
                        "  x9   (s1)   000 007 079 112 (479088)\n"
                        " x10   (a0)   000 000 000 111 (111)\n"
                        " x11   (a1)   000 000 000 111 (111)\n"
                        " x12   (a2)   000 007 097 248 (483832)\n"
                        " x13   (a3)   000 000 000 000 (0)\n"
                        " x14   (a4)   000 000 000 111 (111)\n"
                        " x15   (a5)   000 000 000 000 (0)\n"
                        " x16   (a6)   000 007 093 200 (482760)\n"
                        " x17   (a7)   000 000 000 094 (94)\n"
                        " x18   (s2)   000 000 000 004 (4)\n"
                        " x19   (s3)   000 000 000 003 (3)\n"
                        " x20   (s4)   000 000 000 001 (1)\n"
                        " x21   (s5)   000 000 000 000 (0)\n"
                        " x22   (s6)   000 000 000 111 (111)\n"
                        " x23   (s7)   000 007 048 008 (471048)\n"
                        " x24   (s8)   000 000 000 001 (1)\n"
                        " x25   (s9)   000 007 093 200 (482760)\n"
                        " x26  (s10)   000 001 006 070 (67142)\n"
                        " x27  (s11)   000 000 000 000 (0)\n"
                        " x28   (t3)   000 000 000 000 (0)\n"
                        " x29   (t4)   000 007 063 128 (475008)\n"
                        " x30   (t5)   000 006 233 180 (453044)\n"
                        " x31   (t6)   000 001 000 000 (65536)\n"
                        "  pc          000 002 010 134 (133766)\n"));
    }
};

QTEST_MAIN(TS_02_03)
#include "TS_02_03.moc"
