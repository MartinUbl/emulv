//
// Created by Hynek on 29.04.2023.
//

#pragma once

#include <QWidget>

#include "../Controller.h"

class PeripheralWidget : public QWidget {
public:
    explicit PeripheralWidget(QWidget *parent = nullptr, Controller *controller = nullptr, std::string label = "") :
        QWidget(parent),
        controller_(controller),
        label_(label)
        {}

    virtual void SetReadonly(bool readonly) = 0;

    std::string GetLabel() { return label_; }

protected:
    Controller *controller_;

    std::string label_;
};
