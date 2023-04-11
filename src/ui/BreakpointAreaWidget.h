//
// Created by Hynek on 11.04.2023.
//

#ifndef EMULV_BREAKPOINTAREAWIDGET_H
#define EMULV_BREAKPOINTAREAWIDGET_H

#include <QFrame>
#include <QMouseEvent>
#include "breakpointbutton.h"

class BreakpointAreaWidget : public QFrame {
    Q_OBJECT
public:
    BreakpointAreaWidget(QWidget *parent = nullptr);
    void setMaximumBreakpoints(int max);
    void removeBreakpoint(int line);
    void clear();

private:
    int max_breakpoints_ = 1;
    std::unordered_map<int, BreakpointButton *> breakpoints_;

    void mousePressEvent(QMouseEvent *event) override;
};

#endif //EMULV_BREAKPOINTAREAWIDGET_H
