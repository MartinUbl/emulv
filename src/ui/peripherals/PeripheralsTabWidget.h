//
// Created by Hynek on 06.04.2023.
//

#ifndef EMULV_PERIPHERALSTABWIDGET_H
#define EMULV_PERIPHERALSTABWIDGET_H

#include <QWidget>
#include <QTabWidget>

#include "PeripheralWidget.h"
#include "../Controller.h"
#include "gpio/GPIOWidget.h"
#include "../../modules/uart.h"

class PeripheralsTabWidget : public QWidget {
    Q_OBJECT
public:
    QTabWidget *tabWidget_;
    GPIOWidget *gpioWidget_ = nullptr;
    std::unordered_map<std::string, PeripheralWidget *> widgets_;

    PeripheralsTabWidget(QWidget *parent = nullptr, Controller *controller = nullptr);
    void setReadonly(bool readonly);
    void updateWidgets();
    void clear();

private:
    Controller *controller_;

    void addWidget_(modules::PeripheralDevice *peripheralDevice, const std::string &label);
    void addGPIOPortWidget_(modules::GPIO_Port *gpioPort, const std::string &label);
    void addUARTWidget_(const std::string &label);
};

#endif //EMULV_PERIPHERALSTABWIDGET_H
