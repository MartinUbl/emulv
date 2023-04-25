#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutwindow.h"
#include "peripherals/gpio/GPIOPortWidget.h"

#include <QAction>
#include <QMessageBox>
#include <QStringListModel>

#include <sstream>
#include <iomanip>

MainWindow::MainWindow(QWidget *parent, Controller *controller)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
, controller(controller)
, disassemblyWidget(new DisassemblyWidget(this))
, registersWidget_(new RegistersWidget(this))
, memoryWidget_(new MemoryWidget(this, controller))
, peripheralsTabWidget_(new PeripheralsTabWidget(this)) {
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

    // Following code is only for ui testing purposes and will eventually be removed
    GPIOWidget *gpioWidget = new GPIOWidget(this);
    UARTWidget *uartWidget = new UARTWidget(this);

    gpioWidget->addPort(new GPIOPortWidget(gpioWidget, "PORT_A", {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}));
    gpioWidget->addPort(new GPIOPortWidget(gpioWidget, "PORT_B", {0, 1,    3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}));
    gpioWidget->addPort(new GPIOPortWidget(gpioWidget, "PORT_C", {13, 14, 15}));

    gpioWidget->setPinMode("PORT_A", 4, GPIO_PinMode::kOutput);
    gpioWidget->setPinMode("PORT_B", 0, GPIO_PinMode::kOutput);
    gpioWidget->setPinStatus("PORT_B", 0, true);

    peripheralsTabWidget_->addPeripheralWidget(gpioWidget, "GPIO", true);
    peripheralsTabWidget_->addPeripheralWidget(uartWidget, "UART");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setRunning(bool running)
{
    this->running = running;

    updateToolBarButtons();
    updateMemoryWidgetEnabled();
    updateRegistersWidgetEnabled();

    ui->line->setVisible(!running);
    ui->runningIndicator->setVisible(running);
    ui->statusbar->showMessage(running ? "Running ..." : "");
}

void MainWindow::setDebug(bool debug)
{
    this->debug = debug;
    setRunning(debug);

    updateMemory();
    updateRegisters();

    disassemblyWidget->highlightLine(debug ? 0 : -1);

    if (debug)
    {
        ui->runningIndicator->setVisible(false);
    }

    ui->debugIndicator->setVisible(debug);
    ui->statusbar->showMessage(debug ? "Running in debug mode ..." : "");
}

void MainWindow::updateMemoryWidgetEnabled()
{
    if (this->running && !this->debug)
    {
        ui->memoryWidget->setEnabled(false);
        return;
    }

    ui->memoryWidget->setEnabled(true);
}

void MainWindow::updateRegistersWidgetEnabled()
{
    if (this->running && !this->debug)
    {
        ui->registersWidget->setEnabled(false);
        return;
    }

    ui->registersWidget->setEnabled(true);
}

void MainWindow::updateRegisters() {
    registersWidget_->setRegisters(controller->GetRegisters());
}

void MainWindow::updateMemory() {
    memoryWidget_->updateMemory();
}

void MainWindow::updateToolBarButtons()
{
    ui->btnRun->setEnabled(!running && controller->IsFileLoaded());
    ui->btnRun->setVisible(!running);

    ui->btnDebug->setEnabled(!running && controller->IsFileLoaded());
    ui->btnDebug->setVisible(!running);

    ui->btnStep->setEnabled(debug);
    ui->btnStep->setVisible(debug);

    ui->btnTerminate->setEnabled(running);
    ui->btnTerminate->setVisible(running);

    ui->btnContinue->setEnabled(debug);
    ui->btnContinue->setVisible(debug);
}

void MainWindow::on_action_Open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select binary file", ".");
    ui->statusbar->showMessage(fileName);
    //If no file was selected
    if (fileName.isEmpty()) {
        return;
    }
    controller->LoadFile(fileName.toStdString());
    disassemblyWidget->addInstructionsList(controller->GetDisassembly());
    updateToolBarButtons();
}

void MainWindow::on_action_About_RISCVEmulator_triggered()
{
    AboutWindow aboutWindow;
    aboutWindow.setModal(true);
    aboutWindow.exec();
}

void MainWindow::on_btnRun_clicked()
{
    setRunning(true);
    int exitCode = controller->RunProgram();
    updateRegisters();
    on_btnTerminate_clicked();
    ui->statusbar->showMessage(QString::fromStdString("Program has exited with code: " + std::to_string(exitCode)));
}

void MainWindow::on_btnDebug_clicked()
{
    setDebug(true);
    updateRegisters();
}

void MainWindow::on_btnStep_clicked()
{
    updateMemory();
    updateRegisters();

    disassemblyWidget->highlightLine(disassemblyWidget->getHighlightedLine() + 1);

    if (disassemblyWidget->getHighlightedLine() == disassemblyWidget->getInstructionCount())
    {
        on_btnTerminate_clicked();
    }
}

void MainWindow::on_btnTerminate_clicked()
{
    setDebug(false);
    updateRegisters();
}
