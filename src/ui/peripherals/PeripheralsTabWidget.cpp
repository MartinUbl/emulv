//
// Created by Hynek on 06.04.2023.
//
#include "PeripheralsTabWidget.h"

#include <QVBoxLayout>

#include "uart/UARTWidget.h"
#include "../../utils/events/uart_event.h"

PeripheralsTabWidget::PeripheralsTabWidget(QWidget *parent, Controller *controller)
: QWidget(parent)
, controller_(controller)
, tab_widget_(new QTabWidget(this)) {
    setLayout(new QVBoxLayout(this));
    layout()->setSpacing(0);
    layout()->setContentsMargins(0, 0, 0, 0);
    layout()->addWidget(tab_widget_);

    controller_->GetEventEmitter().On(GPIO_Pin_Mode_Changed_Event_Description, [this](AbstractEvent *res) {
        auto event = dynamic_cast<GPIO_Pin_Mode_Changed_Event *>(res);
        auto widget = widgets_[event->getPeripheralDevice().GetName()];
        auto gpioPortWidget = dynamic_cast<GPIOPortWidget *>(widget);

        gpioPortWidget->SetPinMode(event->Pin_No, event->Current_Mode);

        delete res;
    });

    controller_->GetEventEmitter().On(GPIO_Pin_Level_Changed_Event_Description, [this](AbstractEvent *res) {
        auto event = dynamic_cast<GPIO_Pin_Level_Changed_Event *>(res);
        auto widget = widgets_[event->getPeripheralDevice().GetName()];
        auto gpioPortWidget = dynamic_cast<GPIOPortWidget *>(widget);

        gpioPortWidget->SetPinStatus(event->Pin_No, event->Current_Level);

        delete res;
    });

    controller_->GetEventEmitter().On(UART_event_description, [this](AbstractEvent *res) {
        auto event = dynamic_cast<uart_event *>(res);
        auto widget = widgets_[event->getPeripheralDevice().GetName()];
        auto uartWidget = dynamic_cast<UARTWidget *>(widget);

        char c = static_cast<char>(event->getData());

        QMetaObject::invokeMethod(uartWidget, [uartWidget, c]() {
            uartWidget->AppendChar(c);
        });

        delete res;
    });
}

void PeripheralsTabWidget::SetReadonly(bool readonly) {
    for (auto widget : widgets_) {
        widget.second->SetReadonly(readonly);
    }
}

void PeripheralsTabWidget::UpdateWidgets() {
    Clear();

    auto peripherals = controller_->GetPeripherals();

    setVisible(!peripherals.empty());

    for (const auto& peripheral : peripherals) {
        auto label = peripheral.first;
        auto device = peripheral.second;
        AddWidget_(device, label);
    }
}

void PeripheralsTabWidget::Clear() {
    widgets_.clear();

    if (gpio_widget_ != nullptr) {
        delete gpio_widget_;
        gpio_widget_ = nullptr;
    }

    delete tab_widget_;
    tab_widget_ = new QTabWidget(this);
    layout()->addWidget(tab_widget_);
}

void PeripheralsTabWidget::AddWidget_(modules::PeripheralDevice *peripheral_device, const std::string &label) {
    // Cast device to GPIO_Port pointer, if successful add a new GPIOPortWidget
    auto gpio_port = dynamic_cast<modules::GPIO_Port *>(peripheral_device);
    if (gpio_port != nullptr) {
        AddGPIOPortWidget_(gpio_port, label);
        return;
    }

    // Cast device to UART_Device pointer, if successful add a new UARTWidget
    auto uart = dynamic_cast<modules::UART_Device *>(peripheral_device);
    if (uart != nullptr) {
        AddUARTWidget_(uart->GetName());
        return;
    }
}

void PeripheralsTabWidget::AddGPIOPortWidget_(modules::GPIO_Port *gpio_port, const std::string &label) {
    // If no GPIO port has been added yet, create the widget for all ports
    if (gpio_widget_ == nullptr) {
        gpio_widget_ = new GPIOWidget(tab_widget_);
        tab_widget_->addTab(gpio_widget_, "GPIO");
    }

    auto pins = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    auto port_widget = new GPIOPortWidget(gpio_widget_, controller_, label, pins);

    widgets_[label] = port_widget;
    gpio_widget_->AddPort(port_widget);

    // Initialize default port modes and levels
    for (auto pin : pins) {
        port_widget->SetPinMode(pin, gpio_port->Get_Pin_Mode(pin));
        port_widget->SetPinStatus(pin, gpio_port->Get_Pin_Level(pin));
    }
}

void PeripheralsTabWidget::AddUARTWidget_(const std::string &label) {
    auto uart_widget = new UARTWidget(tab_widget_, controller_, label);

    widgets_[label] = uart_widget;
    tab_widget_->addTab(uart_widget, QString::fromStdString(label));
}
