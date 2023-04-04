//
// Created by Hynek on 04.04.2023.
//

#ifndef EMULV_GPIOPORTWIDGET_H
#define EMULV_GPIOPORTWIDGET_H

#include <QFrame>
#include "GPIOPinButton.h"

class GPIOPortWidget : public QFrame {
    Q_OBJECT
public:
    GPIOPortWidget(QWidget *parent = nullptr, std::string label = "", std::vector<int> pin_ids = {});
    void setPinMode(int pin_id, GPIO_PinMode input);
    void setPinStatus(int pin_id, bool status);

private:
    std::string label_;
    int pin_count_;
    std::unordered_map<int, GPIOPinButton *> pins_;
};

#endif //EMULV_GPIOPORTWIDGET_H
