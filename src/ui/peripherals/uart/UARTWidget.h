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

class UARTWidget : public PeripheralWidget {
    Q_OBJECT
public:
    UARTWidget(QWidget *parent = nullptr);
    void setReadonly(bool readonly) override;
    void appendMessage(std::string message);
    void clear();

private slots:
    void on_lineEditSendMessage_textChanged();
    void on_buttonSendMessage_clicked();

private:
    QTextEdit *textEditMessages_;
    QLineEdit *lineEditSendMessage_;
    QComboBox *comboBoxLineSeparator_;
    QToolButton *buttonSendMessage_;
};

#endif //EMULV_UARTWIDGET_H
