//
// Created by Hynek on 06.04.2023.
//

#ifndef EMULV_PERIPHERALSTABWIDGET_H
#define EMULV_PERIPHERALSTABWIDGET_H

#include <QTabWidget>

#include "PeripheralWidget.h"
#include "../Controller.h"
#include "gpio/GPIOWidget.h"

class PeripheralsTabWidget : public QWidget {
    Q_OBJECT
public:
    QTabWidget *tab_widget_;
    GPIOWidget *gpio_widget_ = nullptr;
    std::unordered_map<std::string, PeripheralWidget *> widgets_;

    explicit PeripheralsTabWidget(QWidget *parent = nullptr, Controller *controller = nullptr);

    void SetReadonly(bool readonly);
    void UpdateWidgets();
    void Clear();

private:
    Controller *controller_;

    void AddWidget_(modules::PeripheralDevice *peripheral_device, const std::string &label);
    void AddGPIOPortWidget_(modules::GPIO_Port *gpio_port, const std::string &label);
    void AddUARTWidget_(const std::string &label);
};

#endif //EMULV_PERIPHERALSTABWIDGET_H
