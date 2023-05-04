#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QToolButton>
#include <QFrame>
#include <QLabel>
#include <QSplitter>
#include <thread>

#include "disassembly/DisassemblyWidget.h"
#include "Controller.h"
#include "peripherals/gpio/GPIOWidget.h"
#include "peripherals/uart/UARTWidget.h"
#include "peripherals/PeripheralsTabWidget.h"
#include "RegistersWidget.h"
#include "memory/MemoryWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    QToolButton *btnTerminate, *btnContinue, *btnRun, *btnDebug, *btnStep;
    QFrame *runningIndicator, *debugIndicator;
    QSplitter *mainSplitter, *topSplitter, *botSplitter;
    QLabel *lblFile, *lblConfig, *lblProgramStatus;

    DisassemblyWidget *disassemblyWidget_;
    RegistersWidget *registersWidget_;
    MemoryWidget *memoryWidget_;
    PeripheralsTabWidget *peripheralsTabWidget_;

    MainWindow(QWidget *parent = nullptr, Controller *controller = nullptr);
    ~MainWindow();

    void openFile(std::string path);
    void selectConfig(std::string path);
    void clearConfig();

private slots:
    void updateUI();
    void on_action_Open_triggered();
    void on_action_SelectConfiguration_triggered();
    void on_action_ClearConfiguration_triggered();
    void on_action_About_RISCVEmulator_triggered();
    void on_btnRun_clicked();
    void on_btnDebug_clicked();
    void on_btnStep_clicked();
    void on_btnTerminate_clicked();
    void on_btnContinue_clicked();

private:
    const QString kDefaultFileLabel = "No file loaded";
    const QString kDefaultConfigLabel = "No config loaded";

    Controller *controller;

    std::unique_ptr<std::thread> mRun_Thread;

    void setupUI();

    void joinThread();
    static void showMessageBox(const QString& title, const QString& message);

    void updateRunningIndicator();
    void updateWidgetsEnabled();
    void updateRegisters();
    void updateMemory();
    void updateToolBarButtons();

    static std::string formatAddress_(uint64_t address);
};
#endif // MAINWINDOW_H
