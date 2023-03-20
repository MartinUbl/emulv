#ifndef DISASSEMBLYVIEW_H
#define DISASSEMBLYVIEW_H

#include <QPlainTextEdit>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LineNumberArea;

class DisassemblyView : public QPlainTextEdit
{
    Q_OBJECT

public:
    DisassemblyView(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void highlightLine(int line);
    int getHighlightedLine();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget *lineNumberArea;
    int highlightedLine = -1;
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(DisassemblyView *dv) : QWidget(dv), disassemblyView(dv)
    {}

    QSize sizeHint() const override
    {
        return QSize(disassemblyView->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        disassemblyView->lineNumberAreaPaintEvent(event);
    }

private:
    DisassemblyView *disassemblyView;
};

#endif
