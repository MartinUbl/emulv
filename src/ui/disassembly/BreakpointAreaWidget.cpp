//
// Created by Hynek on 11.04.2023.
//
#include "BreakpointAreaWidget.h"
#include "Events.h"
#include <QLabel>

BreakpointAreaWidget::BreakpointAreaWidget(QWidget *parent, EmulvApi *controller)
        : QFrame(parent), controller_(controller) {}

void BreakpointAreaWidget::Clear() {
    for (auto breakpoint: breakpoints_) {
        delete breakpoint.second;
    }

    breakpoints_.clear();
}

void BreakpointAreaWidget::RemoveBreakpoint(int line) {
    auto breakpoint = breakpoints_[line];
    breakpoints_.erase(line);
    delete breakpoint;

    EventsLib::globalEmit("breakpoint-removed", EventsLib::EventData{{"line", line}});
}

void BreakpointAreaWidget::SetMaximumBreakpoints(int max) {
    if (max < 1) {
        throw std::invalid_argument("Value of maximum breakpoints must be a positive number.");
    }

    max_breakpoints_ = max;
}

void BreakpointAreaWidget::mousePressEvent(QMouseEvent *event) {
    int clicked_y = event->pos().y();

    // The width of this widget needs to be equal to the line height of the disassembly widget's TextEdits
    // so that the breakpoint widget can be square
    int breakpoint_height = this->width();
    int line = clicked_y / breakpoint_height;
    int y = line * breakpoint_height;

    // Return, if user clicked below the last possible breakpoint position
    if (line > max_breakpoints_ - 1) {
        return;
    }

    // If breakpoint exists, remove it
    if (breakpoints_.find(line) != breakpoints_.end()) {
        RemoveBreakpoint(line);
        return;
    }

    // Otherwise, add a new breakpoint
    auto breakpoint = new QLabel(this);

    // Move up by one so the breakpoint is better centered on the line
    breakpoint->move(0, y - 1);
    breakpoint->setFixedSize(breakpoint_height, breakpoint_height);
    breakpoint->setStyleSheet("padding: 2px;");
    breakpoint->setPixmap(QPixmap(":img/breakpoint.png"));
    breakpoint->setScaledContents(true);
    breakpoint->show();

    breakpoints_[line] = breakpoint;

    EventsLib::globalEmit("breakpoint-added", EventsLib::EventData{{"line", line}});
}
