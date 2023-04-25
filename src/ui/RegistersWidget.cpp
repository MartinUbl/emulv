//
// Created by Hynek on 19.04.2023.
//

#include <sstream>
#include <iomanip>
#include <QHBoxLayout>
#include <QLabel>

#include "RegistersWidget.h"

RegistersWidget::RegistersWidget(QWidget *parent)
: QFrame(parent)
, registers_()
, main_text_edit_(new QTextEdit(this)) {
    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    main_text_edit_->setFont(font);

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

    connect(rb_hex_, SIGNAL(clicked(bool)), this, SLOT(rb_hex_clicked_()));
    connect(rb_dec_, SIGNAL(clicked(bool)), this, SLOT(rb_dec_clicked_()));
}

void RegistersWidget::setRegisters(const std::vector<std::tuple<std::string, uint32_t>> &registers) {
    registers_.clear();

    for (const auto &reg: registers) {
        registers_.push_back(reg);
    }

    updateRegisters_();
}

void RegistersWidget::rb_hex_clicked_() {
    updateRegisters_();
}

void RegistersWidget::rb_dec_clicked_() {
    updateRegisters_();
}

void RegistersWidget::updateRegisters_() {
    std::stringstream ss;

    for (const auto &reg : registers_) {
        std::stringstream ssLabel;
        ssLabel << std::setw(4) << std::setfill(' ') << std::get<0>(reg);
        ss << ssLabel.str() << "   " << formatValueBytes_(std::get<1>(reg));

        if (rb_dec_->isChecked()) {
            ss << "(" << std::get<1>(reg) << ')';
        }
        ss << '\n';
    }

    main_text_edit_->setText(QString::fromStdString(ss.str()));
}

std::string RegistersWidget::formatByte_(int byte) {
    std::stringstream ssByte;

    if (rb_hex_->isChecked()) {
        ssByte << std::uppercase << std::hex << std::setw(2);
    }
    else {
        ssByte << std::setw(3);
    }

    ssByte << std::setfill('0') << byte;
    return ssByte.str();
}

std::string RegistersWidget::formatValueBytes_(uint32_t value) {
    std::stringstream ss;

    for (int i = sizeof(value) - 1; i >= 0; --i) {
        uint8_t byte = (value >> (i * 8)) & 0xff;
        ss << formatByte_(byte) << ' ';
    }

    return ss.str();
}
