//
// Created by Hynek on 25.04.2023.
//

#pragma once

#include <QFrame>
#include <QRadioButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QToolButton>

#include "../Controller.h"

class MemoryWidget : public QFrame {
    Q_OBJECT
public:
    QRadioButton *rb_hex_, *rb_dec_;
    QSpinBox *spinbox_memory_from_, *spinbox_memory_to_;
    QToolButton *btn_search_, *btn_restore_;
    QTextEdit *text_edit_header_;
    QPlainTextEdit *text_edit_memory_;

    explicit MemoryWidget(QWidget *parent = nullptr, Controller *controller = nullptr);

    void SetAddressRangeLimit(int min, int max);
    void UpdateMemory();
    void Clear();

private slots:
    void OnRBHexClicked();
    void OnRBDecClicked();
    void OnSpinBoxMemoryFromChanged();
    void OnSpinBoxMemoryToChanged();
    void OnSearchClicked();
    void OnRestoreClicked();
    void OnHeaderHScrollChanged();
    void OnMemoryHScrollChanged();

private:
    static const int kMaxAddress = 0x7FFFFFFF;

    Controller *controller_;

    int memory_from_, memory_to_;
    std::vector<uint8_t> memory_;

    // Refreshes the memory using the internally saved memory
    void RefreshMemory();
    void UpdateSpinBoxes() const;
    void UpdateButtons() const;
    void UpdateScroll(int value) const;
};
