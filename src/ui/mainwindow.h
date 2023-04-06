#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "disassemblywidget.h"
#include "Controller.h"
#include "peripherals/gpio/GPIOWidget.h"
#include "peripherals/uart/UARTWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    MainWindow(Controller *pController);
    MainWindow(QWidget *parent, Controller *pController);

    ~MainWindow();

private slots:
    void setUARTTabVisible();
    void setGPIOTabVisible();
    void setOutputTabVisible();

    void on_action_Open_triggered();
    void on_action_About_RISCVEmulator_triggered();
    void on_spinBoxMemoryFrom_valueChanged(int arg1);
    void on_spinBoxMemoryTo_valueChanged(int arg1);
    void on_btnRestoreMemory_clicked();
    void on_btnSelectMemory_clicked();
    void on_btnRun_clicked();
    void on_btnDebug_clicked();
    void on_btnStep_clicked();
    void on_btnTerminate_clicked();
    void on_rbMemoryDec_clicked();
    void on_rbMemoryHex_clicked();
    void on_rbRegistersDec_clicked();
    void on_rbRegistersHex_clicked();

private:
    Controller *controller;

    Ui::MainWindow *ui;
    DisassemblyWidget *disassemblyWidget;
    GPIOWidget *gpioWidget;
    UARTWidget *uartWidget;

    int memoryFrom = 0, memoryTo = 0xfff;
    bool running = false;
    bool debug = false;

    void setRunning(bool running);
    void setDebug(bool debug);

    std::string generateByte(bool hex);
    std::string generateBytes(int count, bool hex);

    void updateMemoryWidgetEnabled();
    void updateRegistersWidgetEnabled();
    void updatePeripheralTabWidgetVisible();
    void updateMemorySpinBoxes();
    void updateMemoryButtons();
    void updateMemoryHeader();
    void updateTextEditMemory();
    void updateListViewRegisters();
    void updateToolBarButtons();
};
#endif // MAINWINDOW_H
