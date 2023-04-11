#include "breakpointbutton.h"
#include "BreakpointAreaWidget.h"

BreakpointButton::BreakpointButton(QWidget *parent, int line) : QToolButton(parent) {
    line_ = line;

    setStyleSheet("BreakpointButton { border: 0px; background-color: transparent; }");

    setIcon(QIcon(":img/breakpoint.png"));
    setIconSize(QSize(height() - 2, height() - 2));

    connect(this, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
}

void BreakpointButton::onClicked() {
    BreakpointAreaWidget *p = qobject_cast<BreakpointAreaWidget*>(parent());
    p->removeBreakpoint(line_);
}

void BreakpointButton::setFixedSize(int w, int h) {
    QWidget::setFixedSize(w, h);;
    setIconSize(QSize(height() - 2, height() - 2));
}
