//
// Created by Hynek on 04.04.2023.
//

#include "GPIOPinButton.h"

GPIOPinButton::GPIOPinButton(QWidget *parent, int id)
    : QToolButton(parent)
    , id_(id) {
    this->setFixedSize(25, 25);
    this->update_button();
    this->setIconSize(QSize(height() - 2, height() - 2));

    connect(this, SIGNAL(clicked(bool)), this, SLOT(on_clicked()));
}

void GPIOPinButton::setMode(GPIO_PinMode mode) {
    mode_ = mode;
    update_button();
}

void GPIOPinButton::setStatus(bool status) {
    status_ = status;
    update_button();
}

GPIO_PinMode GPIOPinButton::mode() {
    return mode_;
}

bool GPIOPinButton::status() {
    return status_;
}

void GPIOPinButton::update_button() {
    if (mode_ == GPIO_PinMode::kInput) {
        this->setCursor(Qt::PointingHandCursor);

        if (status_ == false) {
            this->setIcon(QIcon(":img/gpio_input_low.svg"));
        }
        else  {
            this->setIcon(QIcon(":img/gpio_input_high.svg"));
        }
    }
    else if (mode_ == GPIO_PinMode::kOutput) {
        this->unsetCursor();

        if (status_ == false) {
            this->setIcon(QIcon(":img/gpio_output_low.svg"));
        }
        else  {
            this->setIcon(QIcon(":img/gpio_output_high.svg"));
        }
    }
}
