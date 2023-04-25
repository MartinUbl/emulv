//
// Created by Hynek on 19.04.2023.
//

#ifndef EMULV_REGISTERSWIDGET_H
#define EMULV_REGISTERSWIDGET_H

#include <QFrame>
#include <QTextEdit>
#include <QRadioButton>

class RegistersWidget : public QFrame {
    Q_OBJECT
public:
    explicit RegistersWidget(QWidget *parent = nullptr);
    void setRegisters(const std::vector<std::tuple<std::string, uint32_t>> &registers);

private slots:
    void rb_hex_clicked_();
    void rb_dec_clicked_();

private:
    std::vector<std::tuple<std::string, uint32_t>> registers_;
    QRadioButton *rb_hex_, *rb_dec_;
    QTextEdit *main_text_edit_;

    void updateRegisters_();
    std::string formatByte_(int byte);
    std::string formatValueBytes_(uint32_t value);
};

#endif //EMULV_REGISTERSWIDGET_H
