#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutwindow.h"
#include "../utils/events/uart_event.h"

#include <QAction>
#include <QStringListModel>
#include <iostream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent, Controller *controller)
        : QMainWindow(parent), ui(new Ui::MainWindow), controller(controller),
          disassemblyWidget(new DisassemblyWidget(this, controller)), registersWidget_(new RegistersWidget(this)),
          memoryWidget_(new MemoryWidget(this, controller)),
          peripheralsTabWidget_(new PeripheralsTabWidget(this, controller)) {
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

    controller->GetEventEmitter().On(UART_event_description, [](AbstractEvent *res) {
        uart_event *uartEvent = dynamic_cast< uart_event *>(res);

        std::cout << "An uart event has been captured: " << uartEvent->getData() << std::endl;

        //Don't forget to free the event object after using it
        delete res;
    });

}

MainWindow::~MainWindow() {
    controller->Terminate();
    joinThread();

    delete ui;
}

void MainWindow::updateUI() {
    updateRunningIndicator();
    updateWidgetsEnabled();
    updateToolBarButtons();

    if (controller->GetProgramState() == emulator::kTerminated) {
        // TODO: remove
        uint64_t start_address = controller->GetMemoryStartAddress();
        uint64_t end_address = start_address + 0xFFF;
        memoryWidget_->setAddressRangeLimit(start_address, end_address);

        updateRegisters();
        updateMemory();
    }
    // Breakpoint reached
    if (controller->GetProgramState() == emulator::kDebugPaused) {
        disassemblyWidget->highlightLine(controller->GetPc());

        updateRegisters();
        updateMemory();
    } else {
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
            peripheralsTabWidget_->setReadonly(false);
            break;
        case emulator::kDebugPaused:
            memoryWidget_->setEnabled(true);
            registersWidget_->setEnabled(true);
            peripheralsTabWidget_->setEnabled(true);
            peripheralsTabWidget_->setReadonly(false);
            break;
        case emulator::kTerminated:
            memoryWidget_->setEnabled(true);
            registersWidget_->setEnabled(true);
            peripheralsTabWidget_->setEnabled(true);
            peripheralsTabWidget_->setReadonly(true);
            break;
        default:
            memoryWidget_->setEnabled(false);
            registersWidget_->setEnabled(false);
            peripheralsTabWidget_->setEnabled(false);
            peripheralsTabWidget_->setReadonly(true);
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

void MainWindow::showMessageBox(const QString &title, const QString &message) {
    QMessageBox messageBox;
    messageBox.setWindowTitle(title);
    messageBox.setText(message);
    messageBox.exec();
}

void MainWindow::joinThread() {
    if (mRun_Thread && mRun_Thread->joinable()) {
        mRun_Thread->join();
    }
}

void MainWindow::on_action_Open_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, "Select binary file", ".");

    //If no file was selected
    if (fileName.isEmpty()) {
        return;
    }

    controller->Terminate();
    joinThread();

    try {
        controller->LoadFile(fileName.toStdString());
        disassemblyWidget->setInstructions(controller->GetDisassembly());
    }
    catch (const std::exception &e) {
        showMessageBox("File error", "Could not load file " + fileName + '\n' + e.what());
        return;
    }

    memoryWidget_->clear();
    registersWidget_->setRegisters({});
    ui->statusbar->showMessage(fileName);
}

void MainWindow::on_action_About_RISCVEmulator_triggered() {
//    AboutWindow aboutWindow;
//    aboutWindow.setModal(true);
//    aboutWindow.exec();
    controller->SendUartMessage("UART_A", "C");
}

void MainWindow::on_btnRun_clicked() {
    joinThread();
    mRun_Thread = std::make_unique<std::thread>(&Controller::RunProgram, controller);
}

void MainWindow::on_btnDebug_clicked() {
    joinThread();
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
    joinThread();

    updateRegisters();
    updateMemory();
}

void MainWindow::on_btnContinue_clicked() {
    joinThread();
    mRun_Thread = std::make_unique<std::thread>(&Controller::DebugContinue, controller);
}
