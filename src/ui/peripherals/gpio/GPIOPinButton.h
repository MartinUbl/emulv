//
// Created by Hynek on 04.04.2023.
//

#ifndef EMULV_GPIOPIN_H
#define EMULV_GPIOPIN_H

#include <QObject>
#include <QToolButton>
#include "../../../utils/events/gpio_events.h"

class GPIOPinButton : public QToolButton {
    Q_OBJECT
public:
    GPIOPinButton(QWidget *parent = nullptr, int id = 0);
    void setMode(modules::GPIO_Pin_Mode mode);
    void setStatus(bool status);
    int id() { return id_; }
    modules::GPIO_Pin_Mode mode();
    bool status();

private:
    int id_;
    modules::GPIO_Pin_Mode mode_ = modules::GPIO_Pin_Mode::INPUT;
    bool status_ = false;
    void update_button();
};

#endif //EMULV_GPIOPIN_H
