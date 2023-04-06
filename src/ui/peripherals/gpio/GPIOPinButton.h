//
// Created by Hynek on 04.04.2023.
//

#ifndef EMULV_GPIOPIN_H
#define EMULV_GPIOPIN_H

#include <QToolButton>

enum class GPIO_PinMode {
    kInput,
    kOutput
};

class GPIOPinButton : public QToolButton {
    Q_OBJECT
public:
    GPIOPinButton(QWidget *parent = nullptr);
    void setMode(GPIO_PinMode mode);
    void setStatus(bool status);
    GPIO_PinMode mode();
    bool status();

private slots:
    void on_clicked();

private:
    GPIO_PinMode mode_ = GPIO_PinMode::kInput;
    bool status_ = false;
    void update_button();
};

#endif //EMULV_GPIOPIN_H
