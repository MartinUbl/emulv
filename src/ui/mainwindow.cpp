#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutwindow.h"

#include <QAction>
#include <QMessageBox>
#include <QStringListModel>

#include <sstream>
#include <iomanip>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , disassemblyWidget(new DisassemblyWidget)
{
    ui->setupUi(this);

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    ui->listViewRegisters->setFont(font);
    ui->lineEditMemoryHeader->setFont(font);
    ui->textEditMemory->setFont(font);
    ui->spinBoxMemoryFrom->setFont(font);
    ui->spinBoxMemoryTo->setFont(font);
    ui->textEditMemory->setFont(font);

    ui->btnTerminate->setIcon(QIcon(":img/terminate.png"));
    ui->btnContinue->setIcon(QIcon(":img/continue.png"));
    ui->btnRun->setIcon(QIcon(":img/run.png"));
    ui->btnDebug->setIcon(QIcon(":img/debug.png"));
    ui->btnStep->setIcon(QIcon(":img/step.png"));

    ui->btnSelectMemory->setIcon(QIcon(":img/search.png"));
    ui->btnRestoreMemory->setIcon(QIcon(":img/restore.png"));

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
    ui->btnSelectMemory->setIconSize(QSize(14, 14));
    ui->btnRestoreMemory->setIconSize(QSize(14, 14));

    ui->splitterTop->setStretchFactor(0, 1);
    ui->splitterTop->setStretchFactor(1, 0);

    ui->splitterBottom->setStretchFactor(0, 2);
    ui->splitterBottom->setStretchFactor(1, 5);

    ui->splitterMain->setStretchFactor(0, 3);
    ui->splitterMain->setStretchFactor(1, 1);

    ui->runningIndicator->setVisible(false);
    ui->debugIndicator->setVisible(false);

    connect(ui->action_Serial_monitor, SIGNAL(toggled(bool)), this, SLOT(setUARTTabVisible()));
    connect(ui->action_GPIO, SIGNAL(toggled(bool)), this, SLOT(setGPIOTabVisible()));
    connect(ui->action_Output, SIGNAL(toggled(bool)), this, SLOT(setOutputTabVisible()));

    updateMemorySpinBoxes();
    updateToolBarButtons();

    ui->disassemblyLayout->addWidget(disassemblyWidget);

    for (int i = 0; i < 100; i++)
    {
        disassemblyWidget->addInstruction("00000", " ");
    }
}

MainWindow::MainWindow(Controller *pController) : MainWindow(){
    this->controller = pController;
}

MainWindow::MainWindow(QWidget *parent, Controller *pController) : MainWindow(parent) {
    this->controller = pController;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete disassemblyWidget;
}

void MainWindow::setUARTTabVisible()
{
    ui->tabWidget->setTabVisible(0, ui->action_Serial_monitor->isChecked());
    updatePeripheralTabWidgetVisible();
}

void MainWindow::setGPIOTabVisible()
{
    ui->tabWidget->setTabVisible(1, ui->action_GPIO->isChecked());
    updatePeripheralTabWidgetVisible();
}

void MainWindow::setOutputTabVisible()
{
    ui->tabWidget->setTabVisible(2, ui->action_Output->isChecked());
    updatePeripheralTabWidgetVisible();
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

    updateTextEditMemory();
    updateListViewRegisters();

    disassemblyWidget->highlightLine(debug ? 0 : -1);

    if (debug)
    {
        ui->runningIndicator->setVisible(false);
    }

    ui->debugIndicator->setVisible(debug);
    ui->statusbar->showMessage(debug ? "Running in debug mode ..." : "");
}

std::string MainWindow::generateByte(bool hex)
{
    std::stringstream ssByte;

    if (hex)
    {
        ssByte << std::uppercase << std::hex << std::setw(2);
    }
    else
    {
        ssByte << std::setw(3);
    }

    ssByte << std::setfill('0') << (rand() % 256);

    return ssByte.str();
}

