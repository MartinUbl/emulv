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
    const std::string kConfigFile = "../ui/config.json";

    Controller *controller_{};
    MainWindow *main_window_{};
    QToolButton *btn_run_{}, *btn_debug_{}, *btn_terminate_{}, *btn_step_{}, *btn_continue_{};
    DisassemblyWidget *disassembly_widget_{};
    RegistersWidget *registers_widget_{};
    MemoryWidget *memory_widget_{};
    PeripheralsTabWidget *peripherals_tab_widget_{};

private slots:
    void initTestCase() {
        controller_ = new Controller(0, nullptr);
        main_window_ = new MainWindow(nullptr, controller_);
        btn_run_ = main_window_->btn_run_;
        btn_debug_ = main_window_->btn_debug_;
        btn_terminate_ = main_window_->btn_terminate_;
        btn_step_ = main_window_->btn_step_;
        btn_continue_ = main_window_->btn_continue_;
        disassembly_widget_ = main_window_->disassembly_widget_;
        registers_widget_ = main_window_->registers_widget_;
        memory_widget_ = main_window_->memory_widget_;
        peripherals_tab_widget_ = main_window_->peripherals_tab_widget_;
        main_window_->show();
    }

    void cleanupTestCase() {
        delete main_window_;
        delete controller_;
    }

    void TC_02_01_01() {
        QVERIFY(!registers_widget_->isEnabled());
        QVERIFY(!memory_widget_->isEnabled());
        QVERIFY(!peripherals_tab_widget_->isVisible());
        QVERIFY(!peripherals_tab_widget_->isEnabled());
    }

    void TC_02_01_02() {
        QVERIFY(btn_run_->isVisible());
        QVERIFY(!btn_run_->isEnabled());
        QVERIFY(btn_debug_->isVisible());
        QVERIFY(!btn_debug_->isEnabled());
        QVERIFY(!btn_terminate_->isVisible());
        QVERIFY(!btn_step_->isVisible());
        QVERIFY(!btn_continue_->isVisible());
    }

    void TC_02_01_03() {
        main_window_->OpenFile(kGPIOTestElf);
        QVERIFY(btn_run_->isEnabled());
        QVERIFY(btn_debug_->isEnabled());
    }

    void TC_02_01_04() {
        main_window_->SelectConfig(kConfigFile);
        QVERIFY(peripherals_tab_widget_->isVisible());
        QVERIFY(!peripherals_tab_widget_->isEnabled());
    }

    void TC_02_01_05() {
        main_window_->OpenFile(kGPIOTestElf);
        QTest::mouseClick(btn_run_, Qt::LeftButton);
        QTest::qWait(300);
        QVERIFY(!btn_run_->isVisible());
        QVERIFY(!btn_run_->isEnabled());
        QVERIFY(!btn_debug_->isVisible());
        QVERIFY(!btn_debug_->isEnabled());
        QVERIFY(btn_terminate_->isVisible());
        QVERIFY(btn_terminate_->isEnabled());
        QVERIFY(btn_step_->isVisible());
        QVERIFY(!btn_step_->isEnabled());
        QVERIFY(btn_continue_->isVisible());
        QVERIFY(!btn_continue_->isEnabled());
        QVERIFY(!registers_widget_->isEnabled());
        QVERIFY(!memory_widget_->isEnabled());
        QVERIFY(peripherals_tab_widget_->isEnabled());
    }

    void TC_02_01_06() {
        main_window_->OpenFile(kGPIOTestElf);
        QTest::mouseClick(disassembly_widget_->breakpoint_area_widget_, Qt::LeftButton, {}, QPoint(7, 10));
        QTest::mouseClick(btn_debug_, Qt::LeftButton);
        QTest::qWait(300);
        QVERIFY(!btn_run_->isVisible());
        QVERIFY(!btn_run_->isEnabled());
        QVERIFY(!btn_debug_->isVisible());
        QVERIFY(!btn_debug_->isEnabled());
        QVERIFY(btn_terminate_->isVisible());
        QVERIFY(btn_terminate_->isEnabled());
        QVERIFY(btn_step_->isVisible());
        QVERIFY(btn_step_->isEnabled());
        QVERIFY(btn_continue_->isVisible());
        QVERIFY(btn_continue_->isEnabled());
        QVERIFY(registers_widget_->isEnabled());
        QVERIFY(memory_widget_->isEnabled());
        QVERIFY(peripherals_tab_widget_->isEnabled());
    }

    void TC_02_01_07() {
        main_window_->OpenFile(kGPIOTestElf);
        QTest::mouseClick(btn_run_, Qt::LeftButton);
        QTest::mouseClick(btn_terminate_, Qt::LeftButton, {}, QPoint(), 200);
        QTest::qWait(300);
        QVERIFY(btn_run_->isVisible());
        QVERIFY(btn_run_->isEnabled());
        QVERIFY(btn_debug_->isVisible());
        QVERIFY(btn_debug_->isEnabled());
        QVERIFY(!btn_terminate_->isVisible());
        QVERIFY(!btn_step_->isVisible());
        QVERIFY(!btn_continue_->isVisible());
        QVERIFY(registers_widget_->isEnabled());
        QVERIFY(memory_widget_->isEnabled());
        QVERIFY(peripherals_tab_widget_->isEnabled());
    }

    void TC_02_01_08() {
        main_window_->ClearConfig();
        QVERIFY(!peripherals_tab_widget_->isVisible());
    }
};

QTEST_MAIN(TS_02_01)
#include "TS_02_01.moc"
