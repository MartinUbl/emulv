//
// Created by Hynek on 19.04.2023.
//

#pragma once

#include <array>
#include <string>
#include <QFrame>
#include <QTextEdit>
#include <QRadioButton>

class RegistersWidget : public QFrame {
    Q_OBJECT
public:
    QRadioButton *rb_hex_, *rb_dec_;
    QTextEdit *main_text_edit_;

    explicit RegistersWidget(QWidget *parent = nullptr);
    void SetRegisters(const std::vector<std::tuple<std::string, uint32_t>> &registers);

private slots:
    void OnRBHexClicked();
    void OnRBDecClicked();

private:
    static const int kRegisterLabelWidth = 4;
    static const int kRegistersCount = 32;
    const std::array<std::string, kRegistersCount> kRegistersABI_Names {
            "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
            "fp", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
            "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
            "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
    };

    std::vector<std::tuple<std::string, uint32_t>> registers_;

    void UpdateRegisters();
    std::string FormatByte(int byte) const;
    std::string FormatBytes(uint32_t value) const;
};
