#pragma once

#include <unordered_set>
#include <QGroupBox>
#include <QPlainTextEdit>
#include <QScrollArea>

#include "BreakpointAreaWidget.h"

class DisassemblyWidget : public QWidget {
    Q_OBJECT
public:
    QPlainTextEdit *text_edit_addresses_;
    QPlainTextEdit *text_edit_instructions_;
    QScrollArea *scroll_area_breakpoints_;
    BreakpointAreaWidget *breakpoint_area_widget_;

    explicit DisassemblyWidget(QWidget *parent = nullptr, EmulvApi *controller = nullptr);

    void SetInstructions(const std::vector<std::tuple<uint64_t, std::string>> &instructions);
    void HighlightLine(uint64_t address);
    void Clear();

private slots:
    void OnTextEditAddressesScroll();
    void OnTextEditInstructionsScroll();

private:
    // Address template used for setting the address text edit width
    const QString kAddressTemplate = "00000000 ";
    const int kAddressWidth = 8;

    EmulvApi *controller_;

    std::vector<uint64_t> addresses_;
    // Map for quickly finding the corresponding line of an address
    std::unordered_map<uint64_t, int> address_lines_;

    static std::string InstructionSubstring(const std::string &full_instruction);

    void UpdateBreakpointWidget() const;
    void UpdateScroll(int value) const;
    void AddBreakpoint(int line);
    void RemoveBreakpoint(int line);
    int FindLine(uint64_t address);
};