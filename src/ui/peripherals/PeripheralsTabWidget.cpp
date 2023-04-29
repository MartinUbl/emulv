//
// Created by Hynek on 06.04.2023.
//

#include <QScrollArea>
#include <QVBoxLayout>

#include "PeripheralsTabWidget.h"

PeripheralsTabWidget::PeripheralsTabWidget(QWidget *parent, Controller *controller)
: QWidget(parent)
, controller_(controller)
, tabWidget_(new QTabWidget(this)) {
    setLayout(new QVBoxLayout(this));
    layout()->setSpacing(0);
    layout()->setContentsMargins(0, 0, 0, 0);
    layout()->addWidget(tabWidget_);

    controller_->GetEventEmitter().On(GPIO_Pin_Mode_Changed_Event_Description, [this](AbstractEvent *res) {
        auto event = dynamic_cast<GPIO_Pin_Mode_Changed_Event *>(res);
        auto widget = widgets_[event->getPeripheralDevice().GetName()];
        auto gpioPortWidget = dynamic_cast<GPIOPortWidget *>(widget);

        gpioPortWidget->setPinMode(event->Pin_No, event->Current_Mode);

        delete res;
    });

    controller_->GetEventEmitter().On(GPIO_Pin_Level_Changed_Event_Description, [this](AbstractEvent *res) {
        auto event = dynamic_cast<GPIO_Pin_Level_Changed_Event *>(res);
        auto widget = widgets_[event->getPeripheralDevice().GetName()];
        auto gpioPortWidget = dynamic_cast<GPIOPortWidget *>(widget);

        gpioPortWidget->setPinStatus(event->Pin_No, event->Current_Level);

        delete res;
    });
}

void PeripheralsTabWidget::setReadonly(bool readonly) {
    for (auto widget : widgets_) {
        widget.second->setReadonly(readonly);
    }
}

void PeripheralsTabWidget::updateWidgets() {
    clear();
    auto peripherals = controller_->GetPeripherals();

    for (const auto& peripheral : peripherals) {
        addWidget_(peripheral.second, peripheral.first);
    }
}

void PeripheralsTabWidget::clear() {
    widgets_.clear();
    delete tabWidget_;
    delete gpioWidget_;
    gpioWidget_ = nullptr;

    tabWidget_ = new QTabWidget(this);
    layout()->addWidget(tabWidget_);
}

void PeripheralsTabWidget::addWidget_(modules::PeripheralDevice *peripheralDevice, const std::string &label) {
    auto gpioPort = dynamic_cast<modules::GPIO_Port *>(peripheralDevice);
    if (gpioPort != nullptr) {
        addGPIOPortWidget_(gpioPort, label);
    }
}

void PeripheralsTabWidget::addGPIOPortWidget_(modules::GPIO_Port *gpioPort, const std::string &label) {
    if (gpioWidget_ == nullptr) {
        gpioWidget_ = new GPIOWidget(tabWidget_);
        tabWidget_->addTab(gpioWidget_, "GPIO");
    }

    auto pins = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    auto portWidget = new GPIOPortWidget(gpioWidget_, controller_, label, pins);
    widgets_[label] = portWidget;
    gpioWidget_->addPort(portWidget);

    // Initialize default port modes and levels
    for (auto pin : pins) {
        portWidget->setPinMode(pin, gpioPort->Get_Pin_Mode(pin));
        portWidget->setPinStatus(pin, gpioPort->Get_Pin_Level(pin));
    }
}
