//
// Created by Hynek on 25.04.2023.
//

#include <sstream>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QLabel>
#include <iomanip>
#include "MemoryWidget.h"

MemoryWidget::MemoryWidget(QWidget *parent, Controller *controller)
: QFrame(parent)
, controller_(controller)
, memory_from_(0), memory_to_(kMaxAddress)
, memory_() {
    // Initialize top widget (Label, RadioButtons, SpinBoxes, Buttons)
    auto top_widget = new QWidget(this);
    auto top_layout = new QHBoxLayout(top_widget);
    top_layout->setSpacing(0);
    top_layout->setContentsMargins(0, 0, 0, 0);

    rb_hex_ = new QRadioButton("Hex", top_widget);
    rb_dec_ = new QRadioButton("Dec", top_widget);
    sp_memory_from_ = new QSpinBox(top_widget);
    sp_memory_to_ = new QSpinBox(top_widget);
    btn_search_ = new QToolButton(top_widget);
    btn_restore_ = new QToolButton(top_widget);

    sp_memory_from_->setDisplayIntegerBase(16);
    sp_memory_from_->setPrefix("0x");
    sp_memory_from_->setSuffix("0");
    sp_memory_from_->setMinimumWidth(80);
    sp_memory_from_->setMaximum(memory_to_);

    sp_memory_to_->setDisplayIntegerBase(16);
    sp_memory_to_->setPrefix("0x");
    sp_memory_to_->setSuffix("0");
    sp_memory_to_->setMinimumWidth(80);
    sp_memory_to_->setMaximum(memory_to_);
    sp_memory_to_->setValue(sp_memory_to_->maximum());

    btn_search_->setIconSize(QSize(14, 14));
    btn_search_->setIcon(QIcon(":img/search.png"));
    btn_search_->setToolTip("Confirm selection");

    btn_restore_->setIconSize(QSize(14, 14));
    btn_restore_->setIcon(QIcon(":img/restore.png"));
    btn_restore_->setToolTip("Restore selection");

    top_layout->addWidget(new QLabel("Memory", top_widget));
    top_layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
    top_layout->addWidget(rb_hex_);
    top_layout->addWidget(rb_dec_);
    top_layout->addWidget(sp_memory_from_);
    top_layout->addWidget(new QLabel(" - ", top_widget));
    top_layout->addWidget(sp_memory_to_);
    top_layout->addWidget(btn_search_);
    top_layout->addWidget(btn_restore_);

    // Initialize bottom widget (Memory header, memory contents)
    auto *bot_widget = new QFrame(this);
    auto *bot_layout = new QVBoxLayout(bot_widget);
    bot_layout->setSpacing(0);
    bot_layout->setContentsMargins(0, 0, 0, 0);

    bot_widget->setFrameShape(Box);

    te_header_ = new QTextEdit(bot_widget);
    te_memory_ = new QTextEdit(bot_widget);

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    te_header_->setFont(font);
    te_memory_->setFont(font);

    te_header_->setFrameShape(NoFrame);
    te_header_->setReadOnly(true);
    te_header_->document()->setMaximumBlockCount(1);
    te_header_->setWordWrapMode(QTextOption::NoWrap);
    te_header_->verticalScrollBar()->setEnabled(false);
    te_header_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    te_header_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    te_header_->setFixedHeight(te_header_->fontMetrics().height());

    te_memory_->setFrameShape(NoFrame);
    te_memory_->setReadOnly(true);
    te_memory_->setWordWrapMode(QTextOption::NoWrap);

    bot_layout->addWidget(te_header_);
    bot_layout->addWidget(te_memory_);

    // Add top and bottom widgets to main layout
    setLayout(new QVBoxLayout(this));
    layout()->setSpacing(0);
    layout()->setContentsMargins(0, 0, 0, 0);
    layout()->addWidget(top_widget);
    layout()->addWidget(bot_widget);

    updateMemorySpinBoxes_();
    updateMemoryButtons_();

    rb_hex_->setChecked(true);

    connect(rb_hex_, SIGNAL(clicked(bool)), this, SLOT(rb_hex_clicked_()));
    connect(rb_dec_, SIGNAL(clicked(bool)), this, SLOT(rb_dec_clicked_()));
    connect(sp_memory_from_, SIGNAL(valueChanged(int)), this, SLOT(sp_memory_from_changed_()));
    connect(sp_memory_to_, SIGNAL(valueChanged(int)), this, SLOT(sp_memory_to_changed_()));
    connect(btn_search_, SIGNAL(clicked(bool)), this, SLOT(btn_search_clicked_()));
    connect(btn_restore_, SIGNAL(clicked(bool)), this, SLOT(btn_restore_clicked_()));
    connect(te_header_->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(header_scroll_changed()));
    connect(te_memory_->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(memory_scroll_changed()));
}

