#include "mainwindow.h"
#include "aboutwindow.h"
#include "../utils/events/uart_event.h"

#include <QMenuBar>
#include <QStatusBar>
#include <QAction>
#include <QStringListModel>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent, Controller *controller)
: QMainWindow(parent)
, controller(controller) {
    setupUI();

    updateUI();

    peripheralsTabWidget_->updateWidgets();

    memoryWidget_->setEnabled(false);
    registersWidget_->setEnabled(false);
    peripheralsTabWidget_->setEnabled(false);

    memoryWidget_->setAddressRangeLimit(controller->GetRamStartAddress(), controller->GetRamEndAddress());

    controller->GetEventEmitter().On(emulator::State_Changed_Event_Description, [this](AbstractEvent *res) {
        QMetaObject::invokeMethod(this, "updateUI");
    });
}

MainWindow::~MainWindow() {
    controller->Terminate();
    joinThread();
}

void MainWindow::setupUI() {
    setWindowTitle("RISC-V Emulator");
    setWindowIcon(QIcon(":img/logo.ico"));
    setWindowState(Qt::WindowMaximized);

    // <Menu>
    setMenuBar(new QMenuBar(this));

    auto fileMenu = new QMenu("&File", menuBar());
    auto openAction = new QAction("&Open", fileMenu);
    openAction->setShortcut(QKeySequence::fromString("Ctrl+o"));
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(on_action_Open_triggered()));
    fileMenu->addAction(openAction);

    auto toolsMenu = new QMenu("&Tools", menuBar());
    auto selectConfigAction = new QAction("&Select configuration", toolsMenu);
    connect(selectConfigAction, SIGNAL(triggered(bool)), this, SLOT(on_action_SelectConfiguration_triggered()));
    auto clearConfigAction = new QAction("&Clear configuration", toolsMenu);
    connect(clearConfigAction, SIGNAL(triggered(bool)), this, SLOT(on_action_ClearConfiguration_triggered()));
    toolsMenu->addAction(selectConfigAction);
    toolsMenu->addAction(clearConfigAction);

    auto helpMenu = new QMenu("&Help", menuBar());
    auto aboutAction = new QAction("&About", helpMenu);
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(on_action_About_RISCVEmulator_triggered()));
    helpMenu->addAction(aboutAction);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(toolsMenu);
    menuBar()->addMenu(helpMenu);
    // </Menu>

    // <CentralWidget>
    auto centralWidget = new QWidget(this);
    auto centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(6, 0, 6, 0);

    //   <ToolBar>
    auto toolBar = new QWidget(centralWidget);
    toolBar->setLayout(new QHBoxLayout(toolBar));
    toolBar->layout()->setSpacing(3);
    toolBar->layout()->setContentsMargins(3, 2, 0, 2);

    toolBar->setStyleSheet("QWidget#toolBar { background-color: rgba(100, 100, 100, 25); }"
                           "QToolButton { background-color: rgba(100, 100, 100, 0); border: 0px; border-radius: 3px; }"
                           "QToolButton:hover { background-color: rgba(100, 100, 100, 100);}");

    QSize btnSize(24, 24);

    btnRun = new QToolButton(toolBar);
    btnRun->setText("Run");
    btnRun->setToolTip("Run (F5)");
    btnRun->setShortcut(QKeySequence::fromString("F5"));
    btnRun->setIcon(QIcon(":img/run.png"));
    btnRun->setFixedSize(btnSize);
    btnRun->setIconSize(QSize(13, 13));

    btnDebug = new QToolButton(toolBar);
    btnDebug->setText("Debug");
    btnDebug->setToolTip("Debug (F6)");
    btnDebug->setShortcut(QKeySequence::fromString("F6"));
    btnDebug->setIcon(QIcon(":img/debug.png"));
    btnDebug->setFixedSize(btnSize);
    btnDebug->setIconSize(QSize(16, 16));

    btnTerminate = new QToolButton(toolBar);
    btnTerminate->setText("Terminate");
    btnTerminate->setToolTip("Terminate (Shift + F5)");
    btnTerminate->setShortcut(QKeySequence::fromString("Shift+F5"));
    btnTerminate->setIcon(QIcon(":img/terminate.png"));
    btnTerminate->setFixedSize(btnSize);
    btnTerminate->setIconSize(QSize(11, 11));

    btnStep = new QToolButton(toolBar);
    btnStep->setText("Step");
    btnStep->setToolTip("Step (F7)");
    btnStep->setShortcut(QKeySequence::fromString("F7"));
    btnStep->setIcon(QIcon(":img/step.png"));
    btnStep->setFixedSize(btnSize);
    btnStep->setIconSize(QSize(16, 16));

    btnContinue = new QToolButton(toolBar);
    btnContinue->setText("Continue");
    btnContinue->setToolTip("Continue (F8)");
    btnContinue->setShortcut(QKeySequence::fromString("F8"));
    btnContinue->setIcon(QIcon(":img/continue.png"));
    btnContinue->setFixedSize(btnSize);
    btnContinue->setIconSize(QSize(13, 13));

    connect(btnRun, SIGNAL(clicked(bool)), this, SLOT(on_btnRun_clicked()));
    connect(btnDebug, SIGNAL(clicked(bool)), this, SLOT(on_btnDebug_clicked()));
    connect(btnTerminate, SIGNAL(clicked(bool)), this, SLOT(on_btnTerminate_clicked()));
    connect(btnStep, SIGNAL(clicked(bool)), this, SLOT(on_btnStep_clicked()));
    connect(btnContinue, SIGNAL(clicked(bool)), this, SLOT(on_btnContinue_clicked()));

    toolBar->layout()->addWidget(btnRun);
    toolBar->layout()->addWidget(btnDebug);
    toolBar->layout()->addWidget(btnTerminate);
    toolBar->layout()->addWidget(btnStep);
    toolBar->layout()->addWidget(btnContinue);
    toolBar->layout()->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
    centralLayout->addWidget(toolBar);
    //   </ToolBar>

    //   <DebugIndicator>
    debugIndicator = new QFrame(centralWidget);
    debugIndicator->setStyleSheet("border: 3px solid rgb(255, 128, 0);");
    debugIndicator->setFrameShape(QFrame::Box);
    debugIndicator->setFrameShadow(QFrame::Plain);
    debugIndicator->setFixedHeight(2);
    debugIndicator->setVisible(false);
    centralLayout->addWidget(debugIndicator);
    //   </DebugIndicator>

    //   <RunIndicator>
    runningIndicator = new QFrame(centralWidget);
    runningIndicator->setStyleSheet("border: 3px solid green;");
    runningIndicator->setFrameShape(QFrame::Box);
    runningIndicator->setFrameShadow(QFrame::Plain);
    runningIndicator->setFixedHeight(2);
    runningIndicator->setVisible(false);
    centralLayout->addWidget(runningIndicator);
    //   </RunIndicator>

    //   <MainSplitter>
    mainSplitter = new QSplitter(centralWidget);
    mainSplitter->setOrientation(Qt::Vertical);
    mainSplitter->setChildrenCollapsible(false);

    //     <TopSplitter>
    topSplitter = new QSplitter(mainSplitter);
    topSplitter->setChildrenCollapsible(false);

    disassemblyWidget_ = new DisassemblyWidget(this, controller);
    registersWidget_ = new RegistersWidget(this);

    topSplitter->addWidget(disassemblyWidget_);
    topSplitter->addWidget(registersWidget_);

    topSplitter->setStretchFactor(0, 1);
    topSplitter->setStretchFactor(1, 0);
    //     </TopSplitter>

    //     <BotSplitter>
    botSplitter = new QSplitter(mainSplitter);
    botSplitter->setChildrenCollapsible(false);

    memoryWidget_ = new MemoryWidget(this, controller);
    peripheralsTabWidget_ = new PeripheralsTabWidget(this, controller);

    botSplitter->addWidget(memoryWidget_);
    botSplitter->addWidget(peripheralsTabWidget_);

    botSplitter->setStretchFactor(0, 2);
    botSplitter->setStretchFactor(1, 1);
    //     </BotSplitter>

    mainSplitter->addWidget(topSplitter);
    mainSplitter->addWidget(botSplitter);

    mainSplitter->setStretchFactor(0, 2);
    mainSplitter->setStretchFactor(1, 1);
    centralLayout->addWidget(mainSplitter);
    //   </MainSplitter>

    // <StatusBar>
    setStatusBar(new QStatusBar(this));
    statusBar()->setStyleSheet("QLabel { margin-left: 4px; margin-right: 4px; }");
    lblFile = new QLabel(kDefaultFileLabel, statusBar());
    lblConfig = new QLabel(kDefaultConfigLabel, statusBar());
    lblProgramStatus = new QLabel(statusBar());
    statusBar()->addWidget(lblFile);
    statusBar()->addWidget(lblConfig);
    statusBar()->addWidget(lblProgramStatus);
    // </StatusBar>

    // </CentralWidget>
    setCentralWidget(centralWidget);
}

