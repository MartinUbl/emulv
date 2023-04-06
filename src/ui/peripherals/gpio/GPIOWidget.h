//
// Created by Hynek on 06.04.2023.
//

#ifndef EMULV_GPIOWIDGET_H
#define EMULV_GPIOWIDGET_H

#include <QWidget>
#include "GPIOPortWidget.h"
#include "GPIOPinButton.h"

class GPIOWidget : public QWidget {
public:
    GPIOWidget(QWidget *parent = nullptr);
    void addPort(GPIOPortWidget *port_widget);
    void setPinMode(std::string port, int pin_id, GPIO_PinMode mode);
    void setPinStatus(std::string port, int pin_id, bool status);

private:
    std::unordered_map<std::string, GPIOPortWidget *> ports_;
};

#endif //EMULV_GPIOWIDGET_H
