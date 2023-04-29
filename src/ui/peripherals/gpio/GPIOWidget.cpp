//
// Created by Hynek on 06.04.2023.
//

#include <QVBoxLayout>
#include "GPIOWidget.h"

GPIOWidget::GPIOWidget(QWidget *parent) : QWidget(parent), ports_() {
    QVBoxLayout *gpio_layout = new QVBoxLayout(this);
    gpio_layout->setAlignment(Qt::AlignTop);
}

void GPIOWidget::addPort(GPIOPortWidget *port_widget) {
    ports_[port_widget->label()] = port_widget;

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignLeft);

    QWidget *widget = new QWidget(this);
    widget->setLayout(layout);

    layout->addWidget(port_widget);

    this->layout()->addWidget(widget);
}

void GPIOWidget::setPinMode(std::string port, int pin_id, modules::GPIO_Pin_Mode mode) {
    ports_[port]->setPinMode(pin_id, mode);
}

void GPIOWidget::setPinStatus(std::string port, int pin_id, bool status) {
    ports_[port]->setPinStatus(pin_id, status);
}