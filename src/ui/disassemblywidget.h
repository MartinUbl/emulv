#ifndef DISASSEMBLYWIDGET_H
#define DISASSEMBLYWIDGET_H

#include <QGroupBox>
#include <QTextEdit>
#include <QGridLayout>
#include <QObject>
#include <QFrame>
#include <QScrollArea>
#include <unordered_set>
#include "BreakpointAreaWidget.h"

class DisassemblyWidget : public QGroupBox {
Q_OBJECT
public:
    explicit DisassemblyWidget(QWidget *parent = nullptr);

    void addInstruction(QString address, QString instruction);

    void addInstructionsList(const std::vector<std::string> &instructionsList);

    void highlightLine(int line);

    int getHighlightedLine();

    int getInstructionCount();

    void updateBreakpointWidget();

    uint64_t computeLineNumber(uint64_t pc_value);

    std::unordered_set<int64_t> getBreakpointAddresses();

private slots:

    void onBreakpointScrollAreaScroll();

    void onAddressAreaScroll();

    void onInstructionAreaScroll();

private:
    QTextEdit *addressArea;
    QTextEdit *instructionArea;
    QScrollArea *breakpointScrollArea;
    BreakpointAreaWidget *breakpointAreaWidget;

    int highlightedLine = -1;

    void updateScroll(int value);

    static void
    ParseInstructionString(const std::string &instructionString, std::string &address, std::string &instruction);

    uint64_t getFirstAddress() const;
};

#endif // DISASSEMBLYWIDGET_H