void MainWindow::updateUI() {
    std::string pc_str;

    updateRunningIndicator();
    updateWidgetsEnabled();
    updateToolBarButtons();

    disassemblyWidget_->highlightLine(-1);

    switch (controller->GetProgramState()) {
        case emulator::kRunning:
            lblProgramStatus->setText("Running ...");
            break;
        case emulator::kRunningDebug:
            lblProgramStatus->setText("Running in debug mode ...");
            break;
        case emulator::kTerminated:
            lblProgramStatus->setText(QString::fromStdString("Program finished with value " + std::to_string(controller->GetProgramReturnValue())));
            updateRegisters();
            updateMemory();
            break;
        case emulator::kDebugPaused:
            pc_str = formatAddress_(controller->GetPc());
            lblProgramStatus->setText(QString::fromStdString("Program hit breakpoint " + pc_str));
            disassemblyWidget_->highlightLine(controller->GetPc());
            updateRegisters();
            updateMemory();
            break;
        default:
            break;
    }
}

void MainWindow::updateRunningIndicator() {
    switch (controller->GetProgramState()) {
        case emulator::kRunning:
            runningIndicator->setVisible(true);
            debugIndicator->setVisible(false);
            break;
        case emulator::kRunningDebug:
        case emulator::kDebugPaused:
            runningIndicator->setVisible(false);
            debugIndicator->setVisible(true);
            break;
        default:
            runningIndicator->setVisible(false);
            debugIndicator->setVisible(false);
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

    btnRun->setEnabled(state == emulator::kReady || state == emulator::kTerminated);
    btnRun->setVisible(notRunningButtonsVisible);

    btnDebug->setEnabled(state == emulator::kReady || state == emulator::kTerminated);
    btnDebug->setVisible(notRunningButtonsVisible);

    btnTerminate->setEnabled(runningButtonsVisible);
    btnTerminate->setVisible(runningButtonsVisible);

    btnStep->setEnabled(state == emulator::kDebugPaused);
    btnStep->setVisible(runningButtonsVisible);

    btnContinue->setEnabled(state == emulator::kDebugPaused);
    btnContinue->setVisible(runningButtonsVisible);
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

void MainWindow::openFile(std::string path) {
    if (path.empty()) {
        return;
    }

    controller->Terminate();
    joinThread();

    try {
        controller->LoadFile(path);
        disassemblyWidget_->setInstructions(controller->GetDisassembly());
    }
    catch (const std::exception &e) {
        showMessageBox("File error", QString::fromStdString("Could not load file " + path + '\n' + e.what()));
        return;
    }

    memoryWidget_->clear();
    registersWidget_->setRegisters({});
    controller->ResetPeripherals();
    peripheralsTabWidget_->updateWidgets();

    lblFile->setText(QString::fromStdString(path));
}

void MainWindow::selectConfig(std::string path) {
    if (path.empty()) {
        return;
    }

    controller->Terminate();
    joinThread();

    try {
        controller->ConfigureEmulator(path);
    }
    catch (const std::exception &e) {
        showMessageBox("File error", QString::fromStdString("Could not load file " + path + '\n' + e.what()));
        return;
    }

    peripheralsTabWidget_->updateWidgets();
    updateUI();

    memoryWidget_->setAddressRangeLimit(controller->GetRamStartAddress(), controller->GetRamEndAddress());
    memoryWidget_->sp_memory_from_->setValue(controller->GetRamStartAddress());
    memoryWidget_->sp_memory_to_->setValue(controller->GetRamEndAddress());

    lblConfig->setText(QString::fromStdString(path));
}

void MainWindow::clearConfig() {
    controller->Terminate();
    joinThread();

    controller->ClearActivePeripherals();
    peripheralsTabWidget_->updateWidgets();

    lblConfig->setText(kDefaultConfigLabel);
}

void MainWindow::on_action_Open_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, "Select binary file", ".");

    openFile(fileName.toStdString());
}

