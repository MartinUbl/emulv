//
// Created by Hynek on 06.04.2023.
//
#include "GPIOWidget.h"

#include <QVBoxLayout>

GPIOWidget::GPIOWidget(QWidget *parent)
: PeripheralWidget(parent) {
    setLayout(new QVBoxLayout(this));
    layout()->setAlignment(Qt::AlignTop);
}

void GPIOWidget::AddPort(GPIOPortWidget *port_widget) {
    ports_[port_widget->GetLabel()] = port_widget;

    // Adds the port to a widget with a horizontal layout and a left alignment so that is does not stretch
    auto widget = new QWidget(this);
    port_widget->setParent(widget);

    auto widget_layout = new QHBoxLayout(widget);
    widget_layout->setAlignment(Qt::AlignLeft);
    widget->setLayout(widget_layout);

    widget_layout->addWidget(port_widget);

    layout()->addWidget(widget);
}

void GPIOWidget::SetReadonly(bool readonly) {
    for (auto port : ports_) {
        port.second->SetReadonly(readonly);
    }
}
