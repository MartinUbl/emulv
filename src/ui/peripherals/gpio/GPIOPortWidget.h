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
    explicit GPIOPortWidget(QWidget *parent = nullptr, Controller *controller = nullptr,
                            std::string label = "", std::vector<int> pin_ids = {});

    void SetPinMode(int pin_id, modules::GPIO_Pin_Mode mode);
    void SetPinStatus(int pin_id, bool status);

    modules::GPIO_Pin_Mode GetPinMode(int pin_id) { return pins_[pin_id]->GetMode(); }
    bool GetPinStatus(int pin_id) { return pins_[pin_id]->GetStatus(); }

    void SetReadonly(bool readonly) override;

private slots:
    void OnPinButtonClicked();

private:
    std::unordered_map<int, GPIOPinButton *> pins_;

};

#endif //EMULV_GPIOPORTWIDGET_H
