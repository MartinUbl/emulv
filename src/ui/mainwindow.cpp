#include "mainwindow.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <QFileDialog>
#include <QMenuBar>
#include <QStatusBar>
#include <QAction>
#include <QStringListModel>
#include <QLayout>
#include <QSpacerItem>
#include <QMessageBox>

#include "aboutwindow.h"
#include "uart_event.h"

MainWindow::MainWindow(QWidget *parent, Controller *controller)
: QMainWindow(parent)
, controller_(controller) {
    SetupUI();
    UpdateUI();

    peripherals_tab_widget_->UpdateWidgets();

    memory_widget_->SetAddressRangeLimit(controller->GetRamStartAddress(), controller->GetRamEndAddress());

    controller->GetEventEmitter().On(emulator::State_Changed_Event_Description, [this](AbstractEvent *res) {
        QMetaObject::invokeMethod(this, "UpdateUI");
    });
}

MainWindow::~MainWindow() {
    controller_->TerminateProgram();
    JoinThread();
}

void MainWindow::SetupUI() {
    setWindowTitle("RISC-V Emulator");
    setWindowIcon(QIcon(":img/logo.ico"));
    setWindowState(Qt::WindowMaximized);

    // <Menu>
    setMenuBar(new QMenuBar(this));

    auto fileMenu = new QMenu("&File", menuBar());
    auto openAction = new QAction("&Open", fileMenu);
    openAction->setShortcut(QKeySequence::fromString("Ctrl+o"));
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(OnFileOpenTriggered()));
    fileMenu->addAction(openAction);

    auto toolsMenu = new QMenu("&Tools", menuBar());
    auto selectConfigAction = new QAction("&Select configuration", toolsMenu);
    connect(selectConfigAction, SIGNAL(triggered(bool)), this, SLOT(OnSelectConfigurationTriggered()));
    auto clearConfigAction = new QAction("&Clear configuration", toolsMenu);
    connect(clearConfigAction, SIGNAL(triggered(bool)), this, SLOT(OnClearConfigurationTriggered()));
    toolsMenu->addAction(selectConfigAction);
    toolsMenu->addAction(clearConfigAction);

    auto helpMenu = new QMenu("&Help", menuBar());
    auto aboutAction = new QAction("&About", helpMenu);
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(OnAboutTriggered()));
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

    btn_run_ = new QToolButton(toolBar);
    btn_run_->setText("Run");
    btn_run_->setToolTip("Run (F5)");
    btn_run_->setShortcut(QKeySequence::fromString("F5"));
    btn_run_->setIcon(QIcon(":img/run.png"));
    btn_run_->setFixedSize(btnSize);
    btn_run_->setIconSize(QSize(13, 13));

    btn_debug_ = new QToolButton(toolBar);
    btn_debug_->setText("Debug");
    btn_debug_->setToolTip("Debug (F6)");
    btn_debug_->setShortcut(QKeySequence::fromString("F6"));
    btn_debug_->setIcon(QIcon(":img/debug.png"));
    btn_debug_->setFixedSize(btnSize);
    btn_debug_->setIconSize(QSize(16, 16));

    btn_terminate_ = new QToolButton(toolBar);
    btn_terminate_->setText("TerminateProgram");
    btn_terminate_->setToolTip("TerminateProgram (Shift + F5)");
    btn_terminate_->setShortcut(QKeySequence::fromString("Shift+F5"));
    btn_terminate_->setIcon(QIcon(":img/terminate.png"));
    btn_terminate_->setFixedSize(btnSize);
    btn_terminate_->setIconSize(QSize(11, 11));

    btn_step_ = new QToolButton(toolBar);
    btn_step_->setText("Step");
    btn_step_->setToolTip("Step (F7)");
    btn_step_->setShortcut(QKeySequence::fromString("F7"));
    btn_step_->setIcon(QIcon(":img/step.png"));
    btn_step_->setFixedSize(btnSize);
    btn_step_->setIconSize(QSize(16, 16));

    btn_continue_ = new QToolButton(toolBar);
    btn_continue_->setText("Continue");
    btn_continue_->setToolTip("Continue (F8)");
    btn_continue_->setShortcut(QKeySequence::fromString("F8"));
    btn_continue_->setIcon(QIcon(":img/continue.png"));
    btn_continue_->setFixedSize(btnSize);
    btn_continue_->setIconSize(QSize(13, 13));

    connect(btn_run_, SIGNAL(clicked(bool)), this, SLOT(OnRunClicked()));
    connect(btn_debug_, SIGNAL(clicked(bool)), this, SLOT(OnDebugClicked()));
    connect(btn_terminate_, SIGNAL(clicked(bool)), this, SLOT(OnTerminateClicked()));
    connect(btn_step_, SIGNAL(clicked(bool)), this, SLOT(OnStepClicked()));
    connect(btn_continue_, SIGNAL(clicked(bool)), this, SLOT(OnContinueClicked()));

    toolBar->layout()->addWidget(btn_run_);
    toolBar->layout()->addWidget(btn_debug_);
    toolBar->layout()->addWidget(btn_terminate_);
    toolBar->layout()->addWidget(btn_step_);
    toolBar->layout()->addWidget(btn_continue_);
    toolBar->layout()->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
    centralLayout->addWidget(toolBar);
    //   </ToolBar>

    //   <DebugIndicator>
    debug_indicator_ = new QFrame(centralWidget);
    debug_indicator_->setStyleSheet("border: 3px solid rgb(255, 128, 0);");
    debug_indicator_->setFrameShape(QFrame::Box);
    debug_indicator_->setFrameShadow(QFrame::Plain);
    debug_indicator_->setFixedHeight(2);
    debug_indicator_->setVisible(false);
    centralLayout->addWidget(debug_indicator_);
    //   </DebugIndicator>

    //   <RunIndicator>
    running_indicator_ = new QFrame(centralWidget);
    running_indicator_->setStyleSheet("border: 3px solid green;");
    running_indicator_->setFrameShape(QFrame::Box);
    running_indicator_->setFrameShadow(QFrame::Plain);
    running_indicator_->setFixedHeight(2);
    running_indicator_->setVisible(false);
    centralLayout->addWidget(running_indicator_);
    //   </RunIndicator>

    //   <MainSplitter>
    main_splitter_ = new QSplitter(centralWidget);
    main_splitter_->setOrientation(Qt::Vertical);
    main_splitter_->setChildrenCollapsible(false);

    //     <TopSplitter>
    top_splitter_ = new QSplitter(main_splitter_);
    top_splitter_->setChildrenCollapsible(false);

    disassembly_widget_ = new DisassemblyWidget(this, controller_);
    registers_widget_ = new RegistersWidget(this);

    top_splitter_->addWidget(disassembly_widget_);
    top_splitter_->addWidget(registers_widget_);

    top_splitter_->setStretchFactor(0, 1);
    top_splitter_->setStretchFactor(1, 0);
    //     </TopSplitter>

    //     <BotSplitter>
    bot_splitter_ = new QSplitter(main_splitter_);
    bot_splitter_->setChildrenCollapsible(false);

    memory_widget_ = new MemoryWidget(this, controller_);
    peripherals_tab_widget_ = new PeripheralsTabWidget(this, controller_);

    bot_splitter_->addWidget(memory_widget_);
    bot_splitter_->addWidget(peripherals_tab_widget_);

    bot_splitter_->setStretchFactor(0, 2);
    bot_splitter_->setStretchFactor(1, 1);
    //     </BotSplitter>

    main_splitter_->addWidget(top_splitter_);
    main_splitter_->addWidget(bot_splitter_);

    main_splitter_->setStretchFactor(0, 2);
    main_splitter_->setStretchFactor(1, 1);
    centralLayout->addWidget(main_splitter_);
    //   </MainSplitter>

    // <StatusBar>
    setStatusBar(new QStatusBar(this));
    statusBar()->setStyleSheet("QLabel { margin-left: 4px; margin-right: 4px; }");
    lbl_file_ = new QLabel(kDefaultFileLabel, statusBar());
    lbl_config_ = new QLabel(kDefaultConfigLabel, statusBar());
    lbl_program_status_ = new QLabel(statusBar());
    statusBar()->addWidget(lbl_file_);
    statusBar()->addWidget(lbl_config_);
    statusBar()->addWidget(lbl_program_status_);
    // </StatusBar>

    // </CentralWidget>
    setCentralWidget(centralWidget);
}

