//
// Created by Hynek on 19.04.2023.
//

#ifndef EMULV_REGISTERSWIDGET_H
#define EMULV_REGISTERSWIDGET_H

#include <QFrame>
#include <QTextEdit>
#include <QRadioButton>
#include <array>
#include <string>

constexpr size_t kRegisters_Count = 32;
const std::array<std::string, kRegisters_Count> kRegisters_ABI_Names {
    "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "fp", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
    "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
    "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

class RegistersWidget : public QFrame {
    Q_OBJECT
public:
    QRadioButton *rb_hex_, *rb_dec_;
    QTextEdit *main_text_edit_;

    explicit RegistersWidget(QWidget *parent = nullptr);
    void setRegisters(const std::vector<std::tuple<std::string, uint32_t>> &registers);

private slots:
    void rb_hex_clicked_();
    void rb_dec_clicked_();

private:
    static constexpr size_t kRegisterLabelWidth = 4;
    std::vector<std::tuple<std::string, uint32_t>> registers_;

    void updateRegisters_();
    std::string formatByte_(int byte);
    std::string formatValueBytes_(uint32_t value);
};

#endif //EMULV_REGISTERSWIDGET_H
