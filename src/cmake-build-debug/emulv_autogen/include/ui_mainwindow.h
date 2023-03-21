/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Open;
    QAction *action_About_RISCVEmulator;
    QAction *action_Serial_monitor;
    QAction *action_GPIO;
    QAction *action_Output;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_13;
    QWidget *toolBar;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_4;
    QToolButton *btnRun;
    QToolButton *btnDebug;
    QToolButton *btnTerminate;
    QToolButton *btnStep;
    QSpacerItem *horizontalSpacer_3;
    QFrame *debugIndicator;
    QVBoxLayout *verticalLayout_14;
    QFrame *line;
    QFrame *runningIndicator;
    QVBoxLayout *verticalLayout;
    QSplitter *splitterMain;
    QSplitter *splitterTop;
    QWidget *disassemblyWidget;
    QVBoxLayout *verticalLayout_10;
    QVBoxLayout *disassemblyLayout;
    QLabel *label;
    QWidget *registersWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;
    QRadioButton *rbRegistersDec;
    QRadioButton *rbRegistersHex;
    QListView *listViewRegisters;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_12;
    QSplitter *splitterBottom;
    QWidget *memoryWidget;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer;
    QRadioButton *rbMemoryDec;
    QRadioButton *rbMemoryHex;
    QSpinBox *spinBoxMemoryFrom;
    QLabel *label_4;
    QSpinBox *spinBoxMemoryTo;
    QToolButton *btnSelectMemory;
    QToolButton *btnRestoreMemory;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_8;
    QLineEdit *lineEditMemoryHeader;
    QPlainTextEdit *textEditMemory;
    QWidget *peripheralWidget;
    QVBoxLayout *verticalLayout_11;
    QTabWidget *tabWidget;
    QWidget *tabUart;
    QVBoxLayout *verticalLayout_3;
    QPlainTextEdit *plainTextEdit_2;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *lineEditSendMessage;
    QPushButton *btnSendMessage;
    QWidget *tabGPIO;
    QWidget *tabOutput;
    QVBoxLayout *verticalLayout_6;
    QPlainTextEdit *plainTextEdit_4;
    QMenuBar *menubar;
    QMenu *menu_File;
    QMenu *menu_Tools;
    QMenu *menu_Help;
    QMenu *menuView;
    QMenu *menu_Edit;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1051, 674);
        action_Open = new QAction(MainWindow);
        action_Open->setObjectName("action_Open");
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("document-open");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        action_Open->setIcon(icon);
        action_About_RISCVEmulator = new QAction(MainWindow);
        action_About_RISCVEmulator->setObjectName("action_About_RISCVEmulator");
        QIcon icon1;
        iconThemeName = QString::fromUtf8("help-about");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon1 = QIcon::fromTheme(iconThemeName);
        } else {
            icon1.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        action_About_RISCVEmulator->setIcon(icon1);
        action_Serial_monitor = new QAction(MainWindow);
        action_Serial_monitor->setObjectName("action_Serial_monitor");
        action_Serial_monitor->setCheckable(true);
        action_Serial_monitor->setChecked(true);
        action_GPIO = new QAction(MainWindow);
        action_GPIO->setObjectName("action_GPIO");
        action_GPIO->setCheckable(true);
        action_GPIO->setChecked(true);
        action_Output = new QAction(MainWindow);
        action_Output->setObjectName("action_Output");
        action_Output->setCheckable(true);
        action_Output->setChecked(true);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8(".QFrame {\n"
"	border-color: rgb(255, 128, 0);\n"
"}"));
        verticalLayout_13 = new QVBoxLayout(centralwidget);
        verticalLayout_13->setSpacing(0);
        verticalLayout_13->setObjectName("verticalLayout_13");
        verticalLayout_13->setContentsMargins(0, 2, 0, 2);
        toolBar = new QWidget(centralwidget);
        toolBar->setObjectName("toolBar");
        toolBar->setStyleSheet(QString::fromUtf8("QWidget#toolBar {\n"
"	background-color: rgba(99, 99, 99, 25);\n"
"}\n"
"\n"
"QToolButton {\n"
"	padding: 1px 0px 0px 0px;\n"
"}"));
        verticalLayout_9 = new QVBoxLayout(toolBar);
        verticalLayout_9->setSpacing(0);
        verticalLayout_9->setObjectName("verticalLayout_9");
        verticalLayout_9->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(3);
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        horizontalLayout_6->setContentsMargins(0, -1, -1, -1);
        horizontalSpacer_4 = new QSpacerItem(6, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_4);

        btnRun = new QToolButton(toolBar);
        btnRun->setObjectName("btnRun");
        btnRun->setStyleSheet(QString::fromUtf8("#btnRun {\n"
"	color: green;\n"
"}"));
        QIcon icon2;
        iconThemeName = QString::fromUtf8("media-playback-start");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon2 = QIcon::fromTheme(iconThemeName);
        } else {
            icon2.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        btnRun->setIcon(icon2);
        btnRun->setIconSize(QSize(14, 14));

        horizontalLayout_6->addWidget(btnRun);

        btnDebug = new QToolButton(toolBar);
        btnDebug->setObjectName("btnDebug");
        btnDebug->setStyleSheet(QString::fromUtf8(""));
        QIcon icon3;
        iconThemeName = QString::fromUtf8("media-seek-forward");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon3 = QIcon::fromTheme(iconThemeName);
        } else {
            icon3.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        btnDebug->setIcon(icon3);
        btnDebug->setIconSize(QSize(14, 14));

        horizontalLayout_6->addWidget(btnDebug);

        btnTerminate = new QToolButton(toolBar);
        btnTerminate->setObjectName("btnTerminate");
        btnTerminate->setStyleSheet(QString::fromUtf8("#btnTerminate {\n"
"	color: red;\n"
"}"));
        QIcon icon4;
        iconThemeName = QString::fromUtf8("media-playback-stop");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon4 = QIcon::fromTheme(iconThemeName);
        } else {
            icon4.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        btnTerminate->setIcon(icon4);
        btnTerminate->setIconSize(QSize(14, 14));

        horizontalLayout_6->addWidget(btnTerminate);

        btnStep = new QToolButton(toolBar);
        btnStep->setObjectName("btnStep");
        btnStep->setStyleSheet(QString::fromUtf8(""));
        QIcon icon5;
        iconThemeName = QString::fromUtf8("go-down");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon5 = QIcon::fromTheme(iconThemeName);
        } else {
            icon5.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        btnStep->setIcon(icon5);
        btnStep->setIconSize(QSize(14, 14));

        horizontalLayout_6->addWidget(btnStep);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);


        verticalLayout_9->addLayout(horizontalLayout_6);


        verticalLayout_13->addWidget(toolBar);

        debugIndicator = new QFrame(centralwidget);
        debugIndicator->setObjectName("debugIndicator");
        debugIndicator->setMaximumSize(QSize(16777215, 3));
        debugIndicator->setStyleSheet(QString::fromUtf8("#debugIndicator {\n"
"	border: 3px solid rgb(255, 128, 0);\n"
"}"));
        debugIndicator->setFrameShape(QFrame::Box);
        debugIndicator->setFrameShadow(QFrame::Plain);
        debugIndicator->setLineWidth(0);
        verticalLayout_14 = new QVBoxLayout(debugIndicator);
        verticalLayout_14->setSpacing(0);
        verticalLayout_14->setObjectName("verticalLayout_14");
        verticalLayout_14->setContentsMargins(0, 0, 0, 0);

        verticalLayout_13->addWidget(debugIndicator);

        line = new QFrame(centralwidget);
        line->setObjectName("line");
        line->setFrameShadow(QFrame::Plain);
        line->setFrameShape(QFrame::HLine);

        verticalLayout_13->addWidget(line);

        runningIndicator = new QFrame(centralwidget);
        runningIndicator->setObjectName("runningIndicator");
        runningIndicator->setEnabled(true);
        runningIndicator->setMinimumSize(QSize(0, 0));
        runningIndicator->setMaximumSize(QSize(16777215, 3));
        runningIndicator->setStyleSheet(QString::fromUtf8("#runningIndicator {\n"
"	border: 3px solid green;\n"
"}"));
        runningIndicator->setFrameShape(QFrame::Box);
        runningIndicator->setFrameShadow(QFrame::Plain);
        runningIndicator->setLineWidth(0);
        verticalLayout = new QVBoxLayout(runningIndicator);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);

        verticalLayout_13->addWidget(runningIndicator);

        splitterMain = new QSplitter(centralwidget);
        splitterMain->setObjectName("splitterMain");
        splitterMain->setLineWidth(1);
        splitterMain->setOrientation(Qt::Vertical);
        splitterMain->setChildrenCollapsible(false);
        splitterTop = new QSplitter(splitterMain);
        splitterTop->setObjectName("splitterTop");
        splitterTop->setOrientation(Qt::Horizontal);
        splitterTop->setChildrenCollapsible(false);
        disassemblyWidget = new QWidget(splitterTop);
        disassemblyWidget->setObjectName("disassemblyWidget");
        verticalLayout_10 = new QVBoxLayout(disassemblyWidget);
        verticalLayout_10->setSpacing(0);
        verticalLayout_10->setObjectName("verticalLayout_10");
        verticalLayout_10->setContentsMargins(0, 0, 0, 0);
        disassemblyLayout = new QVBoxLayout();
        disassemblyLayout->setObjectName("disassemblyLayout");
        disassemblyLayout->setContentsMargins(6, -1, -1, -1);
        label = new QLabel(disassemblyWidget);
        label->setObjectName("label");

        disassemblyLayout->addWidget(label);


        verticalLayout_10->addLayout(disassemblyLayout);

        splitterTop->addWidget(disassemblyWidget);
        registersWidget = new QWidget(splitterTop);
        registersWidget->setObjectName("registersWidget");
        verticalLayout_2 = new QVBoxLayout(registersWidget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 6, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_2 = new QLabel(registersWidget);
        label_2->setObjectName("label_2");

        horizontalLayout_3->addWidget(label_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        rbRegistersDec = new QRadioButton(registersWidget);
        rbRegistersDec->setObjectName("rbRegistersDec");
        rbRegistersDec->setChecked(false);

        horizontalLayout_3->addWidget(rbRegistersDec);

        rbRegistersHex = new QRadioButton(registersWidget);
        rbRegistersHex->setObjectName("rbRegistersHex");
        rbRegistersHex->setChecked(true);

        horizontalLayout_3->addWidget(rbRegistersHex);


        verticalLayout_2->addLayout(horizontalLayout_3);

        listViewRegisters = new QListView(registersWidget);
        listViewRegisters->setObjectName("listViewRegisters");
        QFont font;
        font.setFamilies({QString::fromUtf8("NotoSansMono Nerd Font Mono")});
        font.setBold(true);
        listViewRegisters->setFont(font);

        verticalLayout_2->addWidget(listViewRegisters);

        splitterTop->addWidget(registersWidget);
        splitterMain->addWidget(splitterTop);
        groupBox = new QGroupBox(splitterMain);
        groupBox->setObjectName("groupBox");
        groupBox->setStyleSheet(QString::fromUtf8("#groupBox { border: 0px; }"));
        groupBox->setFlat(true);
        verticalLayout_12 = new QVBoxLayout(groupBox);
        verticalLayout_12->setSpacing(0);
        verticalLayout_12->setObjectName("verticalLayout_12");
        verticalLayout_12->setContentsMargins(3, 0, 6, 0);
        splitterBottom = new QSplitter(groupBox);
        splitterBottom->setObjectName("splitterBottom");
        splitterBottom->setOrientation(Qt::Horizontal);
        splitterBottom->setChildrenCollapsible(false);
        memoryWidget = new QWidget(splitterBottom);
        memoryWidget->setObjectName("memoryWidget");
        verticalLayout_5 = new QVBoxLayout(memoryWidget);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setObjectName("verticalLayout_5");
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label_3 = new QLabel(memoryWidget);
        label_3->setObjectName("label_3");
        label_3->setMargin(0);

        horizontalLayout->addWidget(label_3);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        rbMemoryDec = new QRadioButton(memoryWidget);
        rbMemoryDec->setObjectName("rbMemoryDec");

        horizontalLayout->addWidget(rbMemoryDec);

        rbMemoryHex = new QRadioButton(memoryWidget);
        rbMemoryHex->setObjectName("rbMemoryHex");
        rbMemoryHex->setChecked(true);

        horizontalLayout->addWidget(rbMemoryHex);

        spinBoxMemoryFrom = new QSpinBox(memoryWidget);
        spinBoxMemoryFrom->setObjectName("spinBoxMemoryFrom");
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(spinBoxMemoryFrom->sizePolicy().hasHeightForWidth());
        spinBoxMemoryFrom->setSizePolicy(sizePolicy);
        spinBoxMemoryFrom->setMinimumSize(QSize(80, 0));
        spinBoxMemoryFrom->setMaximum(4094);
        spinBoxMemoryFrom->setValue(0);
        spinBoxMemoryFrom->setDisplayIntegerBase(16);

        horizontalLayout->addWidget(spinBoxMemoryFrom);

        label_4 = new QLabel(memoryWidget);
        label_4->setObjectName("label_4");
        label_4->setMargin(4);

        horizontalLayout->addWidget(label_4);

        spinBoxMemoryTo = new QSpinBox(memoryWidget);
        spinBoxMemoryTo->setObjectName("spinBoxMemoryTo");
        spinBoxMemoryTo->setMinimumSize(QSize(80, 0));
        spinBoxMemoryTo->setMaximum(4095);
        spinBoxMemoryTo->setValue(4095);
        spinBoxMemoryTo->setDisplayIntegerBase(16);

        horizontalLayout->addWidget(spinBoxMemoryTo);

        btnSelectMemory = new QToolButton(memoryWidget);
        btnSelectMemory->setObjectName("btnSelectMemory");
        QIcon icon6;
        iconThemeName = QString::fromUtf8("edit-find");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon6 = QIcon::fromTheme(iconThemeName);
        } else {
            icon6.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        btnSelectMemory->setIcon(icon6);

        horizontalLayout->addWidget(btnSelectMemory);

        btnRestoreMemory = new QToolButton(memoryWidget);
        btnRestoreMemory->setObjectName("btnRestoreMemory");
        QIcon icon7;
        iconThemeName = QString::fromUtf8("edit-undo");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon7 = QIcon::fromTheme(iconThemeName);
        } else {
            icon7.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        btnRestoreMemory->setIcon(icon7);

        horizontalLayout->addWidget(btnRestoreMemory);


        verticalLayout_4->addLayout(horizontalLayout);

        scrollArea = new QScrollArea(memoryWidget);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 719, 278));
        verticalLayout_7 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setObjectName("verticalLayout_7");
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(0);
        verticalLayout_8->setObjectName("verticalLayout_8");
        lineEditMemoryHeader = new QLineEdit(scrollAreaWidgetContents);
        lineEditMemoryHeader->setObjectName("lineEditMemoryHeader");
        lineEditMemoryHeader->setSizeIncrement(QSize(0, 0));
        lineEditMemoryHeader->setFont(font);
        lineEditMemoryHeader->setCursor(QCursor(Qt::ArrowCursor));
        lineEditMemoryHeader->setStyleSheet(QString::fromUtf8("padding-left: 2px;\n"
"selection-background-color: rgba(255, 255, 255, 0);\n"
"padding-bottom: -5px;"));
        lineEditMemoryHeader->setFrame(false);
        lineEditMemoryHeader->setDragEnabled(true);
        lineEditMemoryHeader->setReadOnly(true);

        verticalLayout_8->addWidget(lineEditMemoryHeader);

        textEditMemory = new QPlainTextEdit(scrollAreaWidgetContents);
        textEditMemory->setObjectName("textEditMemory");
        textEditMemory->setMinimumSize(QSize(600, 0));
        textEditMemory->setFont(font);
        textEditMemory->setStyleSheet(QString::fromUtf8(""));
        textEditMemory->setFrameShape(QFrame::NoFrame);
        textEditMemory->setLineWidth(0);
        textEditMemory->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        textEditMemory->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textEditMemory->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        textEditMemory->setLineWrapMode(QPlainTextEdit::NoWrap);
        textEditMemory->setReadOnly(true);

        verticalLayout_8->addWidget(textEditMemory);


        horizontalLayout_5->addLayout(verticalLayout_8);


        verticalLayout_7->addLayout(horizontalLayout_5);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_4->addWidget(scrollArea);


        verticalLayout_5->addLayout(verticalLayout_4);

        splitterBottom->addWidget(memoryWidget);
        peripheralWidget = new QWidget(splitterBottom);
        peripheralWidget->setObjectName("peripheralWidget");
        verticalLayout_11 = new QVBoxLayout(peripheralWidget);
        verticalLayout_11->setSpacing(0);
        verticalLayout_11->setObjectName("verticalLayout_11");
        verticalLayout_11->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(peripheralWidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setEnabled(true);
        tabWidget->setTabsClosable(false);
        tabWidget->setMovable(false);
        tabUart = new QWidget();
        tabUart->setObjectName("tabUart");
        verticalLayout_3 = new QVBoxLayout(tabUart);
        verticalLayout_3->setSpacing(3);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        plainTextEdit_2 = new QPlainTextEdit(tabUart);
        plainTextEdit_2->setObjectName("plainTextEdit_2");
        plainTextEdit_2->setFrameShape(QFrame::NoFrame);
        plainTextEdit_2->setFrameShadow(QFrame::Plain);
        plainTextEdit_2->setLineWidth(0);
        plainTextEdit_2->setLineWrapMode(QPlainTextEdit::NoWrap);

        verticalLayout_3->addWidget(plainTextEdit_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        lineEditSendMessage = new QLineEdit(tabUart);
        lineEditSendMessage->setObjectName("lineEditSendMessage");
        lineEditSendMessage->setClearButtonEnabled(true);

        horizontalLayout_4->addWidget(lineEditSendMessage);

        btnSendMessage = new QPushButton(tabUart);
        btnSendMessage->setObjectName("btnSendMessage");
        btnSendMessage->setEnabled(false);
        QIcon icon8;
        iconThemeName = QString::fromUtf8("go-next");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon8 = QIcon::fromTheme(iconThemeName);
        } else {
            icon8.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        btnSendMessage->setIcon(icon8);

        horizontalLayout_4->addWidget(btnSendMessage);


        verticalLayout_3->addLayout(horizontalLayout_4);

        tabWidget->addTab(tabUart, QString());
        tabGPIO = new QWidget();
        tabGPIO->setObjectName("tabGPIO");
        tabWidget->addTab(tabGPIO, QString());
        tabOutput = new QWidget();
        tabOutput->setObjectName("tabOutput");
        verticalLayout_6 = new QVBoxLayout(tabOutput);
        verticalLayout_6->setObjectName("verticalLayout_6");
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        plainTextEdit_4 = new QPlainTextEdit(tabOutput);
        plainTextEdit_4->setObjectName("plainTextEdit_4");
        plainTextEdit_4->setFrameShape(QFrame::NoFrame);
        plainTextEdit_4->setFrameShadow(QFrame::Plain);

        verticalLayout_6->addWidget(plainTextEdit_4);

        tabWidget->addTab(tabOutput, QString());

        verticalLayout_11->addWidget(tabWidget);

        splitterBottom->addWidget(peripheralWidget);

        verticalLayout_12->addWidget(splitterBottom);

        splitterMain->addWidget(groupBox);

        verticalLayout_13->addWidget(splitterMain);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1051, 22));
        menu_File = new QMenu(menubar);
        menu_File->setObjectName("menu_File");
        menu_Tools = new QMenu(menubar);
        menu_Tools->setObjectName("menu_Tools");
        menu_Help = new QMenu(menubar);
        menu_Help->setObjectName("menu_Help");
        menuView = new QMenu(menubar);
        menuView->setObjectName("menuView");
        menu_Edit = new QMenu(menubar);
        menu_Edit->setObjectName("menu_Edit");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        QWidget::setTabOrder(listViewRegisters, rbRegistersDec);
        QWidget::setTabOrder(rbRegistersDec, rbRegistersHex);
        QWidget::setTabOrder(rbRegistersHex, spinBoxMemoryFrom);
        QWidget::setTabOrder(spinBoxMemoryFrom, spinBoxMemoryTo);
        QWidget::setTabOrder(spinBoxMemoryTo, btnSelectMemory);
        QWidget::setTabOrder(btnSelectMemory, btnRestoreMemory);
        QWidget::setTabOrder(btnRestoreMemory, tabWidget);
        QWidget::setTabOrder(tabWidget, plainTextEdit_2);
        QWidget::setTabOrder(plainTextEdit_2, lineEditSendMessage);
        QWidget::setTabOrder(lineEditSendMessage, btnSendMessage);
        QWidget::setTabOrder(btnSendMessage, plainTextEdit_4);

        menubar->addAction(menu_File->menuAction());
        menubar->addAction(menu_Edit->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menu_Tools->menuAction());
        menubar->addAction(menu_Help->menuAction());
        menu_File->addAction(action_Open);
        menu_Help->addAction(action_About_RISCVEmulator);
        menuView->addAction(action_Serial_monitor);
        menuView->addAction(action_GPIO);
        menuView->addAction(action_Output);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "RISC-V Emulator", nullptr));
        action_Open->setText(QCoreApplication::translate("MainWindow", "&Open", nullptr));
