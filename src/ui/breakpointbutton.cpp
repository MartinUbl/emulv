#include "breakpointbutton.h"

BreakpointButton::BreakpointButton(QWidget *parent, QString address)
{
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
        setStyleSheet("BreakpointButton { border: 1px solid red; background-color: rgba(255, 0, 0, 100); border-radius: 7px;}");
    }
    else
    {
        setStyleSheet("BreakpointButton { border: 1px solid transparent; background-color: rgba(255, 0, 0, 0); border-radius: 7px;}"
                      "BreakpointButton:hover { border: 1px solid transparent; background-color: rgba(255, 0, 0, 40); border-radius: 7px;}");
    }
}
