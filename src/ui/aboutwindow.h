#pragma once

#include <QDialog>

class AboutWindow : public QDialog {
    Q_OBJECT
public:
    explicit AboutWindow(QWidget *parent = nullptr);

private:
    void SetupUI();
};
