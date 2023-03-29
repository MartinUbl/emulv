#include "breakpointbutton.h"
#include <QString>

BreakpointButton::BreakpointButton(QWidget *parent, QString address)
{
    this->address = address;

    setStyleSheet("BreakpointButton { border: 0px; background-color: transparent; }");

    updateVisibility();
    connect(this, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
}

void BreakpointButton::onClicked()
{
    active = !active;
    updateVisibility();
}

void BreakpointButton::updateVisibility()
{
    if (active)
    {
        setIcon(QIcon(":img/breakpoint.png"));
        setIconSize(QSize(height() - 2, height() - 2));
    }
    else
    {
        setIcon(QIcon());
    }
}
