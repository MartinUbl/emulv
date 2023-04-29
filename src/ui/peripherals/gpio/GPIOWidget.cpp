//
// Created by Hynek on 06.04.2023.
//

#include <QVBoxLayout>
#include "GPIOWidget.h"

GPIOWidget::GPIOWidget(QWidget *parent)
: PeripheralWidget(parent)
, ports_() {
    auto gpio_layout = new QVBoxLayout(this);
    gpio_layout->setAlignment(Qt::AlignTop);
}

void GPIOWidget::addPort(GPIOPortWidget *port_widget) {
    ports_[port_widget->label()] = port_widget;

    auto layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignLeft);

    auto widget = new QWidget(this);
    widget->setLayout(layout);

    layout->addWidget(port_widget);

    this->layout()->addWidget(widget);
}

void GPIOWidget::setReadonly(bool readonly) {
    for (auto port : ports_) {
        port.second->setReadonly(readonly);
    }
}
