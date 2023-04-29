#include "disassemblywidget.h"

#include <sstream>
#include <QLayout>
#include <QTextBlock>
#include <QScrollBar>
#include <QAbstractItemModel>
#include <iostream>
#include <unordered_set>

#include "../utils/events/BreakpointAreaWidgetEvents.h"

DisassemblyWidget::DisassemblyWidget(QWidget *parent, Controller *controller)
: QGroupBox{parent}
, controller_(controller)
, addressArea(new QTextEdit(this))
, instructionArea(new QTextEdit(this))
, breakpointScrollArea(new QScrollArea(this)) {
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

    addressArea->setObjectName("addressArea");
    addressArea->setStyleSheet("#addressArea { background-color: rgba(100, 100, 100, 100); }");
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

    auto *layout = new QHBoxLayout(this);
    layout->addWidget(breakpointScrollArea);
    layout->addWidget(addressArea);
    layout->addWidget(instructionArea);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);

    connect(breakpointScrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), this,
            SLOT(onBreakpointScrollAreaScroll()));
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

void DisassemblyWidget::addInstructionsList(const std::vector<std::string> &instructionsList) {
    addresses_.clear();
    address_lines_.clear();
    addressArea->clear();
    instructionArea->clear();
    breakpointAreaWidget->clear();

    //Block UI signals
    QSignalBlocker blocker1(this);
    QSignalBlocker blocker2(this->parent());

    std::stringstream ssAddresses;
    std::stringstream ssInstructions;

    int line = 0;
    for (const std::string &instructionString: instructionsList) {
        //Parse the instruction string
        std::string address;
        std::string instruction;
        ParseInstructionString(instructionString, address, instruction);

        ssAddresses << address << "\n";
        ssInstructions << instruction << "\n";

        int i_address = std::stoi(address);
        addresses_.push_back(i_address);
        address_lines_[i_address] = line++;
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

void DisassemblyWidget::ParseInstructionString(const std::string &instructionString, std::string &address,
                                               std::string &instruction) {
    int mode = 0;
    for (std::string::size_type i = 0; i < instructionString.size(); ++i) {
        if (instructionString[i] == ' ' && mode == 0) {
            address = instructionString.substr(0, i);
            mode++;
        }

        if (instructionString[i] != ' ' && mode == 1) {
            mode++;
        }

        if (instructionString[i] == ' ' && mode == 2) {
            //Binary representation of instruction
            mode++;
        }

        if (instructionString[i] != ' ' && mode == 3) {
            instruction = instructionString.substr(i, instructionString.size());
            break;
        }
    }
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