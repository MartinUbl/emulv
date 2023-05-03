//
// Created by Hynek on 11.04.2023.
//

#ifndef EMULV_BREAKPOINTAREAWIDGET_H
#define EMULV_BREAKPOINTAREAWIDGET_H

#include <QObject>
#include <QFrame>
#include <QMouseEvent>
#include "../Controller.h"

class BreakpointAreaWidget : public QFrame {
    Q_OBJECT
public:
    BreakpointAreaWidget(QWidget *parent = nullptr, Controller *controller = nullptr);
    void setMaximumBreakpoints(int max);
    void removeBreakpoint(int line);
    void clear();

private:
    Controller *controller_;

    int max_breakpoints_ = 1;
    std::unordered_map<int, QWidget *> breakpoints_;

    void mousePressEvent(QMouseEvent *event) override;
};

#endif //EMULV_BREAKPOINTAREAWIDGET_H
