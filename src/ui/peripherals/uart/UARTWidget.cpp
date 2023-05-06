//
// Created by Hynek on 06.04.2023.
//
#include "UARTWidget.h"

#include <QVBoxLayout>

UARTWidget::UARTWidget(QWidget *parent, Controller *controller, std::string label)
: PeripheralWidget(parent, controller, label)
, text_edit_messages_(new QPlainTextEdit(this))
, line_edit_send_message_(new QLineEdit(this))
, combobox_line_separator_(new QComboBox(this))
, button_send_message_(new QToolButton(this)) {
    auto main_layout = new QVBoxLayout(this);
    main_layout->setSpacing(1);
    main_layout->setContentsMargins(0, 0, 0, 0);

    auto bot_widget = new QWidget(this);
    auto send_layout = new QHBoxLayout(bot_widget);
    send_layout->setSpacing(3);
    send_layout->setContentsMargins(0, 0, 0, 0);

    text_edit_messages_->setReadOnly(true);
    text_edit_messages_->setWordWrapMode(QTextOption::NoWrap);

    line_edit_send_message_->setClearButtonEnabled(true);
    line_edit_send_message_->setPlaceholderText("Send message...");

    combobox_line_separator_->addItem(kNoNewLine);
    combobox_line_separator_->addItem(kLF);
    combobox_line_separator_->addItem(kCRLF);

    button_send_message_->setEnabled(false);
    button_send_message_->setIcon(QIcon(":img/send.svg"));
    button_send_message_->setToolTip("Send");

    connect(line_edit_send_message_, SIGNAL(textChanged(QString)), this, SLOT(OnSendMessageTextChanged()));
    connect(line_edit_send_message_, SIGNAL(returnPressed()), this, SLOT(OnSendMessageEnterPressed()));
    connect(button_send_message_, SIGNAL(clicked(bool)), this, SLOT(OnSendMessageClicked()));

    this->setLayout(main_layout);

    main_layout->addWidget(text_edit_messages_);
    main_layout->addWidget(bot_widget);

    send_layout->addWidget(line_edit_send_message_);
    send_layout->addWidget(combobox_line_separator_);
    send_layout->addWidget(button_send_message_);
}

void UARTWidget::SetReadonly(bool readonly) {
    line_edit_send_message_->setReadOnly(readonly);
    combobox_line_separator_->setEnabled(!readonly);
    button_send_message_->setEnabled(!readonly && !line_edit_send_message_->text().isEmpty());
}

void UARTWidget::AppendChar(char c) const {
    text_edit_messages_->moveCursor(QTextCursor::End);
    text_edit_messages_->insertPlainText(QChar(c));
    text_edit_messages_->moveCursor(QTextCursor::End);
}

void UARTWidget::Clear() const {
    text_edit_messages_->clear();
}

void UARTWidget::OnSendMessageTextChanged() const {
    button_send_message_->setEnabled(!line_edit_send_message_->text().isEmpty());
}

void UARTWidget::OnSendMessageEnterPressed() {
    if (button_send_message_->isEnabled()) {
        OnSendMessageClicked();
    }
}

void UARTWidget::OnSendMessageClicked() {
    auto message = line_edit_send_message_->text().toStdString();
    auto new_line = combobox_line_separator_->currentText();

    if (QString::compare(new_line, kLF) == 0) {
        message.append("\n");
    }
    else if (QString::compare(new_line, kCRLF) == 0) {
        message.append("\r\n");
    }

    controller_->SendUartMessage(label_, message);

    line_edit_send_message_->clear();
}
