//
// Created by Hynek on 02.05.2023.
//
#include <QtWidgets>
#include <QTest>

#include "../../../src/ui/mainwindow.h"

class TS_02_01 : public QObject {
    Q_OBJECT
private:
    const std::string kGPIOTestElf = "../ui/elf/gpio_test.elf";

    Controller *controller_{};
    MainWindow *main_window_{};

private slots:
    void initTestCase() {
        controller_ = new Controller(0, nullptr);
        main_window_ = new MainWindow(nullptr, controller_);
        main_window_->show();
    }

    void cleanupTestCase() {
        delete main_window_;
        delete controller_;
    }

    void TC_02_01_01() {
        auto registers_widget = main_window_->registersWidget_;
        auto memory_widget = main_window_->memoryWidget_;
        auto peripherals_widget = main_window_->peripheralsTabWidget_;

        QVERIFY(registers_widget->isEnabled() == false);
        QVERIFY(memory_widget->isEnabled() == false);
        QVERIFY(peripherals_widget->isEnabled() == false);
    }

    void TC_02_01_02() {
        auto btn = main_window_->btnRun;
        QVERIFY(btn->isVisible() == true);
        QVERIFY(btn->isEnabled() == false);

        btn = main_window_->btnDebug;
        QVERIFY(btn->isVisible() == true);
        QVERIFY(btn->isEnabled() == false);

        btn = main_window_->btnTerminate;
        QVERIFY(btn->isVisible() == false);

        btn = main_window_->btnStep;
        QVERIFY(btn->isVisible() == false);

        btn = main_window_->btnContinue;
        QVERIFY(btn->isVisible() == false);
    }

    void TC_02_01_03() {
        main_window_->openFile(kGPIOTestElf);
        auto btn = main_window_->btnRun;
        QVERIFY(btn->isEnabled() == true);
        btn = main_window_->btnDebug;
        QVERIFY(btn->isEnabled() == true);
    }

    void TC_02_01_04() {
        main_window_->openFile(kGPIOTestElf);
        QTest::mouseClick(main_window_->btnRun, Qt::LeftButton, {}, QPoint(), 1000);
        QTest::qWait(300);
        QVERIFY(main_window_->btnRun->isVisible() == false);
        QVERIFY(main_window_->btnRun->isEnabled() == false);
        QVERIFY(main_window_->btnDebug->isVisible() == false);
        QVERIFY(main_window_->btnDebug->isEnabled() == false);
        QVERIFY(main_window_->btnTerminate->isVisible() == true);
        QVERIFY(main_window_->btnTerminate->isEnabled() == true);
        QVERIFY(main_window_->btnStep->isVisible() == true);
        QVERIFY(main_window_->btnStep->isEnabled() == false);
        QVERIFY(main_window_->btnContinue->isVisible() == true);
        QVERIFY(main_window_->btnContinue->isEnabled() == false);
        QVERIFY(main_window_->registersWidget_->isEnabled() == false);
        QVERIFY(main_window_->memoryWidget_->isEnabled() == false);
        QVERIFY(main_window_->peripheralsTabWidget_->isEnabled() == true);
    }

    void TC_02_01_05() {
        main_window_->openFile(kGPIOTestElf);
        QTest::mouseClick(main_window_->disassemblyWidget_->breakpointAreaWidget, Qt::LeftButton, {}, QPoint(7, 10));
        QTest::mouseClick(main_window_->btnDebug, Qt::LeftButton, {}, QPoint(), 1000);
        QTest::qWait(300);
        QVERIFY(main_window_->btnRun->isVisible() == false);
        QVERIFY(main_window_->btnRun->isEnabled() == false);
        QVERIFY(main_window_->btnDebug->isVisible() == false);
        QVERIFY(main_window_->btnDebug->isEnabled() == false);
        QVERIFY(main_window_->btnTerminate->isVisible() == true);
        QVERIFY(main_window_->btnTerminate->isEnabled() == true);
        QVERIFY(main_window_->btnStep->isVisible() == true);
        QVERIFY(main_window_->btnStep->isEnabled() == true);
        QVERIFY(main_window_->btnContinue->isVisible() == true);
        QVERIFY(main_window_->btnContinue->isEnabled() == true);
        QVERIFY(main_window_->registersWidget_->isEnabled() == true);
        QVERIFY(main_window_->memoryWidget_->isEnabled() == true);
        QVERIFY(main_window_->peripheralsTabWidget_->isEnabled() == true);
    }

    void TC_02_01_06() {
        main_window_->openFile(kGPIOTestElf);
        QTest::mouseClick(main_window_->btnRun, Qt::LeftButton, {}, QPoint(), 1000);
        QTest::mouseClick(main_window_->btnTerminate, Qt::LeftButton, {}, QPoint(), 1000);
        QTest::qWait(300);
        QVERIFY(main_window_->btnRun->isVisible() == true);
        QVERIFY(main_window_->btnRun->isEnabled() == true);
        QVERIFY(main_window_->btnDebug->isVisible() == true);
        QVERIFY(main_window_->btnDebug->isEnabled() == true);
        QVERIFY(main_window_->btnTerminate->isVisible() == false);
        QVERIFY(main_window_->btnStep->isVisible() == false);
        QVERIFY(main_window_->btnContinue->isVisible() == false);
        QVERIFY(main_window_->registersWidget_->isEnabled() == true);
        QVERIFY(main_window_->memoryWidget_->isEnabled() == true);
        QVERIFY(main_window_->peripheralsTabWidget_->isEnabled() == true);
    }
};

QTEST_MAIN(TS_02_01)
#include "TS_02_01.moc"
