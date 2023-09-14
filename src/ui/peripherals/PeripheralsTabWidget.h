//
// Created by Hynek on 06.04.2023.
//

#pragma once

#include <QTabWidget>

#include "PeripheralWidget.h"
#include "../EmulvApi.h"
#include "gpio/GPIOWidget.h"

class PeripheralsTabWidget : public QWidget {
    Q_OBJECT
public:
    QTabWidget *tab_widget_;
    GPIOWidget *gpio_widget_ = nullptr;
    std::unordered_map<std::string, PeripheralWidget *> widgets_;

    explicit PeripheralsTabWidget(QWidget *parent = nullptr, EmulvApi *controller = nullptr);

    void SetReadonly(bool readonly);
    void UpdateWidgets();
    void Clear();

private:
    EmulvApi *controller_;

    void AddWidget(peripherals::PeripheralDevice *peripheral_device, const std::string &label);
    void AddGPIOPortWidget(peripherals::GPIO_Port *gpio_port, const std::string &label);
    void AddUARTWidget(const std::string &label);
};
