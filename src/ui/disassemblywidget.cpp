#include "disassemblywidget.h"

#include <sstream>
#include <QLayout>
#include <QTextBlock>
#include <QScrollBar>
#include <QAbstractItemModel>

DisassemblyWidget::DisassemblyWidget(QWidget *parent)
        : QGroupBox{parent},
          addressArea(new QTextEdit(this)),
          instructionArea(new QTextEdit(this)),
          breakpointScrollArea(new QScrollArea(this)) {
    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    addressArea->setFont(font);
    instructionArea->setFont(font);

    breakpointAreaWidget = new BreakpointAreaWidget(breakpointScrollArea);

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
    addressArea->setAlignment(Qt::AlignRight);
    addressArea->setFrameShape(QFrame::Box);
    addressArea->setLineWidth(0);
    addressArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    addressArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    addressArea->setWordWrapMode(QTextOption::NoWrap);

    instructionArea->setReadOnly(true);
    instructionArea->setFrameShape(QFrame::Box);
    instructionArea->setLineWidth(0);
    instructionArea->setWordWrapMode(QTextOption::NoWrap);

    QHBoxLayout *layout = new QHBoxLayout(this);
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
}

void DisassemblyWidget::addInstruction(QString address, QString instruction) {
    addressArea->append(address);
    instructionArea->append(instruction);
    updateBreakpointWidget();
}


void DisassemblyWidget::highlightLine(int line) {
    QList<QTextEdit::ExtraSelection> extraSelections;

    highlightedLine = line;

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

int DisassemblyWidget::getHighlightedLine() {
    return highlightedLine;
}

int DisassemblyWidget::getInstructionCount() {
    return addressArea->document()->blockCount();
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
    addressArea->clear();
    instructionArea->clear();
    breakpointAreaWidget->clear();

    //Block UI signals
    QSignalBlocker blocker1(this);
    QSignalBlocker blocker2(this->parent());

    std::stringstream ssAddresses;
    std::stringstream ssInstructions;

    for (const std::string &instructionString : instructionsList) {
        //Parse the instruction string
        std::string address;
        std::string instruction;
        ParseInstructionString(instructionString, address, instruction);

        ssAddresses << address << "\n";
        ssInstructions << instruction << "\n";
    }

    std::string addressAreaText = ssAddresses.str();
    std::string instructionAreaText = ssInstructions.str();

    //Remove last line separator
    addressAreaText = addressAreaText.erase(addressAreaText.length() - 1);
    instructionAreaText = instructionAreaText.erase(instructionAreaText.length() - 1);

    addressArea->setText(QString::fromStdString(addressAreaText));
    instructionArea->setText(QString::fromStdString(instructionAreaText));
    updateBreakpointWidget();
}

void DisassemblyWidget::updateBreakpointWidget() {
    int lineHeight = addressArea->fontMetrics().height();
    int lines = addressArea->document()->blockCount();

    breakpointAreaWidget->setMaximumBreakpoints(lines);
    breakpointAreaWidget->setFixedHeight(lines * lineHeight + 8);
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
