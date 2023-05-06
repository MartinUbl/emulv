//
// Created by Hynek on 04.04.2023.
//
#include "GPIOPortWidget.h"

#include <QLabel>
#include <QGridLayout>

GPIOPortWidget::GPIOPortWidget(QWidget *parent, Controller *controller, const std::string& label, std::vector<int> pin_ids)
: PeripheralWidget(parent, controller, label)
, pins_() {
    setStyleSheet("GPIOPinButton, GPIOPinButton:pressed { border: 0px; background-color: transparent; }");

    auto layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(new QLabel(QString::fromStdString(label), this), 1, 0, 2, 1, Qt::AlignRight);

    for (int i = 0; i < pin_ids.size(); ++i) {
        int id = pin_ids[i];
        auto pin = new GPIOPinButton(this, id);
        pins_[id] = pin;

        connect(pin, SIGNAL(clicked(bool)), this, SLOT(OnPinButtonClicked()));

        layout->addWidget(new QLabel(QString::number(id), this), 0, i + 1, Qt::AlignCenter);
        layout->addWidget(pin, 1, i + 1, Qt::AlignCenter);
    }
}

void GPIOPortWidget::SetPinMode(int pin_id, modules::GPIO_Pin_Mode mode) {
    pins_[pin_id]->SetMode(mode);
}

void GPIOPortWidget::SetPinStatus(int pin_id, bool status) {
    pins_[pin_id]->SetStatus(status);
}

void GPIOPortWidget::SetReadonly(bool readonly) {
    for (auto pin : pins_) {
        pin.second->setEnabled(!readonly);
    }
}

void GPIOPortWidget::OnPinButtonClicked() {
    auto clicked_pin = dynamic_cast<GPIOPinButton *>(QObject::sender());

    if (clicked_pin->GetMode() == modules::GPIO_Pin_Mode::INPUT) {
        clicked_pin->SetStatus(!clicked_pin->GetStatus());
        controller_->SetPinStatus(label_, clicked_pin->PinID(), clicked_pin->GetStatus());
    }
}
