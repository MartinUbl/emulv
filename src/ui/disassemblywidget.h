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
    void addInstructionsList(const std::vector<std::string> &instructionsList);
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

    static void ParseInstructionString(const std::string &instructionString, std::string &address, std::string &instruction);
};

#endif // DISASSEMBLYWIDGET_H
