import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: root
    readonly property string sendIconSVG: "qrc:///uart/resources/send_icon.svg";
    // POINTER TO C++ OBJECT CONTAINING SIGNALS (initialized on construction)
    property var uart
    Connections {
        target: uart
        // CONNECT TO C++ SIGNALS HERE
        function onDisplayTextUpdated(text) {centerTextArea.text = text}
    }

    anchors.fill: parent
    anchors.topMargin: 3
    anchors.leftMargin: 3
    anchors.rightMargin: 3
    clip: true

    GroupBox {
        anchors.fill: parent
        //title: "UART A"

        ColumnLayout {
            anchors.fill: parent

            // TOP BAR
            RowLayout {
                Layout.fillWidth: true
                RadioButton {
                    checked: true
                    text: "Display as ASCII"
                    Layout.preferredHeight: 22.5
                    onCheckedChanged: {
                        if(checked) {
                            root.uart.changeDisplayMode("ASCII");
                        }
                    }
                }

                Rectangle {
                    width: 1
                    color: palette.dark
                    Layout.preferredHeight: 22.5
                }

                RadioButton {
                    id: displayAsBytesRadio
                    text: "Display as bytes"
                    Layout.preferredHeight: 22.5
                    onCheckedChanged: {
                        if(checked) {
                            if (numModeCombobox.currentIndex == 0) {
                                root.uart.changeDisplayMode("HEX");
                            } else if (numModeCombobox.currentIndex == 1) {
                                root.uart.changeDisplayMode("DEC");
                            } else if (numModeCombobox.currentIndex == 2) {
                                root.uart.changeDisplayMode("BIN");
                            }
                        }
                    }
                }

                ComboBox {
                    id: numModeCombobox
                    enabled: displayAsBytesRadio.checked
                    model: ListModel {
                        ListElement { text: "Hex" }
                        ListElement { text: "Dec" }
                        ListElement { text: "Bin" }
                    }
                    onCurrentIndexChanged: {
                        if(enabled) {
                            if (currentIndex == 0) {
                                root.uart.changeDisplayMode("HEX");
                            } else if (currentIndex == 1) {
                                root.uart.changeDisplayMode("DEC");
                            } else if (currentIndex == 2) {
                                root.uart.changeDisplayMode("BIN");
                            }
                        }
                    }
                    Layout.preferredHeight: 22.5
                }

                Rectangle {
                    width: 1
                    color: palette.dark
                    Layout.preferredHeight: 22.5
                }

                CheckBox {
                    id: textWrapCheckbox
                    text: "Enable text wrapping"
                    Layout.preferredHeight: 22.5
                }

                Rectangle {
                    width: 1
                    color: palette.dark
                    Layout.preferredHeight: 22.5
                }

                Button {
                    Layout.preferredHeight: 22.5
                    text: "Clear all text"
                    onClicked: root.uart.clearDisplayText()
                }
            }

            // CENTER CONTENT
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Flickable {
                    anchors.fill: parent
                    TextArea.flickable: TextArea {
                        anchors.fill: parent
                        id: centerTextArea
                        readOnly: true
                        background: Rectangle {
                            anchors.fill: parent
                            color: palette.base
                            border.width: 1
                            border.color: palette.dark
                        }

                        wrapMode: (textWrapCheckbox.checked) ? TextEdit.WordWrap : TextEdit.NoWrap

                    }
                    boundsBehavior: Flickable.StopAtBounds
                    boundsMovement: Flickable.StopAtBounds
                    ScrollBar.vertical: ScrollBar{}
                    ScrollBar.horizontal: ScrollBar{}
                }

            }

            // BOTTOM BAR
            RowLayout {
                Layout.fillWidth: true

                TextField {
                    Layout.fillWidth: true

                    id: inputField
                    placeholderText: "Enter UART message"
                    onAccepted: {
                        sendButton.clicked();
                    }
                }

                ComboBox {
                    Layout.preferredHeight: inputField.height
                    model: ListModel {
                        ListElement { text: "No line break" }
                        ListElement { text: "LF" }
                        ListElement { text: "CRLF" }
                    }
                    onCurrentIndexChanged: {
                        if (currentIndex == 0) {
                            root.uart.changeLineBreak("NONE");
                        } else if (currentIndex == 1) {
                            root.uart.changeLineBreak("LF");
                        } else if (currentIndex == 2) {
                            root.uart.changeLineBreak("CRLF");
                        }
                    }
                }

                Button {
                    id: sendButton
                    icon.source: sendIconSVG
                    icon.color: palette.text
                    Layout.preferredHeight: inputField.height
                    Layout.preferredWidth: 30
                    onClicked: {
                        // Do nothing if nothing is entered
                        if(inputField.text === "")
                            return;

                        root.uart.transmitToDevice(inputField.text)
                        inputField.text = ""
                    }
                }

            }

        }
    }

}
