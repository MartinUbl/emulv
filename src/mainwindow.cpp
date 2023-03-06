#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QMessageBox>
#include <QStringListModel>

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
    connect(ui->toolButton, SIGNAL(clicked(bool)), this, SLOT(showMessageBox()));
    connect(ui->action_About_RISCVEmulator, SIGNAL(triggered(bool)), this, SLOT(showAbout()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showMessageBox() {
    QMessageBox msgBox;
    msgBox.setText("Select address range.");
    msgBox.exec();
}

void MainWindow::showAbout() {

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
