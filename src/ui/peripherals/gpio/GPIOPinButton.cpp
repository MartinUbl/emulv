//
// Created by Hynek on 04.04.2023.
//
#include "GPIOPinButton.h"

GPIOPinButton::GPIOPinButton(QWidget *parent, int id)
: QToolButton(parent)
, id_(id) {
    setFixedSize(25, 25);
    UpdateButton();
    setIconSize(QSize(height() - 2, height() - 2));
}

void GPIOPinButton::SetMode(peripherals::GPIO_Pin_Mode mode) {
    mode_ = mode;
    UpdateButton();
}

void GPIOPinButton::SetStatus(bool status) {
    status_ = status;
    UpdateButton();
}

void GPIOPinButton::UpdateButton() {
    if (mode_ == peripherals::GPIO_Pin_Mode::INPUT) {
        setCursor(Qt::PointingHandCursor);

        if (status_ == false) {
            setIcon(QIcon(":img/gpio_input_low.svg"));
        }
        else  {
            setIcon(QIcon(":img/gpio_input_high.svg"));
        }
    }
    else if (mode_ == peripherals::GPIO_Pin_Mode::OUTPUT) {
        this->unsetCursor();

        if (status_ == false) {
            setIcon(QIcon(":img/gpio_output_low.svg"));
        }
        else  {
            setIcon(QIcon(":img/gpio_output_high.svg"));
        }
    }
}
