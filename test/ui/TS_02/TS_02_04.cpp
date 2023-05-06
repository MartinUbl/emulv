//
// Created by Hynek on 02.05.2023.
//
#include <QtWidgets>
#include <QTest>

#include "../../../src/ui/mainwindow.h"

class TS_02_04 : public QObject {
    Q_OBJECT
private:
    const std::string kTestElf = "../ui/elf/fib.rv64.elf";
    const int kLines = 18086;

    Controller *controller_{};
    MainWindow *main_window_{};
    DisassemblyWidget *disassembly_widget_{};
    QPlainTextEdit *text_edit_addresses_{}, *text_edit_instructions_{};
    QFrame *breakpoint_area_widget_{};

private slots:
    void initTestCase() {
        controller_ = new Controller(0, nullptr);
        main_window_ = new MainWindow(nullptr, controller_);
        disassembly_widget_ = main_window_->disassembly_widget_;
        text_edit_addresses_ = disassembly_widget_->text_edit_addresses_;
        text_edit_instructions_ = disassembly_widget_->text_edit_instructions_;
        breakpoint_area_widget_ = disassembly_widget_->breakpoint_area_widget_;
        main_window_->show();

        main_window_->OpenFile(kTestElf);
    }

    void cleanupTestCase() {
        delete main_window_;
        delete controller_;
    }

    void TC_02_04_01() {
        QCOMPARE(text_edit_addresses_->document()->blockCount(),
                 kLines);
        QCOMPARE(text_edit_instructions_->document()->blockCount(),
                 kLines);
    }

    void TC_02_04_02() {
        QCOMPARE(text_edit_addresses_->document()->findBlockByLineNumber(0).text(),
                 QString("000101B8"));
        QCOMPARE(text_edit_addresses_->document()->findBlockByLineNumber(kLines - 1).text(),
                 QString("00021C4C"));
    }

    void TC_02_04_03() {
        QCOMPARE(text_edit_instructions_->document()->findBlockByLineNumber(0).text(),
                 QString(" auipc         gp,16384                        # 0x141b8"));
        QCOMPARE(text_edit_instructions_->document()->findBlockByLineNumber(kLines - 1).text(),
                 QString(" j             61440                           # 0x30c4c"));
    }

    void TC_02_04_04() {
        QTest::mouseClick(breakpoint_area_widget_, Qt::LeftButton, {}, QPoint(7, 10));
        QCOMPARE(breakpoint_area_widget_->children().size(), 1);
        QTest::mouseClick(breakpoint_area_widget_, Qt::LeftButton, {}, QPoint(7, 24));
        QCOMPARE(breakpoint_area_widget_->children().size(), 2);
    }

    void TC_02_04_05() {
        QTest::mouseClick(breakpoint_area_widget_, Qt::LeftButton, {}, QPoint(7, 10));
        QCOMPARE(breakpoint_area_widget_->children().size(), 1);
        QTest::mouseClick(breakpoint_area_widget_, Qt::LeftButton, {}, QPoint(7, 24));
        QCOMPARE(breakpoint_area_widget_->children().size(), 0);
    }

    void TC_02_04_06() {
        QTest::mouseClick(breakpoint_area_widget_, Qt::LeftButton, {}, QPoint(7, 10));
        QTest::mouseClick(breakpoint_area_widget_, Qt::LeftButton, {}, QPoint(7, 24));

        main_window_->OpenFile(kTestElf);
        QCOMPARE(breakpoint_area_widget_->children().size(), 0);
    }
};
QTEST_MAIN(TS_02_04)
#include "TS_02_04.moc"