void MainWindow::UpdateUI() {
    UpdateRunningIndicator();
    UpdateWidgetsEnabled();
    UpdateToolbarButtons();
    UpdateProgramStatusLabel();

    disassembly_widget_->HighlightLine(-1);

    auto state = controller_->GetProgramState();

    if (state == emulator::kTerminated) {
        UpdateRegisters();
        UpdateMemory();
    }
    else if (state == emulator::kDebugPaused) {
        UpdateRegisters();
        UpdateMemory();
        disassembly_widget_->HighlightLine(controller_->GetPc());
    }
}

void MainWindow::UpdateRunningIndicator() {
    switch (controller_->GetProgramState()) {
        case emulator::kRunning:
            running_indicator_->setVisible(true);
            debug_indicator_->setVisible(false);
            break;
        case emulator::kRunningDebug:
        case emulator::kDebugPaused:
            running_indicator_->setVisible(false);
            debug_indicator_->setVisible(true);
            break;
        default:
            running_indicator_->setVisible(false);
            debug_indicator_->setVisible(false);
            break;
    }
}

void MainWindow::UpdateWidgetsEnabled() {
    switch (controller_->GetProgramState()) {
        case emulator::kRunning:
        case emulator::kRunningDebug:
            memory_widget_->setEnabled(false);
            registers_widget_->setEnabled(false);
            peripherals_tab_widget_->setEnabled(true);
            peripherals_tab_widget_->SetReadonly(false);
            break;
        case emulator::kDebugPaused:
            memory_widget_->setEnabled(true);
            registers_widget_->setEnabled(true);
            peripherals_tab_widget_->setEnabled(true);
            peripherals_tab_widget_->SetReadonly(false);
            break;
        case emulator::kTerminated:
            memory_widget_->setEnabled(true);
            registers_widget_->setEnabled(true);
            peripherals_tab_widget_->setEnabled(true);
            peripherals_tab_widget_->SetReadonly(true);
            break;
        default:
            memory_widget_->setEnabled(false);
            registers_widget_->setEnabled(false);
            peripherals_tab_widget_->setEnabled(false);
            peripherals_tab_widget_->SetReadonly(true);
            break;
    }
}

