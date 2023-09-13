#pragma once

#include <QMainWindow>
#include <QToolButton>
#include <QFrame>
#include <QLabel>
#include <QSplitter>
#include <thread>

#include "EmulvApi.h"
#include "disassembly/DisassemblyWidget.h"
#include "registers/RegistersWidget.h"
#include "memory/MemoryWidget.h"
#include "peripherals/PeripheralsTabWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    QToolButton *btn_terminate_{}, *btn_continue_{}, *btn_run_{}, *btn_debug_{}, *btn_step_{};
    QFrame *running_indicator_{}, *debug_indicator_{};
    QSplitter *main_splitter_{}, *top_splitter_{}, *bot_splitter_{};
    QLabel *lbl_file_{}, *lbl_config_{}, *lbl_program_status_{};

    DisassemblyWidget *disassembly_widget_{};
    RegistersWidget *registers_widget_{};
    MemoryWidget *memory_widget_{};
    PeripheralsTabWidget *peripherals_tab_widget_{};

    explicit MainWindow(QWidget *parent = nullptr, EmulvApi *controller = nullptr);
    ~MainWindow() override;

    void OpenFile(const std::string& path);
    void SelectConfig(const std::string& path);
    void ClearConfig();

private slots:
    static void OnAboutTriggered();

    void UpdateUI();
    void OnFileOpenTriggered();
    void OnSelectConfigurationTriggered();
    void OnClearConfigurationTriggered();
    void OnRunClicked();
    void OnDebugClicked();
    void OnStepClicked();
    void OnTerminateClicked();
    void OnContinueClicked();

private:
    static const size_t kAddressWidth = 8;

    const QString kDefaultFileLabel = "No file loaded";
    const QString kDefaultConfigLabel = "No config loaded";

    EmulvApi *emulvApi_;
    std::unique_ptr<std::thread> thread_;

    static void ShowMessageBox(const QString& title, const QString& message);
    static std::string FormatAddress(uint64_t address);

    void SetupUI();
    void JoinThread();

    void UpdateRunningIndicator();
    void UpdateWidgetsEnabled();
    void UpdateToolbarButtons();
    void UpdateProgramStatusLabel();
    void UpdateRegisters();
    void UpdateMemory() const;
};
