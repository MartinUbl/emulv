#ifndef DISASSEMBLYWIDGET_H
#define DISASSEMBLYWIDGET_H

#include <QGroupBox>
#include <QPlainTextEdit>
#include <QGridLayout>
#include <QObject>
#include <QFrame>
#include <QScrollArea>
#include "BreakpointAreaWidget.h"

class DisassemblyWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit DisassemblyWidget(QWidget *parent = nullptr);
    void addInstruction(QString address, QString instruction);
    void addInstructionsList(const std::vector<std::string> &instructionsList);
    void highlightLine(int line);
    int getHighlightedLine();
    int getInstructionCount();
    void updateBreakpointWidget();

signals:

private slots:
    void onBreakpointScrollAreaScroll();
    void onAddressAreaScroll();
    void onInstructionAreaScroll();

private:
    QPlainTextEdit *addressArea;
    QPlainTextEdit *instructionArea;
    QScrollArea *breakpointScrollArea;
    BreakpointAreaWidget *breakpointAreaWidget;

    int highlightedLine = -1;

    void updateScroll(int value);

    static void ParseInstructionString(const std::string &instructionString, std::string &address, std::string &instruction);
};

#endif // DISASSEMBLYWIDGET_H
