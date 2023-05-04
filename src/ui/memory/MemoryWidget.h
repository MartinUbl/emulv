//
// Created by Hynek on 25.04.2023.
//

#ifndef EMULV_MEMORYWIDGET_H
#define EMULV_MEMORYWIDGET_H

#include <QFrame>
#include <QRadioButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QToolButton>
#include <QScrollArea>
#include "../Controller.h"

class MemoryWidget : public QFrame {
    Q_OBJECT
public:
    QRadioButton *rb_hex_, *rb_dec_;
    QSpinBox *sp_memory_from_, *sp_memory_to_;
    QToolButton *btn_search_, *btn_restore_;
    QTextEdit *te_header_;
    QPlainTextEdit *te_memory_;

    MemoryWidget(QWidget *parent = nullptr, Controller *controller = nullptr);

    void setAddressRangeLimit(int min, int max);
    void updateMemory();
    void clear();

private slots:
    void rb_hex_clicked_();
    void rb_dec_clicked_();
    void sp_memory_from_changed_();
    void sp_memory_to_changed_();
    void btn_search_clicked_();
    void btn_restore_clicked_();
    void memory_scroll_changed();
    void header_scroll_changed();

private:
    static constexpr int kMaxAddress = 0x7FFFFFFF;
    Controller *controller_;

    int memory_from_, memory_to_;
    std::vector<uint8_t> memory_;

    void updateMemory_();
    void updateMemorySpinBoxes_();
    void updateMemoryButtons_();
    void updateScroll_(int value);
};

#endif //EMULV_MEMORYWIDGET_H