void MainWindow::UpdateToolbarButtons() {
    std::vector<QToolButton *> toolbar_buttons = {btn_run_, btn_debug_, btn_terminate_, btn_step_, btn_continue_};

    // Buttons enabled (in order of toolbar_buttons)
    std::unordered_map<emulator::EmulatorState, std::vector<bool>> buttons_enabled = {
            {emulator::kDefault,      {false, false, false, false, false}},
            {emulator::kReady,        { true,  true, false, false, false}},
            {emulator::kRunning,      {false, false,  true, false, false}},
            {emulator::kRunningDebug, {false, false,  true, false, false}},
            {emulator::kDebugPaused,  {false, false,  true,  true,  true}},
            {emulator::kTerminated,   { true,  true, false, false, false}},
    };

    // Buttons visible (in order of toolbar_buttons)
    std::unordered_map<emulator::EmulatorState, std::vector<bool>> buttons_visible = {
            {emulator::kDefault,      { true,  true, false, false, false}},
            {emulator::kReady,        { true,  true, false, false, false}},
            {emulator::kRunning,      {false, false,  true,  true,  true}},
            {emulator::kRunningDebug, {false, false,  true,  true,  true}},
            {emulator::kDebugPaused,  {false, false,  true,  true,  true}},
            {emulator::kTerminated,   { true,  true, false, false, false}},
    };

    auto state = controller_->GetProgramState();

    for (size_t i = 0; i < toolbar_buttons.size(); ++i) {
        toolbar_buttons[i]->setEnabled(buttons_enabled[state][i]);
        toolbar_buttons[i]->setVisible(buttons_visible[state][i]);
    }
}

void MainWindow::UpdateProgramStatusLabel() {
    std::string string_pc;

    switch (controller_->GetProgramState()) {
        case emulator::kRunning:
            lbl_program_status_->setText("Running ...");
            break;
        case emulator::kRunningDebug:
            lbl_program_status_->setText("Running in debug mode ...");
            break;
        case emulator::kTerminated:
            lbl_program_status_->setText(
                    QString::fromStdString("Program finished with value " + std::to_string(controller_->GetProgramReturnValue())));
            break;
        case emulator::kDebugPaused:
            string_pc = FormatAddress(controller_->GetPc());
            lbl_program_status_->setText(
                    QString::fromStdString("Program hit breakpoint " + string_pc));
            break;
        default:
            break;
    }
}

