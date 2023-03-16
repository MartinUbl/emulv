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
{
    ui->setupUi(this);

    ui->splitterTop->setStretchFactor(0, 1);
    ui->splitterTop->setStretchFactor(1, 0);

    ui->splitterBottom->setStretchFactor(0, 2);
    ui->splitterBottom->setStretchFactor(1, 5);

    ui->splitterMain->setStretchFactor(0, 3);
    ui->splitterMain->setStretchFactor(1, 1);

    connect(ui->action_Serial_monitor, SIGNAL(toggled(bool)), this, SLOT(setUARTTabVisible()));
    connect(ui->action_GPIO, SIGNAL(toggled(bool)), this, SLOT(setGPIOTabVisible()));
    connect(ui->action_Output, SIGNAL(toggled(bool)), this, SLOT(setOutputTabVisible()));
    connect(ui->action_About_RISCVEmulator, SIGNAL(triggered(bool)), this, SLOT(showAbout()));

    updateMemorySpinBoxes();
    updateToolBarButtons();
}

MainWindow::~MainWindow()
{
    delete ui;
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


void MainWindow::updateMemoryWidgetEnabled() {
    if (this->running && !this->debug)
    {
        ui->memoryWidget->setEnabled(false);
        return;
    }

    ui->memoryWidget->setEnabled(true);
}

void MainWindow::updateRegistersWidgetEnabled() {
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

void MainWindow::updateTextEditMemory()
{
    std::string s = "";

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

    for (int i = this->memoryFrom; i <= this->memoryTo; ++i)
    {
        std::stringstream ss;
        ss << std::uppercase << std::hex << std::setw(7) << std::setfill('0') << i;
        s += ss.str() + "0";

        for (int j = 0; j < 16; ++j)
        {
            std::stringstream ssByte;
            if (ui->rbMemoryDec->isChecked())
            {
                ssByte << std::setw(3) << std::setfill('0') << (rand() % 256);
            }
            else
            {
                ssByte << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (rand() % 256);
            }

            s += " " + ssByte.str();
        }
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

        for (int j = 0; j < 4; ++j) {
            std::stringstream ssByte;
            if (ui->rbRegistersDec->isChecked())
            {
                ssByte << std::setw(3) << std::setfill('0') << (rand() % 256);
            }
            else
            {
                ssByte << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (rand() % 256);
            }
            s += " " + ssByte.str();
        }

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
}

void MainWindow::on_action_Open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select binary file", ".");
    ui->statusbar->showMessage(fileName);
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
    this->running = true;
    updateToolBarButtons();
    updateMemoryWidgetEnabled();
    updateRegistersWidgetEnabled();
}

void MainWindow::on_btnDebug_clicked()
{
    this->running = true;
    this->debug = true;
    updateToolBarButtons();
    updateMemoryWidgetEnabled();
    updateRegistersWidgetEnabled();
    updateTextEditMemory();
    updateListViewRegisters();
}

void MainWindow::on_btnStep_clicked()
{
    updateTextEditMemory();
    updateListViewRegisters();
}

void MainWindow::on_btnTerminate_clicked()
{
    this->running = false;
    this->debug = false;
    updateToolBarButtons();
    updateMemoryWidgetEnabled();
    updateRegistersWidgetEnabled();
}

void MainWindow::on_rbMemoryDec_clicked()
{
    updateTextEditMemory();
}

void MainWindow::on_rbMemoryHex_clicked()
{
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
