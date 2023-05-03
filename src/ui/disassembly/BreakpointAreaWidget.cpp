//
// Created by Hynek on 11.04.2023.
//

#include <QLabel>
#include "BreakpointAreaWidget.h"
#include "../../utils/events/BreakpointAreaWidgetEvents.h"

BreakpointAreaWidget::BreakpointAreaWidget(QWidget *parent, Controller *controller)
    : QFrame(parent)
    , controller_(controller) {

}

void BreakpointAreaWidget::clear() {
    for (auto breakpoint : breakpoints_) {
        delete breakpoint.second;
    }

    breakpoints_.clear();
}

void BreakpointAreaWidget::mousePressEvent(QMouseEvent *event) {
    int clicked_y = event->pos().y();

    int breakpoint_height = this->width();
    int line = clicked_y / breakpoint_height;
    int y = line * breakpoint_height;

    if (line > max_breakpoints_ - 1) {
        return;
    }

    // If breakpoint exists, remove it
    if (breakpoints_.find(line) != breakpoints_.end()) {
        removeBreakpoint(line);
        return;
    }

    // Add a new breakpoint
    QLabel *breakpoint = new QLabel(this);

    breakpoint->setFixedSize(breakpoint_height, breakpoint_height);
    breakpoint->move(0, y - 1);
    breakpoint->setStyleSheet("padding: 2px;");
    breakpoint->setPixmap(QPixmap(":img/breakpoint.png"));
    breakpoint->setScaledContents(true);
    breakpoint->show();

    breakpoints_[line] = breakpoint;

    controller_->GetEventEmitter().Emit(Breakpoint_Added_Event_Description, new BreakpointAreaWidgetEvent(line));
}

void BreakpointAreaWidget::removeBreakpoint(int line) {
    QWidget *breakpoint = breakpoints_[line];
    breakpoints_.erase(line);
    delete breakpoint;

    controller_->GetEventEmitter().Emit(Breakpoint_Removed_Event_Description, new BreakpointAreaWidgetEvent(line));
}

void BreakpointAreaWidget::setMaximumBreakpoints(int max) {
    if (max < 1) {
        throw std::invalid_argument("Value of maximum breakpoints must be a positive number.");
    }

    max_breakpoints_ = max;
}
