#include "DisassemblyWidget.h"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <QLayout>
#include <QScrollBar>
#include <QLabel>
#include <QTextBlock>

DisassemblyWidget::DisassemblyWidget(QWidget *parent, Controller *controller)
: QWidget(parent)
, controller_(controller) {
    setLayout(new QVBoxLayout(this));
    layout()->setSpacing(0);
    layout()->setContentsMargins(0, 0, 0, 0);

    auto top_widget = new QWidget(this);
    top_widget->setLayout(new QHBoxLayout(top_widget));
    top_widget->layout()->setSpacing(0);
    top_widget->layout()->setContentsMargins(0, 0, 0, 0);
    top_widget->layout()->addWidget(new QLabel("Disassembly", top_widget));
    top_widget->layout()->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));

    auto bot_widget = new QGroupBox(this);
    bot_widget->setLayout(new QHBoxLayout(bot_widget));
    bot_widget->layout()->setSpacing(0);
    bot_widget->layout()->setContentsMargins(0, 0, 0, 0);

    text_edit_addresses_ = new QPlainTextEdit(bot_widget);
    text_edit_instructions_ = new QPlainTextEdit(bot_widget);
    scroll_area_breakpoints_ = new QScrollArea(bot_widget);

    bot_widget->layout()->addWidget(scroll_area_breakpoints_);
    bot_widget->layout()->addWidget(text_edit_addresses_);
    bot_widget->layout()->addWidget(text_edit_instructions_);

    layout()->addWidget(top_widget);
    layout()->addWidget(bot_widget);

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    text_edit_addresses_->setFont(font);
    text_edit_instructions_->setFont(font);

    text_edit_addresses_->document()->setDocumentMargin(0);
    text_edit_instructions_->document()->setDocumentMargin(0);

    breakpoint_area_widget_ = new BreakpointAreaWidget(scroll_area_breakpoints_, controller);

    breakpoint_area_widget_->setFixedWidth(text_edit_addresses_->fontMetrics().height());
    // Background color has to be set using the widget name,
    // otherwise the child breakpoints would have the same (partially transparent) background
    breakpoint_area_widget_->setObjectName("breakpoint_area_widget_");
    breakpoint_area_widget_->setStyleSheet("#breakpoint_area_widget_ { background-color: rgba(100, 100, 100, 100); }");

    scroll_area_breakpoints_->setFixedWidth(text_edit_addresses_->fontMetrics().height());
    scroll_area_breakpoints_->setFrameShape(QFrame::Box);
    scroll_area_breakpoints_->setLineWidth(0);
    scroll_area_breakpoints_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll_area_breakpoints_->verticalScrollBar()->setEnabled(false);

    scroll_area_breakpoints_->setWidget(breakpoint_area_widget_);

    text_edit_addresses_->setStyleSheet("background-color: rgba(100, 100, 100, 100);");
    text_edit_addresses_->setMaximumWidth(text_edit_addresses_->fontMetrics().horizontalAdvance(kAddressTemplate));
    text_edit_addresses_->setReadOnly(true);
    text_edit_addresses_->setFrameShape(QFrame::Box);
    text_edit_addresses_->setLineWidth(0);
    text_edit_addresses_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    text_edit_addresses_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    text_edit_addresses_->setWordWrapMode(QTextOption::NoWrap);

    text_edit_instructions_->setReadOnly(true);
    text_edit_instructions_->setFrameShape(QFrame::Box);
    text_edit_instructions_->setLineWidth(0);
    text_edit_instructions_->setWordWrapMode(QTextOption::NoWrap);

    connect(text_edit_addresses_->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(OnTextEditAddressesScroll()));
    connect(text_edit_instructions_->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(OnTextEditInstructionsScroll()));

    controller_->GetEventEmitter().On("breakpoint-added", [this](EventsLib::EventData data) {
        int line = std::any_cast<int>(data.getData("line"));
        AddBreakpoint(line);
    });

    controller_->GetEventEmitter().On("breakpoint-removed", [this](EventsLib::EventData data) {
        int line = std::any_cast<int>(data.getData("line"));
        RemoveBreakpoint(line);
    });
}

