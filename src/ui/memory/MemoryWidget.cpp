//
// Created by Hynek on 25.04.2023.
//
#include "MemoryWidget.h"

#include <sstream>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QLabel>
#include <QGroupBox>

#include "MemoryFormatter.h"

MemoryWidget::MemoryWidget(QWidget *parent, EmulvApi *controller)
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
    spinbox_memory_from_ = new QSpinBox(top_widget);
    spinbox_memory_to_ = new QSpinBox(top_widget);
    btn_search_ = new QToolButton(top_widget);
    btn_restore_ = new QToolButton(top_widget);

    spinbox_memory_from_->setDisplayIntegerBase(16);
    spinbox_memory_from_->setPrefix("0x");
    spinbox_memory_from_->setMinimumWidth(80);
    spinbox_memory_from_->setMaximum(memory_to_);

    spinbox_memory_to_->setDisplayIntegerBase(16);
    spinbox_memory_to_->setPrefix("0x");
    spinbox_memory_to_->setMinimumWidth(80);
    spinbox_memory_to_->setMaximum(memory_to_);
    spinbox_memory_to_->setValue(spinbox_memory_to_->maximum());

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
    top_layout->addWidget(spinbox_memory_from_);
    top_layout->addWidget(new QLabel(" - ", top_widget));
    top_layout->addWidget(spinbox_memory_to_);
    top_layout->addWidget(btn_search_);
    top_layout->addWidget(btn_restore_);

    // Initialize bottom widget (Memory header, memory contents)
    auto *bot_widget = new QGroupBox(this);
    auto *bot_layout = new QVBoxLayout(bot_widget);
    bot_layout->setSpacing(0);
    bot_layout->setContentsMargins(0, 0, 0, 0);

    text_edit_header_ = new QTextEdit(bot_widget);
    text_edit_memory_ = new QPlainTextEdit(bot_widget);

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    text_edit_header_->setFont(font);
    text_edit_memory_->setFont(font);

    text_edit_header_->setStyleSheet("background-color: rgba(100, 100, 100, 100);");
    text_edit_header_->setFrameShape(NoFrame);
    text_edit_header_->setReadOnly(true);
    text_edit_header_->document()->setMaximumBlockCount(1);
    text_edit_header_->setWordWrapMode(QTextOption::NoWrap);
    text_edit_header_->verticalScrollBar()->setEnabled(false);
    text_edit_header_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    text_edit_header_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    text_edit_header_->setFixedHeight(text_edit_header_->fontMetrics().height());

    text_edit_memory_->setFrameShape(NoFrame);
    text_edit_memory_->setReadOnly(true);
    text_edit_memory_->setWordWrapMode(QTextOption::NoWrap);

    bot_layout->addWidget(text_edit_header_);
    bot_layout->addWidget(text_edit_memory_);

    // Add top and bottom widgets to main layout
    setLayout(new QVBoxLayout(this));
    layout()->setSpacing(3);
    layout()->setContentsMargins(0, 0, 0, 0);
    layout()->addWidget(top_widget);
    layout()->addWidget(bot_widget);

    UpdateSpinBoxes();
    UpdateButtons();

    rb_hex_->setChecked(true);

    connect(rb_hex_, SIGNAL(clicked(bool)), this, SLOT(OnRBHexClicked()));
    connect(rb_dec_, SIGNAL(clicked(bool)), this, SLOT(OnRBDecClicked()));
    connect(spinbox_memory_from_, SIGNAL(valueChanged(int)), this, SLOT(OnSpinBoxMemoryFromChanged()));
    connect(spinbox_memory_to_, SIGNAL(valueChanged(int)), this, SLOT(OnSpinBoxMemoryToChanged()));
    connect(btn_search_, SIGNAL(clicked(bool)), this, SLOT(OnSearchClicked()));
    connect(btn_restore_, SIGNAL(clicked(bool)), this, SLOT(OnRestoreClicked()));
    connect(text_edit_header_->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(OnHeaderHScrollChanged()));
    connect(text_edit_memory_->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(OnMemoryHScrollChanged()));
}

void MemoryWidget::SetAddressRangeLimit(const int min, const int max) {
    if (min > kMaxAddress || max > kMaxAddress) {
        throw std::invalid_argument("Address out of bounds");
    }
    if (min < 0 || max < 0) {
        throw std::invalid_argument("Address cannot be a negative integer.");
    }
    if (min > max) {
        throw std::invalid_argument("Invalid address range.");
    }

    spinbox_memory_from_->setMinimum(min);
    spinbox_memory_to_->setMaximum(max);

    if (min > memory_from_) {
        memory_from_ = min;
    }
    if (max < memory_to_) {
        memory_to_ = max;
    }

    UpdateSpinBoxes();
    UpdateButtons();
}

void MemoryWidget::UpdateMemory() {
    try {
        SetAddressRangeLimit(controller_->getRamStartAddress(), controller_->getRamEndAddress());
        memory_ = controller_->getMemory(memory_from_, memory_to_);
    } catch (...) {
        text_edit_memory_->setPlainText("Failed to read memory");
        return;
    }

    RefreshMemory();
}

void MemoryWidget::Clear() {
    memory_.clear();
    text_edit_memory_->clear();
}

void MemoryWidget::OnRBHexClicked() {
    RefreshMemory();
}

void MemoryWidget::OnRBDecClicked() {
    RefreshMemory();
}

void MemoryWidget::OnSpinBoxMemoryFromChanged() {
    spinbox_memory_to_->setMinimum(spinbox_memory_from_->value());
    UpdateButtons();
}

void MemoryWidget::OnSpinBoxMemoryToChanged() {
    spinbox_memory_from_->setMaximum(spinbox_memory_to_->value());
    UpdateButtons();
}

void MemoryWidget::OnSearchClicked() {
    memory_from_ = spinbox_memory_from_->value();
    memory_to_ = spinbox_memory_to_->value();

    UpdateSpinBoxes();
    UpdateButtons();
    UpdateMemory();
}

void MemoryWidget::OnRestoreClicked() {
    UpdateSpinBoxes();
}

void MemoryWidget::OnMemoryHScrollChanged() {
    UpdateScroll(text_edit_memory_->horizontalScrollBar()->value());
}

void MemoryWidget::OnHeaderHScrollChanged() {
    UpdateScroll(text_edit_header_->horizontalScrollBar()->value());
}

void MemoryWidget::UpdateScroll(int value) const {
    text_edit_header_->horizontalScrollBar()->setValue(value);
    text_edit_memory_->horizontalScrollBar()->setValue(value);
}

void MemoryWidget::RefreshMemory() {
    MemoryFormat format = rb_hex_->isChecked() ? kHex : kDec;

    int v_scroll = text_edit_memory_->verticalScrollBar()->value();
    int h_scroll = text_edit_memory_->horizontalScrollBar()->value();

    text_edit_header_->setPlainText(QString::fromStdString(MemoryFormatter::FormatHeader(format)));
    text_edit_memory_->setPlainText(QString::fromStdString(MemoryFormatter::FormatMemory(memory_from_, memory_, format)));

    text_edit_memory_->verticalScrollBar()->setValue(v_scroll);
    text_edit_memory_->horizontalScrollBar()->setValue(h_scroll);
}

void MemoryWidget::UpdateSpinBoxes() const {
    spinbox_memory_from_->setValue(memory_from_);
    spinbox_memory_to_->setValue(memory_to_);
}

void MemoryWidget::UpdateButtons() const {
    bool enabled = spinbox_memory_from_->value() != memory_from_ ||
                   spinbox_memory_to_->value() != memory_to_;
    btn_restore_->setEnabled(enabled);
}
