#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setUARTTabVisible();
    void setGPIOTabVisible();
    void setOutputTabVisible();

    void on_action_Open_triggered();
    void on_action_About_RISCVEmulator_triggered();

    void on_spinBoxMemoryFrom_valueChanged(int arg1);

    void on_spinBoxMemoryTo_valueChanged(int arg1);

    void on_btnRestoreMemory_clicked();

    void on_btnSelectMemory_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    int memoryFrom = 0, memoryTo = 0xfff;

    void updatePeripheralTabWidgetVisible();
    void updateMemorySpinBoxes();
    void updateMemoryButtons();
    void updateTextEditMemory();
};
#endif // MAINWINDOW_H
