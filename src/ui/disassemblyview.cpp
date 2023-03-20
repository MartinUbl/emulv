#include "disassemblyview.h"

#include <QPainter>
#include <QTextBlock>

DisassemblyView::DisassemblyView(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, &DisassemblyView::blockCountChanged, this, &DisassemblyView::updateLineNumberAreaWidth);
    connect(this, &DisassemblyView::updateRequest, this, &DisassemblyView::updateLineNumberArea);

    updateLineNumberAreaWidth(0);

    setReadOnly(true);
    setLineWrapMode(LineWrapMode::NoWrap);

    QFont font = this->font();
    font.setFamily("monospace");
    this->setFont(font);
}

int DisassemblyView::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = 6 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void DisassemblyView::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 6, 0);
}

void DisassemblyView::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
    {
        lineNumberArea->scroll(0, dy);
    }
    else
    {
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect()))
    {
        updateLineNumberAreaWidth(0);
    }
}

void DisassemblyView::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void DisassemblyView::highlightLine(int line)
{
    highlightedLine = line;

    QList<QTextEdit::ExtraSelection> extraSelections;

    if (line >= 0)
    {
        QTextEdit::ExtraSelection selection;
        const QColor lineBgColor = QColor(160, 0, 0, 100);
        selection.format.setBackground(lineBgColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);

        QTextCursor cursor(document()->findBlockByLineNumber(static_cast<int>(line)));
        setTextCursor(cursor);

        selection.cursor = cursor;
        cursor.select(QTextCursor::LineUnderCursor);
        selection.cursor.clearSelection();

        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

int DisassemblyView::getHighlightedLine()
{
    return highlightedLine;
}

void DisassemblyView::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor(100, 100, 100, 100));

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

