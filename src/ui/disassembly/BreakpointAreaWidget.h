//
// Created by Hynek on 11.04.2023.
//

#pragma once

#include <QFrame>
#include <QMouseEvent>

#include "../EmulvApi.h"

class BreakpointAreaWidget : public QFrame {
    Q_OBJECT
public:
    explicit BreakpointAreaWidget(QWidget *parent = nullptr, EmulvApi *controller = nullptr);

    void SetMaximumBreakpoints(int max);
    void RemoveBreakpoint(int line);
    void Clear();

private:
    EmulvApi *controller_;

    int max_breakpoints_ = 1;
    std::unordered_map<int, QWidget *> breakpoints_;

    void mousePressEvent(QMouseEvent *event) override;
};