void MainWindow::on_action_SelectConfiguration_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, "Select config file", ".", "*.json");

    selectConfig(fileName.toStdString());
}

void MainWindow::on_action_ClearConfiguration_triggered() {
    clearConfig();
}

void MainWindow::on_action_About_RISCVEmulator_triggered() {
    AboutWindow aboutWindow;
    aboutWindow.setModal(true);
    aboutWindow.exec();
}

void MainWindow::on_btnRun_clicked() {
    joinThread();
    controller->ResetPeripherals();
    peripheralsTabWidget_->updateWidgets();
    mRun_Thread = std::make_unique<std::thread>(&Controller::RunProgram, controller);
}

void MainWindow::on_btnDebug_clicked() {
    joinThread();
    controller->ResetPeripherals();
    peripheralsTabWidget_->updateWidgets();
    mRun_Thread = std::make_unique<std::thread>(&Controller::DebugProgram, controller);
}

void MainWindow::on_btnStep_clicked() {
    controller->DebugStep();

    updateRegisters();
    updateMemory();

    uint64_t pc = controller->GetPc();
    disassemblyWidget_->highlightLine(pc);

    std::string pc_str = formatAddress_(pc);
    lblProgramStatus->setText(QString::fromStdString("Program stepped to " + pc_str));
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

std::string MainWindow::formatAddress_(uint64_t address) {
    std::stringstream ss;
    ss << std::uppercase << std::hex << std::setw(8) << std::setfill('0') << address;

    return ss.str();
}
