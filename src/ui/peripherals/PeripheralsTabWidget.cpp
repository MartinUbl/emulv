//
// Created by Hynek on 06.04.2023.
//

#include <QScrollArea>
#include <QVBoxLayout>
#include "PeripheralsTabWidget.h"

PeripheralsTabWidget::PeripheralsTabWidget(QWidget *parent)
    : QWidget(parent)
    , tabWidget_(new QTabWidget(this)) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    this->setLayout(layout);
    layout->addWidget(tabWidget_);
}

void PeripheralsTabWidget::addPeripheralWidget(QWidget *peripheral_widget, std::string header, bool scrollable) {
    peripheral_widget->setParent(tabWidget_);

    if (scrollable) {
        QScrollArea *scroll_area = new QScrollArea(tabWidget_);
        scroll_area->setWidget(peripheral_widget);

        tabWidget_->addTab(scroll_area, QString::fromStdString(header));
    }
    else {
        tabWidget_->addTab(peripheral_widget, QString::fromStdString(header));
    }
}

void PeripheralsTabWidget::clear() {
    tabWidget_->clear();
}