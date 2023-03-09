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

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 0);

    ui->splitter_4->setStretchFactor(0, 2);
    ui->splitter_4->setStretchFactor(1, 1);

    ui->splitter_2->setStretchFactor(0, 3);
    ui->splitter_2->setStretchFactor(1, 1);

    auto model = new QStringListModel(this);

    QStringList list;
    list << "R0:   40 00 bf 1a";
    list << "R1:   40 00 bf 1a";
    list << "R2:   40 00 bf 1a";
    list << "R3:   40 00 bf 1a";
    model->setStringList(list);

    ui->listView->setModel(model);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->action_Serial_monitor, SIGNAL(toggled(bool)), this, SLOT(setUARTTabVisible()));
    connect(ui->action_GPIO, SIGNAL(toggled(bool)), this, SLOT(setGPIOTabVisible()));
    connect(ui->action_Output, SIGNAL(toggled(bool)), this, SLOT(setOutputTabVisible()));
    connect(ui->action_About_RISCVEmulator, SIGNAL(triggered(bool)), this, SLOT(showAbout()));

    MainWindow::on_btnRestoreMemory_clicked();
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

void MainWindow::updatePeripheralTabWidgetVisible() {
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        if (ui->tabWidget->isTabVisible(i)) {
            ui->tabWidget->setVisible(true);
            return;
        }
    }

    ui->tabWidget->setVisible(false);
}

void MainWindow::updateMemoryButtons() {
    bool enabled = ui->spinBoxMemoryFrom->value() != this->memoryFrom ||
                   ui->spinBoxMemoryTo->value() != this->memoryTo;

    ui->btnSelectMemory->setEnabled(enabled);
    ui->btnRestoreMemory->setEnabled(enabled);
}

void MainWindow::updateTextEditMemory() {
    std::string s = "       00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n";

    for (int i = this->memoryFrom; i <= this->memoryTo; ++i) {
        std::stringstream ss;
        ss << std::uppercase << std::hex << std::setw(3) << std::setfill('0') << i;
        s += ss.str() + "0  ";
        for (int j = 0; j < 16; ++j) {
            std::stringstream ssByte;
            ssByte << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (rand() % 16);
            s += " " + ssByte.str();
        }
        s += "\n";
    }

    QString str = QString::fromStdString(s);
    ui->textEditMemory->setPlainText(str);
}

void MainWindow::updateMemorySpinBoxes() {
    ui->spinBoxMemoryFrom->setValue(this->memoryFrom);
    ui->spinBoxMemoryTo->setValue(this->memoryTo);
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
    MainWindow::updateMemoryButtons();
}

void MainWindow::on_spinBoxMemoryTo_valueChanged(int arg1)
{
    ui->spinBoxMemoryFrom->setMaximum(ui->spinBoxMemoryTo->value());
    MainWindow::updateMemoryButtons();
}

void MainWindow::on_btnRestoreMemory_clicked()
{
    MainWindow::updateMemorySpinBoxes();
    MainWindow::updateTextEditMemory();
}

void MainWindow::on_btnSelectMemory_clicked()
{
    this->memoryFrom = ui->spinBoxMemoryFrom->value();
    this->memoryTo = ui->spinBoxMemoryTo->value();

    MainWindow::updateMemorySpinBoxes();
    MainWindow::updateMemoryButtons();
    MainWindow::updateTextEditMemory();
}

