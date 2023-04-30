#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <thread>
#include "disassembly/DisassemblyWidget.h"
#include "Controller.h"
#include "peripherals/gpio/GPIOWidget.h"
#include "peripherals/uart/UARTWidget.h"
#include "peripherals/PeripheralsTabWidget.h"
#include "RegistersWidget.h"
#include "memory/MemoryWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr, Controller *controller = nullptr);
    ~MainWindow();

private slots:
    void updateUI();
    void on_action_Open_triggered();
    void on_action_About_RISCVEmulator_triggered();
    void on_btnRun_clicked();
    void on_btnDebug_clicked();
    void on_btnStep_clicked();
    void on_btnTerminate_clicked();
    void on_btnContinue_clicked();

private:
    Controller *controller;

    Ui::MainWindow *ui;
    DisassemblyWidget *disassemblyWidget;
    RegistersWidget *registersWidget_;
    MemoryWidget *memoryWidget_;
    PeripheralsTabWidget *peripheralsTabWidget_;

    std::unique_ptr<std::thread> mRun_Thread;

    void updateRunningIndicator();
    void updateWidgetsEnabled();
    void updateRegisters();
    void updateMemory();
    void updateToolBarButtons();
};
#endif // MAINWINDOW_H
