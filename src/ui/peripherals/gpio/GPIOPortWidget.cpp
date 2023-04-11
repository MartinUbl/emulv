//
// Created by Hynek on 04.04.2023.
//

#include <QLabel>
#include <QGridLayout>
#include "GPIOPortWidget.h"

GPIOPortWidget::GPIOPortWidget(QWidget *parent, std::string label, std::vector<int> pin_ids)
    : QFrame(parent), pins_() {

    this->setStyleSheet("GPIOPinButton, GPIOPinButton:pressed { border: 0px; background-color: transparent; }");

    this->label_ = label;

    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(new QLabel(QString::fromStdString(label), this), 1, 0, 2, 1, Qt::AlignRight);

    for (int i = 0; i < pin_ids.size(); ++i) {
        int id = pin_ids[i];
        GPIOPinButton *pin = new GPIOPinButton(this);
        pins_[id] = pin;

        layout->addWidget(new QLabel(QString::number(id), this), 0, i + 1, Qt::AlignCenter);
        layout->addWidget(pin, 1, i + 1, Qt::AlignCenter);
    }

    this->setLayout(layout);
}

void GPIOPortWidget::setPinMode(int pin_id, GPIO_PinMode input) {
    this->pins_[pin_id]->setMode(input);
}

void GPIOPortWidget::setPinStatus(int pin_id, bool status) {
    this->pins_[pin_id]->setStatus(status);
}

std::string GPIOPortWidget::label() {
    return label_;
}