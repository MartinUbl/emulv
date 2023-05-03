//
// Created by Hynek on 04.04.2023.
//

#ifndef EMULV_GPIOPORTWIDGET_H
#define EMULV_GPIOPORTWIDGET_H

#include <QFrame>
#include "GPIOPinButton.h"
#include "../../Controller.h"
#include "../PeripheralWidget.h"

class GPIOPortWidget : public PeripheralWidget {
    Q_OBJECT
public:
    GPIOPortWidget(QWidget *parent = nullptr, Controller *controller = nullptr, std::string label = "", std::vector<int> pin_ids = {});
    void setPinMode(int pin_id, modules::GPIO_Pin_Mode mode);
    void setPinStatus(int pin_id, bool status);
    modules::GPIO_Pin_Mode pinMode(int pin_id);
    bool pinStatus(int pin_id);
    void setReadonly(bool readonly) override;

private:
    std::unordered_map<int, GPIOPinButton *> pins_;

private slots:
    void onPinButtonClicked();
};

#endif //EMULV_GPIOPORTWIDGET_H
