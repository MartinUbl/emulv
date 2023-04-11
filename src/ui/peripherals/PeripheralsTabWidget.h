//
// Created by Hynek on 06.04.2023.
//

#ifndef EMULV_PERIPHERALSTABWIDGET_H
#define EMULV_PERIPHERALSTABWIDGET_H

#include <QWidget>
#include <QTabWidget>

class PeripheralsTabWidget : public QWidget {
    Q_OBJECT
public:
    PeripheralsTabWidget(QWidget *parent = nullptr);
    void addPeripheralWidget(QWidget *peripheral_widget, std::string header, bool scrollable = false);
    void clear();

private:
    QTabWidget *tabWidget_;
};

#endif //EMULV_PERIPHERALSTABWIDGET_H
