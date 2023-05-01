//
// Created by Hynek on 06.04.2023.
//

#ifndef EMULV_UARTWIDGET_H
#define EMULV_UARTWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include "../PeripheralWidget.h"
#include "../../Controller.h"

class UARTWidget : public PeripheralWidget {
    Q_OBJECT
public:
    UARTWidget(QWidget *parent = nullptr, Controller *controller = nullptr, std::string label = "");
    void setReadonly(bool readonly) override;
    void appendChar(char c);
    void clear();

private slots:
    void on_lineEditSendMessage_textChanged();
    void on_lineEditSendMessage_returnPressed();
    void on_buttonSendMessage_clicked();

private:
    const QString kNoNewLine;
    const QString kLF = "LF";
    const QString kCRLF = "CRLF";

    QTextEdit *textEditMessages_;
    QLineEdit *lineEditSendMessage_;
    QComboBox *comboBoxLineSeparator_;
    QToolButton *buttonSendMessage_;
};

#endif //EMULV_UARTWIDGET_H
