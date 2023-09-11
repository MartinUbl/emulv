//
// Created by Hynek on 06.04.2023.
//
#pragma once

#include <QWidget>

#include "../PeripheralWidget.h"
#include "GPIOPortWidget.h"

class GPIOWidget : public PeripheralWidget {
    Q_OBJECT
public:
    explicit GPIOWidget(QWidget *parent = nullptr);

    void AddPort(GPIOPortWidget *port_widget);
    void SetReadonly(bool readonly) override;

private:
    std::unordered_map<std::string, GPIOPortWidget *> ports_;
};