void DisassemblyWidget::HighlightLine(uint64_t address) {
    QList<QTextEdit::ExtraSelection> extraSelections;

    int line = FindLine(address);

    if (line >= 0) {
        QTextEdit::ExtraSelection selection;
        const QColor lineBgColor = QColor(160, 0, 0, 100);
        selection.format.setBackground(lineBgColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);

        QTextCursor cursor(text_edit_instructions_->document()->findBlockByLineNumber(line));
        text_edit_instructions_->setTextCursor(cursor);

        selection.cursor = cursor;
        cursor.select(QTextCursor::LineUnderCursor);
        selection.cursor.clearSelection();

        extraSelections.append(selection);
    }

    text_edit_instructions_->setExtraSelections(extraSelections);

    int scroll = text_edit_instructions_->verticalScrollBar()->value();
    UpdateScroll(scroll);
}

void DisassemblyWidget::Clear() {
    addresses_.clear();
    address_lines_.clear();
    text_edit_addresses_->clear();
    text_edit_instructions_->clear();
    breakpoint_area_widget_->Clear();
}

void DisassemblyWidget::OnTextEditAddressesScroll() {
    QScrollBar *sb = text_edit_addresses_->verticalScrollBar();
    UpdateScroll(sb->value());
}

void DisassemblyWidget::OnTextEditInstructionsScroll() {
    QScrollBar *sb = text_edit_instructions_->verticalScrollBar();
    UpdateScroll(sb->value());
}

void DisassemblyWidget::UpdateScroll(int value) const {
    QScrollBar *sbBrk = scroll_area_breakpoints_->verticalScrollBar();
    QScrollBar *sbAddr = text_edit_addresses_->verticalScrollBar();
    QScrollBar *sbInstr = text_edit_instructions_->verticalScrollBar();

    sbBrk->setValue(value * text_edit_addresses_->fontMetrics().height());
    sbAddr->setValue(value);
    sbInstr->setValue(value);
}

void DisassemblyWidget::SetInstructions(const std::vector<std::tuple<uint64_t, std::string>> &instructions) {
    Clear();

    std::stringstream ss_addresses;
    std::stringstream ss_instructions;

    int line = 0;
    for (const auto &instruction: instructions) {
        auto address = std::get<0>(instruction);
        auto full_instruction = std::get<1>(instruction);

        std::stringstream ss_address;
        ss_address << std::uppercase << std::hex << std::setw(kAddressWidth) << std::setfill('0') << address;

        ss_addresses << ss_address.str() << '\n';
        ss_instructions << " " << InstructionSubstring(full_instruction) << '\n';

        addresses_.push_back(address);
        address_lines_[address] = line++;
    }

    std::string addressAreaText = ss_addresses.str();
    std::string instructionAreaText = ss_instructions.str();

    //Remove last line separator
    addressAreaText = addressAreaText.erase(addressAreaText.length() - 1);
    instructionAreaText = instructionAreaText.erase(instructionAreaText.length() - 1);

    text_edit_addresses_->setPlainText(QString::fromStdString(addressAreaText));
    text_edit_instructions_->setPlainText(QString::fromStdString(instructionAreaText));

    UpdateBreakpointWidget();
}

void DisassemblyWidget::UpdateBreakpointWidget() const {
    int line_height = text_edit_addresses_->fontMetrics().height();
    int lines = text_edit_addresses_->document()->blockCount();

    breakpoint_area_widget_->SetMaximumBreakpoints(lines);
    breakpoint_area_widget_->setFixedHeight((lines + 1) * line_height);
    // The additional line is for the scroll area to behave correctly,
    // as the QPlainTextEdit has an empty space at the bottom
}

std::string DisassemblyWidget::InstructionSubstring(const std::string &full_instruction) {
    size_t i = 0;

    // Skip binary representation
    for (; i < full_instruction.size() && full_instruction[i] != ' '; ++i) {}

    // Skip space after binary representation
    for (; i < full_instruction.size() && full_instruction[i] == ' '; ++i) {}

    return full_instruction.substr(i);
}

int DisassemblyWidget::FindLine(uint64_t address) {
    auto it = address_lines_.find(address);
    if (it == address_lines_.end()) {
        return -1;
    }

    return it->second;
}

void DisassemblyWidget::AddBreakpoint(int line) {
    controller_->AddBreakpoint(addresses_.at(line));
}

void DisassemblyWidget::RemoveBreakpoint(int line) {
    controller_->RemoveBreakpoint(addresses_.at(line));
}
