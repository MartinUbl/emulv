import QtQuick
import Borderless_lib_test
import QtQuick.Controls
import QtQuick.Layouts

import "../controls"

Rectangle {
    anchors.fill: parent
    color: Colors.primaryPanel

    // Contains top row with controls
    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 3
        anchors.leftMargin: 3
        clip: true
        color: Colors.secondaryPanel
        height: topRow.implicitHeight

        id: topRect
        Row {
            anchors.fill: parent
            spacing: 5
            id: topRow

            RadioButton {
                checked: true
                text: "Hex"
                onCheckedChanged: {
                    if(checked) {
                        RegistersTableModel.switchToHex();
                    }
                }
            }
            RadioButton {
                text: "Dec"
                onCheckedChanged: {
                    if(checked) {
                        RegistersTableModel.switchToDec();
                    }
                }
            }
            RadioButton {
                text: "Bin"
                onCheckedChanged: {
                    if(checked) {
                        RegistersTableModel.switchToBin();
                    }
                }
            }
        }

    }

    // Contains table
    Item {
        anchors.top:  topRect.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        anchors.rightMargin: 3
        anchors.leftMargin: 3

        TableView {
            id: tableView
            anchors.fill: parent
            clip: true
            topMargin: horizontalHeader.implicitHeight


            boundsBehavior: Flickable.StopAtBounds
            boundsMovement: Flickable.StopAtBounds

            model: RegistersTableModel

            delegate: Rectangle {
                color: Colors.primaryPanel
                implicitWidth: tableView.width/3
                implicitHeight: itemText.height + 4

                // border.color: Colors.primaryText
                // border.width: 1

                TextEdit {
                    anchors.fill: parent
                    anchors.leftMargin: 5
                    id: itemText
                    font.family: monospaceFont.family
                    font.bold: true
                    text: display
                    color: Colors.primaryText
                    verticalAlignment: Qt.AlignVCenter
                    //horizontalAlignment: Qt.AlignHCenter
                    readOnly: true
                    selectByMouse: true
                }
            }

            // Custom scroll indicator
            ScrollIndicator.vertical: ScrollIndicator {
                active: true
                implicitWidth: 15

                contentItem: Rectangle {
                    implicitWidth: 6
                    implicitHeight: 6
                    color: Colors.primaryText
                    opacity: tableView.movingVertically ? 0.5 : 0.0
                    Behavior on opacity {
                        OpacityAnimator {
                            duration: 500
                        }
                    }
                }
            }
        }

        HorizontalHeaderView {
            id: horizontalHeader
            syncView: tableView
            anchors.left: tableView.left
            clip: true
            interactive: false

            delegate: Rectangle {
                color: Colors.secondaryPanel
                implicitWidth: tableView.width/3
                implicitHeight: 30

                // gradient: Gradient {
                //     GradientStop { position: 0.9; color: Colors.secondaryPanel }
                //     GradientStop { position: 1; color: Colors.primaryPanel}
                // }

                Label {
                    anchors.fill: parent
                    text: display
                    verticalAlignment: Qt.AlignVCenter
                    //horizontalAlignment: Qt.AlignHCenter
                    padding: 5
                }
            }
        }
    }
}
