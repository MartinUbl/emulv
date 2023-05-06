//
// Created by Hynek on 04.04.2023.
//

#ifndef EMULV_GPIOPINBUTTON_H
#define EMULV_GPIOPINBUTTON_H

#include <QToolButton>

#include "../../../utils/events/gpio_events.h"

class GPIOPinButton : public QToolButton {
    Q_OBJECT
public:
    explicit GPIOPinButton(QWidget *parent = nullptr, int id = 0);

    void SetMode(modules::GPIO_Pin_Mode mode);
    void SetStatus(bool status);

    int PinID() const { return id_; }
    bool GetStatus() const { return status_; }
    modules::GPIO_Pin_Mode GetMode() const { return mode_; }

private:
    int id_;
    modules::GPIO_Pin_Mode mode_ = modules::GPIO_Pin_Mode::INPUT;
    bool status_ = false;

    void UpdateButton();
};

#endif //EMULV_GPIOPINBUTTON_H
