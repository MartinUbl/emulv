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
    QTextEdit *addressArea;
    QTextEdit *instructionArea;
    QScrollArea *breakpointScrollArea;
    BreakpointAreaWidget *breakpointAreaWidget;

    explicit DisassemblyWidget(QWidget *parent = nullptr, Controller *controller = nullptr);

    void setInstructions(const std::vector<std::tuple<uint64_t, std::string>> &instructions);

    void highlightLine(uint64_t address);

    void updateBreakpointWidget();

private slots:

    void onBreakpointScrollAreaScroll();

    void onAddressAreaScroll();

    void onInstructionAreaScroll();

private:
    static constexpr int kBottomPadding = 8;

    Controller *controller_;

    std::vector<uint64_t> addresses_;
    std::unordered_map<uint64_t, int> address_lines_;

    void updateScroll(int value);

    static std::string instructionSubstring_(const std::string &fullString);

    int findLine_(uint64_t address);

    void addBreakpoint_(int line);
    void removeBreakpoint_(int line);
};

#endif // DISASSEMBLYWIDGET_H
