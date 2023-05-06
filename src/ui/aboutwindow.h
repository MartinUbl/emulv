#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QDialog>

class AboutWindow : public QDialog {
    Q_OBJECT
public:
    explicit AboutWindow(QWidget *parent = nullptr);

private:
    void SetupUI();
};

#endif // ABOUTWINDOW_H
