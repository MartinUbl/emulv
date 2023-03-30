#ifndef DISASSEMBLYWIDGET_H
#define DISASSEMBLYWIDGET_H

#include <QGroupBox>
#include <QTextEdit>
#include <QGridLayout>
#include <QObject>
#include <QFrame>
#include <QScrollArea>

class DisassemblyWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit DisassemblyWidget(QWidget *parent = nullptr);
    void addInstruction(QString address, QString instruction);
    void highlightLine(int line);
    int getHighlightedLine();
    int getInstructionCount();

signals:

private slots:
    void onBreakpointScrollAreaScroll();
    void onAddressAreaScroll();
    void onInstructionAreaScroll();

private:
    QTextEdit *addressArea;
    QTextEdit *instructionArea;
    QVBoxLayout *breakpointAreaLayout;
    QScrollArea *breakpointScrollArea;
    QWidget *breakpointAreaWidget;

    int highlightedLine = -1;
    int instructionCount = 0;

    void updateScroll(int value);
};

#endif // DISASSEMBLYWIDGET_H
