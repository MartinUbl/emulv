//
// Created by Hynek on 29.04.2023.
//

#ifndef EMULV_PERIPHERALWIDGET_H
#define EMULV_PERIPHERALWIDGET_H

#include <QWidget>

class PeripheralWidget : public QWidget {
public:
    PeripheralWidget(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual void setReadonly(bool readonly) = 0;
};

#endif //EMULV_PERIPHERALWIDGET_H
