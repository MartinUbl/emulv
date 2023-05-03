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

private slots:
    void initTestCase() {
        controller_ = new Controller(0, nullptr);
        main_window_ = new MainWindow(nullptr, controller_);
        main_window_->show();

        main_window_->openFile(kTestElf);
        QTest::qWait(1000);
    }

    void cleanupTestCase() {
        delete main_window_;
        delete controller_;
    }

    void TC_02_04_01() {
        QCOMPARE(main_window_->disassemblyWidget_->addressArea->document()->blockCount(),
                 kLines);
        QCOMPARE(main_window_->disassemblyWidget_->instructionArea->document()->blockCount(),
                 kLines);
    }

    void TC_02_04_02() {
        QCOMPARE(main_window_->disassemblyWidget_->addressArea->document()->findBlockByLineNumber(0).text(),
                 QString("000101B8"));
        QCOMPARE(main_window_->disassemblyWidget_->addressArea->document()->findBlockByLineNumber(kLines - 1).text(),
                 QString("00021C4C"));
    }

    void TC_02_04_03() {
        QCOMPARE(main_window_->disassemblyWidget_->instructionArea->document()->findBlockByLineNumber(0).text(),
                 QString("auipc         gp,16384                        # 0x141b8"));
        QCOMPARE(main_window_->disassemblyWidget_->instructionArea->document()->findBlockByLineNumber(kLines - 1).text(),
                 QString("j             61440                           # 0x30c4c"));
    }

    void TC_02_04_04() {
        QTest::mouseClick(main_window_->disassemblyWidget_->breakpointAreaWidget, Qt::LeftButton, {}, QPoint(7, 10));
        QCOMPARE(main_window_->disassemblyWidget_->breakpointAreaWidget->children().size(), 1);
        QTest::mouseClick(main_window_->disassemblyWidget_->breakpointAreaWidget, Qt::LeftButton, {}, QPoint(7, 24));
        QCOMPARE(main_window_->disassemblyWidget_->breakpointAreaWidget->children().size(), 2);
    }

    void TC_02_04_05() {
        QTest::mouseClick(main_window_->disassemblyWidget_->breakpointAreaWidget, Qt::LeftButton, {}, QPoint(7, 10));
        QCOMPARE(main_window_->disassemblyWidget_->breakpointAreaWidget->children().size(), 1);
        QTest::mouseClick(main_window_->disassemblyWidget_->breakpointAreaWidget, Qt::LeftButton, {}, QPoint(7, 24));
        QCOMPARE(main_window_->disassemblyWidget_->breakpointAreaWidget->children().size(), 0);
    }

    void TC_02_04_06() {
        QTest::mouseClick(main_window_->disassemblyWidget_->breakpointAreaWidget, Qt::LeftButton, {}, QPoint(7, 10));
        QTest::mouseClick(main_window_->disassemblyWidget_->breakpointAreaWidget, Qt::LeftButton, {}, QPoint(7, 24));

        main_window_->openFile(kTestElf);
        QTest::qWait(1000);
        QCOMPARE(main_window_->disassemblyWidget_->breakpointAreaWidget->children().size(), 0);
    }
};
QTEST_MAIN(TS_02_04)
#include "TS_02_04.moc"
