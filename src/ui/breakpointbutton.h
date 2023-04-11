#ifndef BREAKPOINTBUTTON_H
#define BREAKPOINTBUTTON_H

#include <QToolButton>

class BreakpointButton : public QToolButton
{
    Q_OBJECT
public:
    BreakpointButton(QWidget *parent = nullptr, int line = -1);
    void setFixedSize(int w, int h);

private slots:
    void onClicked();

private:
    int line_;
};

#endif // BREAKPOINTBUTTON_H
