#include "DisassemblyWidget.h"

#include <sstream>
#include <QLayout>
#include <QTextBlock>
#include <QScrollBar>
#include <QAbstractItemModel>
#include <QLabel>
#include <iostream>
#include <unordered_set>
#include <iomanip>

#include "../../utils/events/BreakpointAreaWidgetEvents.h"

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

    addressArea = new QTextEdit(bot_widget);
    instructionArea = new QTextEdit(bot_widget);
    breakpointScrollArea = new QScrollArea(bot_widget);

    bot_widget->layout()->addWidget(breakpointScrollArea);
    bot_widget->layout()->addWidget(addressArea);
    bot_widget->layout()->addWidget(instructionArea);

    layout()->addWidget(top_widget);
    layout()->addWidget(bot_widget);

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    addressArea->setFont(font);
    instructionArea->setFont(font);

    breakpointAreaWidget = new BreakpointAreaWidget(breakpointScrollArea, controller);

    breakpointAreaWidget->setFixedWidth(addressArea->fontMetrics().height());
    breakpointAreaWidget->setObjectName("breakpointAreaWidget");
    breakpointAreaWidget->setStyleSheet("#breakpointAreaWidget { background-color: rgba(100, 100, 100, 100); }");

    breakpointScrollArea->setFixedWidth(addressArea->fontMetrics().height());
    breakpointScrollArea->setFrameShape(QFrame::Box);
    breakpointScrollArea->setLineWidth(0);
    breakpointScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    breakpointScrollArea->setWidget(breakpointAreaWidget);

    addressArea->setStyleSheet("background-color: rgba(100, 100, 100, 100);");
    addressArea->setMaximumWidth(addressArea->fontMetrics().horizontalAdvance("00000000 "));
    addressArea->setReadOnly(true);
    addressArea->setFrameShape(QFrame::Box);
    addressArea->setLineWidth(0);
    addressArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    addressArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    addressArea->setWordWrapMode(QTextOption::NoWrap);

    instructionArea->setReadOnly(true);
    instructionArea->setFrameShape(QFrame::Box);
    instructionArea->setLineWidth(0);
    instructionArea->setWordWrapMode(QTextOption::NoWrap);

    connect(breakpointScrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onBreakpointScrollAreaScroll()));
    connect(addressArea->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onAddressAreaScroll()));
    connect(instructionArea->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onInstructionAreaScroll()));

    controller_->GetEventEmitter().On(Breakpoint_Added_Event_Description, [this](AbstractEvent *res) {
        auto event = dynamic_cast<BreakpointAreaWidgetEvent *>(res);
        addBreakpoint_(event->Line);
        delete res;
    });

    controller_->GetEventEmitter().On(Breakpoint_Removed_Event_Description, [this](AbstractEvent *res) {
        auto event = dynamic_cast<BreakpointAreaWidgetEvent *>(res);
        removeBreakpoint_(event->Line);
        delete res;
    });
}

void DisassemblyWidget::highlightLine(uint64_t address) {
    QList<QTextEdit::ExtraSelection> extraSelections;

    int line = findLine_(address);

    if (line >= 0) {
        QTextEdit::ExtraSelection selection;
        const QColor lineBgColor = QColor(160, 0, 0, 100);
        selection.format.setBackground(lineBgColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);

        QTextCursor cursor(instructionArea->document()->findBlockByLineNumber(line));
        instructionArea->setTextCursor(cursor);

        selection.cursor = cursor;
        cursor.select(QTextCursor::LineUnderCursor);
        selection.cursor.clearSelection();

        extraSelections.append(selection);
    }

    instructionArea->setExtraSelections(extraSelections);
}

void DisassemblyWidget::onBreakpointScrollAreaScroll() {
    QScrollBar *sb = breakpointScrollArea->verticalScrollBar();
    updateScroll(sb->value());
}

void DisassemblyWidget::onAddressAreaScroll() {
    QScrollBar *sb = addressArea->verticalScrollBar();
    updateScroll(sb->value());
}

void DisassemblyWidget::onInstructionAreaScroll() {
    QScrollBar *sb = instructionArea->verticalScrollBar();
    updateScroll(sb->value());
}

void DisassemblyWidget::updateScroll(int value) {
    QScrollBar *sbBrk = breakpointScrollArea->verticalScrollBar();
    QScrollBar *sbAddr = addressArea->verticalScrollBar();
    QScrollBar *sbInstr = instructionArea->verticalScrollBar();

    sbBrk->setValue(value);
    sbAddr->setValue(value);
    sbInstr->setValue(value);
}

void DisassemblyWidget::setInstructions(const std::vector<std::tuple<uint64_t, std::string>> &instructions) {
    addresses_.clear();
    address_lines_.clear();
    addressArea->clear();
    instructionArea->clear();
    breakpointAreaWidget->clear();

    std::stringstream ssAddresses;
    std::stringstream ssInstructions;;

    int line = 0;
    for (const auto &instructionString: instructions) {
        uint64_t address = std::get<0>(instructionString);
        std::string instruction = std::get<1>(instructionString);

        std::stringstream ssAddress;
        ssAddress << std::uppercase << std::hex << std::setw(8) << std::setfill('0') << address;

        ssAddresses << ssAddress.str() << '\n';
        ssInstructions << instructionSubstring_(instruction) << '\n';

        addresses_.push_back(address);
        address_lines_[address] = line++;
    }

    std::string addressAreaText = ssAddresses.str();
    std::string instructionAreaText = ssInstructions.str();

    //Remove last line separator
    addressAreaText = addressAreaText.erase(addressAreaText.length() - 1);
    instructionAreaText = instructionAreaText.erase(instructionAreaText.length() - 1);

    addressArea->setPlainText(QString::fromStdString(addressAreaText));
    instructionArea->setPlainText(QString::fromStdString(instructionAreaText));
    updateBreakpointWidget();
}

void DisassemblyWidget::updateBreakpointWidget() {
    int lineHeight = addressArea->fontMetrics().height();
    int lines = addressArea->document()->blockCount();

    breakpointAreaWidget->setMaximumBreakpoints(lines);
    breakpointAreaWidget->setFixedHeight(lines * lineHeight + kBottomPadding);
}

std::string DisassemblyWidget::instructionSubstring_(const std::string &fullString) {
    std::string::size_type i = 0;

    // Skip binary representation
    for (; i < fullString.size() && fullString[i] != ' '; ++i) {}

    // Skip space after binary representation
    for (; i < fullString.size() && fullString[i] == ' '; ++i) {}

    return fullString.substr(i);
}

int DisassemblyWidget::findLine_(uint64_t address) {
    auto it = address_lines_.find(address);
    if (it == address_lines_.end()) {
        return -1;
    }

    return it->second;
}

void DisassemblyWidget::addBreakpoint_(int line) {
    controller_->AddBreakpoint(addresses_.at(line));
}

void DisassemblyWidget::removeBreakpoint_(int line) {
    controller_->RemoveBreakpoint(addresses_.at(line));
}
