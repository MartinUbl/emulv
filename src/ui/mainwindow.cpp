#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutwindow.h"
#include "peripherals/gpio/GPIOPortWidget.h"

#include <QAction>
#include <QStringListModel>

#include <sstream>
#include <iostream>

MainWindow::MainWindow(QWidget *parent, Controller *controller)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
, controller(controller)
, disassemblyWidget(new DisassemblyWidget(this, controller))
, registersWidget_(new RegistersWidget(this))
, memoryWidget_(new MemoryWidget(this, controller))
, peripheralsTabWidget_(new PeripheralsTabWidget(this, controller)) {
    ui->setupUi(this);

    // Initialize toolbar buttons
    ui->btnTerminate->setIcon(QIcon(":img/terminate.png"));
    ui->btnContinue->setIcon(QIcon(":img/continue.png"));
    ui->btnRun->setIcon(QIcon(":img/run.png"));
    ui->btnDebug->setIcon(QIcon(":img/debug.png"));
    ui->btnStep->setIcon(QIcon(":img/step.png"));

    QSize btnSize(25, 25);
    ui->btnTerminate->setFixedSize(btnSize);
    ui->btnContinue->setFixedSize(btnSize);
    ui->btnRun->setFixedSize(btnSize);
    ui->btnDebug->setFixedSize(btnSize);
    ui->btnStep->setFixedSize(btnSize);

    QSize iconSize(13, 13);
    ui->btnTerminate->setIconSize(QSize(11, 11));
    ui->btnContinue->setIconSize(iconSize);
    ui->btnRun->setIconSize(iconSize);
    ui->btnDebug->setIconSize(QSize(16, 16));
    ui->btnStep->setIconSize(QSize(16, 16));

    // Make running and debug indicator lines invisible
    ui->runningIndicator->setVisible(false);
    ui->debugIndicator->setVisible(false);

    // Initialize toolbar button states
    updateToolBarButtons();

    // Initialize ui for disassembly widget
    ui->disassemblyLayout->addWidget(disassemblyWidget);

    // Initialize ui for registers widget
    ui->registersWidget->setLayout(new QVBoxLayout(ui->registersWidget));
    ui->registersWidget->layout()->setSpacing(0);
    ui->registersWidget->layout()->setContentsMargins(0, 0, 6, 0);
    ui->registersWidget->layout()->addWidget(registersWidget_);

    // Initialize ui for memory widget
    ui->memoryWidget->setLayout(new QVBoxLayout(ui->memoryWidget));
    ui->memoryWidget->layout()->setSpacing(0);
    ui->memoryWidget->layout()->setContentsMargins(0, 0, 0, 0);
    ui->memoryWidget->layout()->addWidget(memoryWidget_);

    // Initialize ui for peripheral widgets
    ui->peripheralWidget->layout()->addWidget(peripheralsTabWidget_);

    // Initialize splitter ratio
    ui->splitterTop->setStretchFactor(0, 1);
    ui->splitterTop->setStretchFactor(1, 0);

    ui->splitterBottom->setStretchFactor(0, 1);
    ui->splitterBottom->setStretchFactor(1, 1);

    ui->splitterMain->setStretchFactor(0, 3);
    ui->splitterMain->setStretchFactor(1, 2);

    peripheralsTabWidget_->updateWidgets();

    memoryWidget_->setEnabled(false);
    registersWidget_->setEnabled(false);
    peripheralsTabWidget_->setEnabled(false);

    controller->GetEventEmitter().On(emulator::State_Changed_Event_Description, [this](AbstractEvent *res) {
        QMetaObject::invokeMethod(this, "updateUI");
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updateUI() {
    updateRunningIndicator();
    updateWidgetsEnabled();
    updateToolBarButtons();

    // Breakpoint reached
    if (controller->GetProgramState() == emulator::kDebugPaused) {
        disassemblyWidget->highlightLine(controller->GetPc());

        updateRegisters();
        updateMemory();
    }
    else {
        disassemblyWidget->highlightLine(-1);
    }
}

void MainWindow::updateRunningIndicator() {
    switch (controller->GetProgramState()) {
        case emulator::kRunning:
        case emulator::kRunningDebug:
        case emulator::kDebugPaused:
            ui->runningIndicator->setVisible(false);
            ui->debugIndicator->setVisible(true);
            break;
        default:
            ui->runningIndicator->setVisible(false);
            ui->debugIndicator->setVisible(false);
            break;
    }
}

void MainWindow::updateWidgetsEnabled() {
    switch (controller->GetProgramState()) {
        case emulator::kRunning:
        case emulator::kRunningDebug:
            memoryWidget_->setEnabled(false);
            registersWidget_->setEnabled(false);
            peripheralsTabWidget_->setEnabled(true);
            break;
        case emulator::kDebugPaused:
        case emulator::kTerminated:
            memoryWidget_->setEnabled(true);
            registersWidget_->setEnabled(true);
            peripheralsTabWidget_->setEnabled(true);
            break;
        default:
            memoryWidget_->setEnabled(false);
            registersWidget_->setEnabled(false);
            peripheralsTabWidget_->setEnabled(false);
            break;
    }
}

void MainWindow::updateToolBarButtons() {
    auto state = controller->GetProgramState();

    bool notRunningButtonsVisible = state == emulator::kDefault ||
                             state == emulator::kReady ||
                             state == emulator::kTerminated;

    bool runningButtonsVisible = !notRunningButtonsVisible;

    ui->btnRun->setEnabled(state == emulator::kReady || state == emulator::kTerminated);
    ui->btnRun->setVisible(notRunningButtonsVisible);

    ui->btnDebug->setEnabled(state == emulator::kReady || state == emulator::kTerminated);
    ui->btnDebug->setVisible(notRunningButtonsVisible);

    ui->btnTerminate->setEnabled(runningButtonsVisible);
    ui->btnTerminate->setVisible(runningButtonsVisible);

    ui->btnStep->setEnabled(state == emulator::kDebugPaused);
    ui->btnStep->setVisible(runningButtonsVisible);

    ui->btnContinue->setEnabled(state == emulator::kDebugPaused);
    ui->btnContinue->setVisible(runningButtonsVisible);
}

void MainWindow::updateRegisters() {
    registersWidget_->setRegisters(controller->GetRegisters());
}

void MainWindow::updateMemory() {
    memoryWidget_->updateMemory();
}

void MainWindow::on_action_Open_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, "Select binary file", ".");
    ui->statusbar->showMessage(fileName);
    //If no file was selected
    if (fileName.isEmpty()) {
        return;
    }

    controller->LoadFile(fileName.toStdString());
    disassemblyWidget->addInstructionsList(controller->GetDisassembly());
    updateToolBarButtons();

    registersWidget_->setEnabled(false);
    memoryWidget_->setEnabled(false);
    peripheralsTabWidget_->setEnabled(false);
}

void MainWindow::on_action_About_RISCVEmulator_triggered() {
    AboutWindow aboutWindow;
    aboutWindow.setModal(true);
    aboutWindow.exec();
}

void MainWindow::on_btnRun_clicked() {
    if (mRun_Thread && mRun_Thread->joinable()) {
        mRun_Thread->join();
    }

    mRun_Thread = std::make_unique<std::thread>(&Controller::RunProgram, controller);
}

void MainWindow::on_btnDebug_clicked() {
    if (mRun_Thread && mRun_Thread->joinable()) {
        mRun_Thread->join();
    }

    ui->statusbar->showMessage(QString::fromStdString("Debugger started."));
    mRun_Thread = std::make_unique<std::thread>(&Controller::DebugProgram, controller);
}

void MainWindow::on_btnStep_clicked() {
    controller->DebugStep();

    updateRegisters();
    updateMemory();

    disassemblyWidget->highlightLine(controller->GetPc());
}

void MainWindow::on_btnTerminate_clicked() {
    controller->Terminate();

    uint64_t start_address = controller->GetMemoryStartAddress();
    uint64_t end_address = start_address + 0xFFF;
    memoryWidget_->setAddressRangeLimit(start_address, end_address);

    updateRegisters();
    updateMemory();
}

void MainWindow::on_btnContinue_clicked() {
    if (mRun_Thread && mRun_Thread->joinable()) {
        mRun_Thread->join();
    }

    mRun_Thread = std::make_unique<std::thread>(&Controller::DebugContinue, controller);
}