void MainWindow::UpdateRegisters() {
    registers_widget_->SetRegisters(controller_->GetRegisters());
}

void MainWindow::UpdateMemory() const {
    memory_widget_->UpdateMemory();
}

void MainWindow::OpenFile(const std::string& path) {
    if (path.empty()) {
        return;
    }

    controller_->TerminateProgram();
    JoinThread();

    try {
        controller_->LoadFile(path);
        disassembly_widget_->SetInstructions(controller_->GetDisassembly());
    }
    catch (const std::exception &e) {
        ShowMessageBox("File error", QString::fromStdString("Could not load file " + path + '\n' + e.what()));
        return;
    }

    memory_widget_->Clear();
    registers_widget_->SetRegisters({});
    controller_->ResetPeripherals();
    peripherals_tab_widget_->UpdateWidgets();

    lbl_file_->setText(QString::fromStdString(path));
}

void MainWindow::SelectConfig(const std::string& path) {
    if (path.empty()) {
        return;
    }

    controller_->TerminateProgram();
    JoinThread();

    try {
        controller_->ConfigureEmulator(path);
    }
    catch (const std::exception &e) {
        ShowMessageBox("File error", QString::fromStdString("Could not load file " + path + '\n' + e.what()));
        return;
    }

    peripherals_tab_widget_->UpdateWidgets();
    UpdateUI();

    memory_widget_->SetAddressRangeLimit(controller_->GetRamStartAddress(), controller_->GetRamEndAddress());
    memory_widget_->spinbox_memory_from_->setValue(controller_->GetRamStartAddress());
    memory_widget_->spinbox_memory_to_->setValue(controller_->GetRamEndAddress());

    lbl_config_->setText(QString::fromStdString(path));
}

void MainWindow::ClearConfig() {
    controller_->TerminateProgram();
    JoinThread();

    controller_->ClearActivePeripherals();
    peripherals_tab_widget_->UpdateWidgets();

    lbl_config_->setText(kDefaultConfigLabel);
}

void MainWindow::OnFileOpenTriggered() {
    QString fileName = QFileDialog::getOpenFileName(this, "Select binary file", ".");

    OpenFile(fileName.toStdString());
}

void MainWindow::OnSelectConfigurationTriggered() {
    QString fileName = QFileDialog::getOpenFileName(this, "Select config file", ".", "*.json");

    SelectConfig(fileName.toStdString());
}

void MainWindow::OnClearConfigurationTriggered() {
    ClearConfig();
}

void MainWindow::OnAboutTriggered() {
    AboutWindow aboutWindow;
    aboutWindow.setModal(true);
    aboutWindow.exec();
}

void MainWindow::OnRunClicked() {
    JoinThread();
    controller_->ResetPeripherals();
    peripherals_tab_widget_->UpdateWidgets();
    thread_ = std::make_unique<std::thread>(&Controller::RunProgram, controller_);
}

void MainWindow::OnDebugClicked() {
    JoinThread();
    controller_->ResetPeripherals();
    peripherals_tab_widget_->UpdateWidgets();
    thread_ = std::make_unique<std::thread>(&Controller::DebugProgram, controller_);
}

void MainWindow::OnTerminateClicked() {
    controller_->TerminateProgram();
    JoinThread();

    UpdateRegisters();
    UpdateMemory();
}

void MainWindow::OnStepClicked() {
    controller_->DebugStep();

    UpdateRegisters();
    UpdateMemory();

    uint64_t pc = controller_->GetPc();
    disassembly_widget_->HighlightLine(pc);

    std::string pc_str = FormatAddress(pc);
    lbl_program_status_->setText(QString::fromStdString("Program stepped to " + pc_str));
}

void MainWindow::OnContinueClicked() {
    JoinThread();
    thread_ = std::make_unique<std::thread>(&Controller::DebugContinue, controller_);
}

std::string MainWindow::FormatAddress(uint64_t address) {
    std::stringstream ss;
    ss << std::uppercase << std::hex << std::setw(kAddressWidth) << std::setfill('0') << address;
    return ss.str();
}

void MainWindow::ShowMessageBox(const QString &title, const QString &message) {
    QMessageBox message_box;
    message_box.setWindowTitle(title);
    message_box.setText(message);
    message_box.exec();
}

void MainWindow::JoinThread() {
    if (thread_ && thread_->joinable()) {
        thread_->join();
    }
}