std::string MainWindow::generateBytes(int count, bool hex)
{
    std::string s = "";

    for (int i = 0; i < count; ++i)
    {
        s += " " + generateByte(hex);
    }

    return s;
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

void MainWindow::updatePeripheralTabWidgetVisible()
{
    for (int i = 0; i < ui->tabWidget->count(); ++i)
    {
        if (ui->tabWidget->isTabVisible(i))
        {
            ui->tabWidget->setVisible(true);
            return;
        }
    }

    ui->tabWidget->setVisible(false);
}

void MainWindow::updateMemoryButtons()
{
    bool enabled = ui->spinBoxMemoryFrom->value() != this->memoryFrom ||
                   ui->spinBoxMemoryTo->value() != this->memoryTo;

    ui->btnSelectMemory->setEnabled(enabled);
    ui->btnRestoreMemory->setEnabled(enabled);
}

void MainWindow::updateMemoryHeader()
{
    if (ui->rbMemoryDec->isChecked())
    {
        ui->lineEditMemoryHeader->setText("          00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F");
        ui->textEditMemory->setMinimumWidth(600);
    }
    else
    {
        ui->lineEditMemoryHeader->setText("         00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");
        ui->textEditMemory->setMinimumWidth(480);
    }
}

void MainWindow::updateTextEditMemory()
{
    std::string s = "";

    for (int i = this->memoryFrom; i <= this->memoryTo; ++i)
    {
        std::stringstream ss;
        ss << std::uppercase << std::hex << std::setw(7) << std::setfill('0') << i;

        s += ss.str() + "0";
        s += generateBytes(16, ui->rbMemoryHex->isChecked());
        s += "\n";
    }

    QString str = QString::fromStdString(s);
    ui->textEditMemory->setPlainText(str);
}

void MainWindow::updateListViewRegisters()
{
    auto model = new QStringListModel(this);

    QStringList list;

    for (int i = 0; i < 32; ++i) {
        std::string s = "R" + std::to_string(i) + ":  ";
        s += generateBytes(4, ui->rbRegistersHex->isChecked());
        QString str = QString::fromStdString(s);
        list << str;
    }

    model->setStringList(list);
    ui->listViewRegisters->setModel(model);
    ui->listViewRegisters->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::updateMemorySpinBoxes()
{
    ui->spinBoxMemoryFrom->setValue(this->memoryFrom);
    ui->spinBoxMemoryTo->setValue(this->memoryTo);
}

void MainWindow::updateToolBarButtons()
{
    ui->btnRun->setEnabled(!running);
    ui->btnRun->setVisible(!running);

    ui->btnDebug->setEnabled(!running);
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
    controller->LoadFile(fileName.toStdString());
    disassemblyWidget->addInstructionsList(controller->GetDisassembly());
}

void MainWindow::on_action_About_RISCVEmulator_triggered()
{
    AboutWindow aboutWindow;
    aboutWindow.setModal(true);
    aboutWindow.exec();
}

void MainWindow::on_spinBoxMemoryFrom_valueChanged(int arg1)
{
    ui->spinBoxMemoryTo->setMinimum(ui->spinBoxMemoryFrom->value());
    updateMemoryButtons();
}

void MainWindow::on_spinBoxMemoryTo_valueChanged(int arg1)
{
    ui->spinBoxMemoryFrom->setMaximum(ui->spinBoxMemoryTo->value());
    updateMemoryButtons();
}

void MainWindow::on_btnRestoreMemory_clicked()
{
    updateMemorySpinBoxes();
}

void MainWindow::on_btnSelectMemory_clicked()
{
    this->memoryFrom = ui->spinBoxMemoryFrom->value();
    this->memoryTo = ui->spinBoxMemoryTo->value();

    updateMemorySpinBoxes();
    updateMemoryButtons();
    updateTextEditMemory();
}

void MainWindow::on_lineEditSendMessage_textChanged(const QString &arg1)
{
    ui->btnSendMessage->setEnabled(!ui->lineEditSendMessage->text().isEmpty());
}

void MainWindow::on_btnRun_clicked()
{
    if(!controller->IsFileLoaded()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Error");
        QString msg = QString::fromStdString("Cannot run program because no file has been loaded. Please use the \"File -> Open\" button in order to proceed.");
        msgBox.setText(msg);
        msgBox.exec();
        return;
    }

    setRunning(true);
    int exitCode = controller->RunProgram();
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("Program Exit Status");
    QString msg = QString::fromStdString("Program has exited with code: " + std::to_string(exitCode));
    msgBox.setText(msg);
    msgBox.exec();
    on_btnTerminate_clicked();
}

void MainWindow::on_btnDebug_clicked()
{
    if(!controller->IsFileLoaded()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Error");
        QString msg = QString::fromStdString("Cannot debug program because no file has been loaded. Please use the \"File -> Open\" button in order to proceed.");
        msgBox.setText(msg);
        msgBox.exec();
        return;
    }

    setDebug(true);
}

void MainWindow::on_btnStep_clicked()
{
    updateTextEditMemory();
    updateListViewRegisters();

    disassemblyWidget->highlightLine(disassemblyWidget->getHighlightedLine() + 1);

    if (disassemblyWidget->getHighlightedLine() == disassemblyWidget->getInstructionCount())
    {
        on_btnTerminate_clicked();
    }
}

void MainWindow::on_btnTerminate_clicked()
{
    setDebug(false);
}

void MainWindow::on_rbMemoryDec_clicked()
{
    updateMemoryHeader();
    updateTextEditMemory();
}

void MainWindow::on_rbMemoryHex_clicked()
{
    updateMemoryHeader();
    updateTextEditMemory();
}

void MainWindow::on_rbRegistersDec_clicked()
{
    updateListViewRegisters();
}

void MainWindow::on_rbRegistersHex_clicked()
{
    updateListViewRegisters();
}

