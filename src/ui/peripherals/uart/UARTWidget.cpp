//
// Created by Hynek on 06.04.2023.
//

#include <QVBoxLayout>
#include "UARTWidget.h"

UARTWidget::UARTWidget(QWidget *parent, Controller *controller, std::string label)
: PeripheralWidget(parent, controller, label)
, textEditMessages_(new QTextEdit(this))
, lineEditSendMessage_(new QLineEdit(this))
, comboBoxLineSeparator_(new QComboBox(this))
, buttonSendMessage_(new QToolButton(this)) {
    auto *main_layout = new QVBoxLayout(this);
    auto *bot_widget = new QWidget(this);
    auto *send_layout = new QHBoxLayout(bot_widget);

    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);

    send_layout->setSpacing(3);
    send_layout->setContentsMargins(0, 0, 0, 0);

    textEditMessages_->setReadOnly(true);
    textEditMessages_->setWordWrapMode(QTextOption::NoWrap);

    lineEditSendMessage_->setClearButtonEnabled(true);
    lineEditSendMessage_->setPlaceholderText("Send message...");

    comboBoxLineSeparator_->addItem("LF");
    comboBoxLineSeparator_->addItem("CRLF");

    buttonSendMessage_->setEnabled(false);
    buttonSendMessage_->setIcon(QIcon(":img/send.svg"));
    buttonSendMessage_->setToolTip("Send");

    connect(lineEditSendMessage_, SIGNAL(textChanged(QString)), this, SLOT(on_lineEditSendMessage_textChanged()));
    connect(buttonSendMessage_, SIGNAL(clicked(bool)), this, SLOT(on_buttonSendMessage_clicked()));

    this->setLayout(main_layout);

    main_layout->addWidget(textEditMessages_);
    main_layout->addWidget(bot_widget);

    send_layout->addWidget(lineEditSendMessage_);
    send_layout->addWidget(comboBoxLineSeparator_);
    send_layout->addWidget(buttonSendMessage_);
}

void UARTWidget::setReadonly(bool readonly) {
    textEditMessages_->setReadOnly(readonly);
    comboBoxLineSeparator_->setEnabled(!readonly);
    buttonSendMessage_->setEnabled(!readonly);
}

void UARTWidget::appendChar(char c) {
    textEditMessages_->moveCursor(QTextCursor::End);
    textEditMessages_->insertPlainText(QChar(c));
    textEditMessages_->moveCursor(QTextCursor::End);
}

void UARTWidget::clear() {
    textEditMessages_->clear();
}

void UARTWidget::on_lineEditSendMessage_textChanged() {
    buttonSendMessage_->setEnabled(!lineEditSendMessage_->text().isEmpty());
}

void UARTWidget::on_buttonSendMessage_clicked() {
    std::string message = lineEditSendMessage_->text().toStdString();
    std::string newLine = comboBoxLineSeparator_->currentText() == QString::fromStdString("LF") ? "\n" : "\r\n";

    message.append(newLine);

    controller_->SendUartMessage(label_, message);

    lineEditSendMessage_->clear();
}