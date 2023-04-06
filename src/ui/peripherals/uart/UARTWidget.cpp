//
// Created by Hynek on 06.04.2023.
//

#include <QVBoxLayout>
#include "UARTWidget.h"

UARTWidget::UARTWidget(QWidget *parent)
    : QWidget(parent)
    , textEditMessages_(new QTextEdit(this))
    , lineEditSendMessage_(new QLineEdit(this))
    , comboBoxLineSeparator_(new QComboBox(this))
    , buttonSendMessage_(new QToolButton(this)) {
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    QHBoxLayout *send_layout = new QHBoxLayout(this);

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
    buttonSendMessage_->setFixedSize(lineEditSendMessage_->height() - 6, lineEditSendMessage_->height() - 6);
    buttonSendMessage_->setIcon(QIcon(":img/send.svg"));
    buttonSendMessage_->setIconSize(QSize(buttonSendMessage_->height() - 2, buttonSendMessage_->height() - 2));

    connect(lineEditSendMessage_, SIGNAL(textChanged(const QString &)), this, SLOT(on_lineEditSendMessage_textChanged()));
    connect(comboBoxLineSeparator_, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBoxLineSeparator_selected()));
    connect(buttonSendMessage_, SIGNAL(clicked(bool)), this, SLOT(on_buttonSendMessage_clicked()));

    this->setLayout(main_layout);

    main_layout->addWidget(textEditMessages_);
    main_layout->addLayout(send_layout);

    send_layout->addWidget(lineEditSendMessage_);
    send_layout->addWidget(comboBoxLineSeparator_);
    send_layout->addWidget(buttonSendMessage_);
}

void UARTWidget::appendMessage(std::string message) {
    textEditMessages_->append(QString::fromStdString(message));
}

void UARTWidget::clear() {
    textEditMessages_->clear();
}

void UARTWidget::on_lineEditSendMessage_textChanged() {
    buttonSendMessage_->setEnabled(!lineEditSendMessage_->text().isEmpty());
}

void UARTWidget::on_buttonSendMessage_clicked() {
    appendMessage(lineEditSendMessage_->text().toStdString()); // To be removed
    lineEditSendMessage_->clear();
}

void UARTWidget::on_comboBoxLineSeparator_selected() {

}