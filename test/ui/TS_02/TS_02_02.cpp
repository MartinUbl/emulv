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

private slots:
    void init() {
        controller_ = new Controller(0, nullptr);
        main_window_ = new MainWindow(nullptr, controller_);
        main_window_->show();

        main_window_->openFile(kMemoryTestElf);
        QTest::mouseClick(main_window_->btnRun, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(), 1000);
        QTest::qWait(300);

        main_window_->memoryWidget_->setAddressRangeLimit(0x20000010, 0x2000001F);

        QTest::mouseClick(main_window_->memoryWidget_->btn_search_, Qt::LeftButton);
    }

    void cleanup() {
        delete main_window_;
        delete controller_;
    }

    void TC_02_02_01() {
        QCOMPARE(main_window_->memoryWidget_->te_memory_->toPlainText(), QString("20000010 6F 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    o..............."));
    }

    void TC_02_02_02() {
        QTest::mouseClick(main_window_->memoryWidget_->rb_dec_, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(), 1000);
        QCOMPARE(main_window_->memoryWidget_->te_memory_->toPlainText(), QString("20000010 111 000 000 000 000 000 000 000 000 000 000 000 000 000 000 000    o..............."));
    }
};

QTEST_MAIN(TS_02_02)
#include "TS_02_02.moc"
