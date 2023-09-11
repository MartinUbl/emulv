//
// Created by Hynek on 06.04.2023.
//

#pragma once

#include <QPlainTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QToolButton>

#include "../PeripheralWidget.h"
#include "../../Controller.h"

class UARTWidget : public PeripheralWidget {
    Q_OBJECT
public:
    QPlainTextEdit *text_edit_messages_;
    QLineEdit *line_edit_send_message_;
    QComboBox *combobox_line_separator_;
    QToolButton *button_send_message_;

    explicit UARTWidget(QWidget *parent = nullptr, Controller *controller = nullptr, std::string label = "");

    void SetReadonly(bool readonly) override;
    void AppendChar(char c) const;
    void Clear() const;

private slots:
    void OnSendMessageTextChanged() const;
    void OnSendMessageEnterPressed();
    void OnSendMessageClicked();

private:
    const QString kNoNewLine;
    const QString kLF = "LF";
    const QString kCRLF = "CRLF";
};
