//
// Created by Hynek on 19.04.2023.
//
#include "RegistersWidget.h"

#include <sstream>
#include <iomanip>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QLabel>

RegistersWidget::RegistersWidget(QWidget *parent)
: QFrame(parent)
, registers_()
, main_text_edit_(new QTextEdit(this)) {
    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    main_text_edit_->setFont(font);
    main_text_edit_->setReadOnly(true);
    main_text_edit_->setWordWrapMode(QTextOption::NoWrap);

    auto top_widget = new QWidget(this);
    auto top_layout = new QHBoxLayout(top_widget);
    top_layout->setSpacing(0);
    top_layout->setContentsMargins(0, 0, 0, 0);

    rb_hex_ = new QRadioButton("Hex", top_widget);
    rb_dec_ = new QRadioButton("Dec", top_widget);

    top_layout->addWidget(new QLabel("Registers", top_widget));
    top_layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
    top_layout->addWidget(rb_hex_);
    top_layout->addWidget(rb_dec_);

    setLayout(new QVBoxLayout(this));
    layout()->setSpacing(0);
    layout()->setContentsMargins(0, 0, 0, 0);
    layout()->addWidget(top_widget);
    layout()->addWidget(main_text_edit_);

    rb_hex_->setChecked(true);

    connect(rb_hex_, SIGNAL(clicked(bool)), this, SLOT(OnRBHexClicked()));
    connect(rb_dec_, SIGNAL(clicked(bool)), this, SLOT(OnRBDecClicked()));
}

void RegistersWidget::SetRegisters(const std::vector<std::tuple<std::string, uint32_t>> &registers) {
    registers_.clear();

    for (const auto &reg: registers) {
        registers_.push_back(reg);
    }

    UpdateRegisters_();
}

void RegistersWidget::OnRBHexClicked() {
    UpdateRegisters_();
}

void RegistersWidget::OnRBDecClicked() {
    UpdateRegisters_();
}

void RegistersWidget::UpdateRegisters_() {
    std::stringstream ss;

    for (size_t i = 0; i < registers_.size(); i++) {
        auto reg = registers_.at(i);

        auto label = std::get<0>(reg);
        auto value = std::get<1>(reg);

        ss << std::setw(kRegisterLabelWidth) << std::setfill(' ') << label;

        // Add alternative name if it exists
        std::string reg_abi;
        if (i < kRegistersCount) {
            reg_abi = '(' + kRegistersABI_Names.at(i) + ')';
        }

        ss << ' ' << std::setw(kRegisterLabelWidth + 2) << std::setfill(' ') << reg_abi; // +2 for parentheses
        ss << "   " << FormatBytes_(value);

        // Add full unsigned register value if in decimal format
        if (rb_dec_->isChecked()) {
            ss << '(' << value << ')';
        }

        ss << '\n';
    }

    int scroll = main_text_edit_->verticalScrollBar()->value();

    main_text_edit_->setText(QString::fromStdString(ss.str()));

    main_text_edit_->verticalScrollBar()->setValue(scroll);
}

std::string RegistersWidget::FormatByte_(int byte) const {
    std::stringstream ss;

    if (rb_hex_->isChecked()) {
        ss << std::setw(2) << std::uppercase << std::hex;
    }
    else {
        ss << std::setw(3);
    }

    ss << std::setfill('0') << byte;
    return ss.str();
}

std::string RegistersWidget::FormatBytes_(uint32_t value) const {
    std::stringstream ss;

    for (int i = sizeof(value) - 1; i >= 0; --i) {
        uint8_t byte = (value >> (i * 8)) & 0xff;
        ss << FormatByte_(byte) << ' ';
    }

    return ss.str();
}