#if QT_CONFIG(shortcut)
        action_Open->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        action_About_RISCVEmulator->setText(QCoreApplication::translate("MainWindow", "&About RISC-V Emulator", nullptr));
        action_Serial_monitor->setText(QCoreApplication::translate("MainWindow", "&Serial monitor", nullptr));
        action_GPIO->setText(QCoreApplication::translate("MainWindow", "&GPIO", nullptr));
        action_Output->setText(QCoreApplication::translate("MainWindow", "&Output", nullptr));
#if QT_CONFIG(tooltip)
        btnRun->setToolTip(QCoreApplication::translate("MainWindow", "Run (F5)", nullptr));
#endif // QT_CONFIG(tooltip)
        btnRun->setText(QCoreApplication::translate("MainWindow", "Run", nullptr));
#if QT_CONFIG(shortcut)
        btnRun->setShortcut(QCoreApplication::translate("MainWindow", "F5", nullptr));
#endif // QT_CONFIG(shortcut)
#if QT_CONFIG(tooltip)
        btnDebug->setToolTip(QCoreApplication::translate("MainWindow", "Debug (F6)", nullptr));
#endif // QT_CONFIG(tooltip)
        btnDebug->setText(QCoreApplication::translate("MainWindow", "Debug", nullptr));
#if QT_CONFIG(shortcut)
        btnDebug->setShortcut(QCoreApplication::translate("MainWindow", "F6", nullptr));