void MemoryWidget::setAddressRangeLimit(const int min, const int max) {
    if (min > kMaxAddress || max > kMaxAddress) {
        throw std::invalid_argument("Address out of bounds");
    }
    if (min < 0 || max < 0) {
        throw std::invalid_argument("Address cannot be a negative integer.");
    }
    if (min > max) {
        throw std::invalid_argument("Invalid address range.");
    }
    sp_memory_from_->setMinimum(min);
    sp_memory_to_->setMaximum(max);

    if (min > memory_from_) {
        memory_from_ = min;
    }
    if (max < memory_to_) {
        memory_to_ = max;
    }

    updateMemorySpinBoxes_();
    updateMemoryButtons_();
}

void MemoryWidget::updateMemory() {
    memory_ = controller_->GetMemory(memory_from_, memory_to_);
    updateMemory_();
}

void MemoryWidget::clear() {
    memory_.clear();
    te_memory_->clear();
}

void MemoryWidget::rb_hex_clicked_() {
    updateMemory_();
}

void MemoryWidget::rb_dec_clicked_() {
    updateMemory_();
}

void MemoryWidget::sp_memory_from_changed_() {
    sp_memory_to_->setMinimum(sp_memory_from_->value());
    updateMemoryButtons_();
}

void MemoryWidget::sp_memory_to_changed_() {
    sp_memory_from_->setMaximum(sp_memory_to_->value());
    updateMemoryButtons_();
}

void MemoryWidget::btn_search_clicked_() {
    memory_from_ = sp_memory_from_->value();
    memory_to_ = sp_memory_to_->value();

    updateMemorySpinBoxes_();
    updateMemoryButtons_();
    updateMemory();
}

void MemoryWidget::btn_restore_clicked_() {
    updateMemorySpinBoxes_();
}

void MemoryWidget::memory_scroll_changed() {
    updateScroll_(te_memory_->horizontalScrollBar()->value());
}

void MemoryWidget::header_scroll_changed() {
    updateScroll_(te_header_->horizontalScrollBar()->value());
}

void MemoryWidget::updateScroll_(int value) {
    te_header_->horizontalScrollBar()->setValue(value);
    te_memory_->horizontalScrollBar()->setValue(value);
}

void MemoryWidget::updateMemory_() {
    std::stringstream ss;

    int address = memory_from_;

    for (const auto& bytes : memory_) {
        ss << formatLine_(address, bytes) << '\n';
        ++address;
    }

    te_header_->setText(QString::fromStdString(formatHeader_()));
    te_memory_->setText(QString::fromStdString(ss.str()));
}

void MemoryWidget::updateMemorySpinBoxes_() {
    sp_memory_from_->setValue(memory_from_);
    sp_memory_to_->setValue(memory_to_);
}

void MemoryWidget::updateMemoryButtons_() {
    bool enabled = sp_memory_from_->value() != memory_from_ ||
                   sp_memory_to_->value() != memory_to_;
    btn_search_->setEnabled(enabled);
    btn_restore_->setEnabled(enabled);
}

std::string MemoryWidget::formatByte_(int byte) {
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

std::string MemoryWidget::formatHeader_() {
    if (memory_.empty()) {
        return "";
    }

    std::stringstream ss;
    ss << std::setw(kAddressWidth) << std::setfill(' ') << "";

    if (rb_dec_->isChecked()) {
        return ss.str() + "  00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F    ";
    }
    return ss.str() + " 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F    ";
}

std::string MemoryWidget::formatLine_(int address, const std::vector<uint8_t>& bytes) {
    std::stringstream ss_address;
    std::stringstream ss_bytes;

    ss_address << std::uppercase << std::hex << std::setw(kAddressWidth - 1) << std::setfill('0') << address << '0';
    ss_bytes << " ";

    for (auto byte : bytes) {
        ss_bytes << formatByte_(byte) << " ";
    }

    return ss_address.str() + ss_bytes.str();
}
