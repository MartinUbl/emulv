//
// Created by Hynek on 02.05.2023.
//
#include <QtWidgets>
#include <QTest>

#include "../../../src/ui/mainwindow.h"

class TS_02_02 : public QObject {
    Q_OBJECT
private:
    const std::string kMemoryTestElf = "../ui/elf/memory_experiment.elf";

    Controller *controller_{};
    MainWindow *main_window_{};
    MemoryWidget *memory_widget_{};

private slots:
    void initTestCase() {
        controller_ = new Controller(0, nullptr);
        main_window_ = new MainWindow(nullptr, controller_);
        memory_widget_ = main_window_->memory_widget_;
        main_window_->show();

        main_window_->OpenFile(kMemoryTestElf);
        QTest::mouseClick(main_window_->btn_run_, Qt::LeftButton);
        QTest::qWait(300);

        main_window_->memory_widget_->SetAddressRangeLimit(0x20000010, 0x2000001F);

        QTest::mouseClick(memory_widget_->btn_search_, Qt::LeftButton);
    }

    void cleanupTestCase() {
        delete main_window_;
        delete controller_;
    }

    void TC_02_02_01() {
        QCOMPARE(memory_widget_->text_edit_memory_->toPlainText(), QString("20000010 6F 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    o..............."));
    }

    void TC_02_02_02() {
        QTest::mouseClick(memory_widget_->rb_dec_, Qt::LeftButton);
        QCOMPARE(memory_widget_->text_edit_memory_->toPlainText(), QString("20000010 111 000 000 000 000 000 000 000 000 000 000 000 000 000 000 000    o..............."));
    }
};

QTEST_MAIN(TS_02_02)
#include "TS_02_02.moc"
