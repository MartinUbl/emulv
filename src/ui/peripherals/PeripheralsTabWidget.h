//
// Created by Hynek on 06.04.2023.
//

#ifndef EMULV_PERIPHERALSTABWIDGET_H
#define EMULV_PERIPHERALSTABWIDGET_H

#include <QWidget>
#include <QTabWidget>

#include "../Controller.h"
#include "gpio/GPIOWidget.h"

class PeripheralsTabWidget : public QWidget {
    Q_OBJECT
public:
    PeripheralsTabWidget(QWidget *parent = nullptr, Controller *controller = nullptr);
    void updateWidgets();
    void clear();

private:
    Controller *controller_;
    QTabWidget *tabWidget_;
    GPIOWidget *gpioWidget_ = nullptr;

    std::unordered_map<std::string, QWidget *> widgets_;

    void addWidget_(modules::PeripheralDevice *peripheralDevice, const std::string &label);
    void addGPIOPortWidget_(modules::GPIO_Port *gpioPort, const std::string &label);
};

#endif //EMULV_PERIPHERALSTABWIDGET_H
