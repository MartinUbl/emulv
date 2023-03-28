#ifndef BREAKPOINTBUTTON_H
#define BREAKPOINTBUTTON_H

#include <QToolButton>

class BreakpointButton : public QToolButton
{
    Q_OBJECT
public:
    BreakpointButton(QWidget *parent = nullptr, QString address = "");

private slots:
    void onClicked();

private:
    bool active = false;
    QString address;

    void updateVisibility();
};

#endif // BREAKPOINTBUTTON_H
