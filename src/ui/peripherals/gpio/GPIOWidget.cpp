//
// Created by Hynek on 06.04.2023.
//

#include <QVBoxLayout>
#include "GPIOWidget.h"

GPIOWidget::GPIOWidget(QWidget *parent)
: PeripheralWidget(parent) {
    setLayout(new QVBoxLayout(this));
    layout()->setAlignment(Qt::AlignTop);
}

void GPIOWidget::addPort(GPIOPortWidget *port_widget) {
    ports_[port_widget->label()] = port_widget;

    auto widget = new QWidget(this);
    port_widget->setParent(widget);

    auto widget_layout = new QHBoxLayout(widget);
    widget_layout->setAlignment(Qt::AlignLeft);
    widget->setLayout(widget_layout);

    widget_layout->addWidget(port_widget);

    layout()->addWidget(widget);
}

void GPIOWidget::setReadonly(bool readonly) {
    for (auto port : ports_) {
        port.second->setReadonly(readonly);
    }
}