#endif // QT_CONFIG(shortcut)
#if QT_CONFIG(tooltip)
        btnTerminate->setToolTip(QCoreApplication::translate("MainWindow", "Terminate (Shift+F5)", nullptr));
#endif // QT_CONFIG(tooltip)
        btnTerminate->setText(QCoreApplication::translate("MainWindow", "Terminate", nullptr));
#if QT_CONFIG(shortcut)
        btnTerminate->setShortcut(QCoreApplication::translate("MainWindow", "Shift+F5", nullptr));
#endif // QT_CONFIG(shortcut)
#if QT_CONFIG(tooltip)
        btnStep->setToolTip(QCoreApplication::translate("MainWindow", "Step (F7)", nullptr));
#endif // QT_CONFIG(tooltip)
        btnStep->setText(QCoreApplication::translate("MainWindow", "Step", nullptr));
#if QT_CONFIG(shortcut)
        btnStep->setShortcut(QCoreApplication::translate("MainWindow", "F7", nullptr));
#endif // QT_CONFIG(shortcut)
        label->setText(QCoreApplication::translate("MainWindow", "Disassembly", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Registers", nullptr));
        rbRegistersDec->setText(QCoreApplication::translate("MainWindow", "Dec", nullptr));
        rbRegistersHex->setText(QCoreApplication::translate("MainWindow", "Hex", nullptr));
        groupBox->setTitle(QString());
        label_3->setText(QCoreApplication::translate("MainWindow", "Memory", nullptr));
        rbMemoryDec->setText(QCoreApplication::translate("MainWindow", "Dec", nullptr));
        rbMemoryHex->setText(QCoreApplication::translate("MainWindow", "Hex", nullptr));
        spinBoxMemoryFrom->setSuffix(QCoreApplication::translate("MainWindow", "0", nullptr));
        spinBoxMemoryFrom->setPrefix(QCoreApplication::translate("MainWindow", "0x", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        spinBoxMemoryTo->setSuffix(QCoreApplication::translate("MainWindow", "0", nullptr));
        spinBoxMemoryTo->setPrefix(QCoreApplication::translate("MainWindow", "0x", nullptr));
#if QT_CONFIG(tooltip)
        btnSelectMemory->setToolTip(QCoreApplication::translate("MainWindow", "Confirm range", nullptr));
#endif // QT_CONFIG(tooltip)
        btnSelectMemory->setText(QString());
#if QT_CONFIG(tooltip)
        btnRestoreMemory->setToolTip(QCoreApplication::translate("MainWindow", "Restore", nullptr));
#endif // QT_CONFIG(tooltip)
        btnRestoreMemory->setText(QString());
        lineEditMemoryHeader->setText(QCoreApplication::translate("MainWindow", "         00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F", nullptr));
        textEditMemory->setPlainText(QString());
        lineEditSendMessage->setPlaceholderText(QCoreApplication::translate("MainWindow", "Send message...", nullptr));
#if QT_CONFIG(tooltip)
        btnSendMessage->setToolTip(QCoreApplication::translate("MainWindow", "Send", nullptr));
#endif // QT_CONFIG(tooltip)
        btnSendMessage->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tabUart), QCoreApplication::translate("MainWindow", "UART", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabGPIO), QCoreApplication::translate("MainWindow", "GPIO", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabOutput), QCoreApplication::translate("MainWindow", "Output", nullptr));
        menu_File->setTitle(QCoreApplication::translate("MainWindow", "&File", nullptr));
        menu_Tools->setTitle(QCoreApplication::translate("MainWindow", "&Tools", nullptr));
        menu_Help->setTitle(QCoreApplication::translate("MainWindow", "&Help", nullptr));
        menuView->setTitle(QCoreApplication::translate("MainWindow", "&View", nullptr));
        menu_Edit->setTitle(QCoreApplication::translate("MainWindow", "&Edit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
