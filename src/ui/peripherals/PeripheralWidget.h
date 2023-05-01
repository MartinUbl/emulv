//
// Created by Hynek on 29.04.2023.
//

#ifndef EMULV_PERIPHERALWIDGET_H
#define EMULV_PERIPHERALWIDGET_H

#include <QWidget>
#include "../Controller.h"

class PeripheralWidget : public QWidget {
public:
    PeripheralWidget(QWidget *parent = nullptr, Controller *controller = nullptr, std::string label = "") :
        QWidget(parent), controller_(controller), label_(label) {}
    virtual void setReadonly(bool readonly) = 0;

protected:
    std::string label_;
    Controller *controller_;
};

#endif //EMULV_PERIPHERALWIDGET_H
