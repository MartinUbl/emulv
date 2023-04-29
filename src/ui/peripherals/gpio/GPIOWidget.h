//
// Created by Hynek on 06.04.2023.
//

#ifndef EMULV_GPIOWIDGET_H
#define EMULV_GPIOWIDGET_H

#include <QWidget>
#include "GPIOPortWidget.h"
#include "GPIOPinButton.h"
#include "../PeripheralWidget.h"

class GPIOWidget : public PeripheralWidget {
    Q_OBJECT
public:
    GPIOWidget(QWidget *parent = nullptr);
    void addPort(GPIOPortWidget *port_widget);
    void setReadonly(bool readonly) override;

private:
    std::unordered_map<std::string, GPIOPortWidget *> ports_;
};

#endif //EMULV_GPIOWIDGET_H
