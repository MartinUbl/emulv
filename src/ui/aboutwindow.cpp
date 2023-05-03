#include <QVBoxLayout>
#include <QLabel>
#include <QTextBrowser>
#include "aboutwindow.h"

AboutWindow::AboutWindow(QWidget *parent) :
QDialog(parent) {
    setupUi();
}

void AboutWindow::setupUi() {
    setLayout(new QVBoxLayout(this));
    setWindowIcon(QIcon(":img/logo.ico"));

    auto lblTitle = new QLabel("RISV-V Emulator", this);
    lblTitle->setAlignment(Qt::AlignHCenter);
    auto font = lblTitle->font();
    font.setPointSize(12);
    font.setBold(true);
    lblTitle->setFont(font);

    auto lblVersion = new QLabel("1.0.0", this);
    lblVersion->setAlignment(Qt::AlignHCenter);

    auto textBrowser = new QTextBrowser(this);
    textBrowser->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    textBrowser->setFrameShape(QFrame::NoFrame);
    textBrowser->setFrameShadow(QFrame::Plain);
    textBrowser->setHtml("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                         "<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
                         "p, li { white-space: pre-wrap; }\n"
                         "hr { height: 1px; border-width: 0; }\n"
                         "li.unchecked::marker { content: \"\\2610\"; }\n"
                         "li.checked::marker { content: \"\\2612\"; }\n"
                         "</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
                         "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif';\">Report issues: </span><a href=\"https://gitlab.com/skjdhm/emulv/-/issues\"><span style=\" font-family:'Sans Serif'; text-decoration: underline; color:#007af4;\">GitLab</span></a></p>\n"
                         "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans Serif';\"><br /></p>\n"
                         "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif';\">Created by: </span></p>\n"
                         "<ul style=\"margin-top: 0px; margin-bottom: 0px; margin-left: 0px; margin-right: 0px; -qt-list-indent: 1;\">\n"
                         "<li style=\" font-family:'Sans Serif';\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Stanislav Kafara  </li>\n"
                         "<li style=\" font-family:'Sans Serif';\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Jonáš Dufek  </li>\n"
                         "<li style=\" font-family:'Sans Serif';\" style=\" margin-top:0px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Hynek Moudrý </li></ul>\n"
                         "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif';\">Under the supervision of: </span></p>\n"
                         "<ul style=\"margin-top: 0px; margin-bottom: 0px; margin-left: 0px; margin-right: 0px; -qt-list-indent: 1;\">\n"
                         "<li style=\" font-family:'Sans Serif';\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Ing. Martin Úbl  </li>\n"
                         "<li style=\" font-family:'Sans Serif';\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Ing. Jan Rychlík  </li>\n"
                         "<li style=\" font-family:'Sans Serif';\" style=\" margin-top:0px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Doc. Ing. Roman Mouček Ph.D. </li></ul>\n"
                         "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans Serif';\"><br /></p></body></html>");

    layout()->addWidget(lblTitle);
    layout()->addWidget(lblVersion);
    layout()->addWidget(textBrowser);
}
