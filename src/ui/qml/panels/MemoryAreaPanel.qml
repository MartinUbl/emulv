import QtQuick
import EmulvQt
import QtQuick.Controls
import QtQuick.Layouts

import "../controls"

Rectangle {
    anchors.fill: parent
    color: Colors.primaryPanel
    property bool topHeaderVisible: false

    property var memoryModel: UiController.memoryTableModel

    // Top row
    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 3
        clip: true
        color: Colors.secondaryPanel
        height: 40
        id: topRect

        RowLayout {
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            spacing: 10
            id: topRowLeft

            Label {
                text: "Address:"
                verticalAlignment: Qt.AlignVCenter
                height: parent.height
            }

            TextField {
                id: addressField
                placeholderText: "Hex format"
                validator: RegularExpressionValidator {
                    regularExpression: /[0-9A-Fa-f]+$/
                }
                onAccepted: {
                    viewButton.clicked();
                }
            }

            ActionButton {
                id: viewButton
                source: "qrc:///assets/search_icon.svg"
                text: "View"
                height: parent.height
                onClicked: {
                    var addr = parseInt(addressField.text, 16);
                    if(addr === NaN)
                        return;
                    var rowIndex = memoryModel.getRowFromAddress(addr);
                    tableView.positionViewAtRow(rowIndex, TableView.AlignTop)
                }
            }
            IconButton {
                source: "qrc:///assets/refresh_icon.svg"
                iconColor: "#53C2F8" // Fixed color
                onClicked: {
                    memoryModel.refreshView()
                }
            }
        }

        RowLayout {
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            spacing: 10
            visible: !(topRowLeft.width + width + 5 > parent.width)

            Label {
                text: "Columns:"
                verticalAlignment: Qt.AlignVCenter
                height: parent.height
            }

            ComboBox {
                id: columnsComboBox
                implicitContentWidthPolicy: ComboBox.WidestText
                model: ["8", "16", "32"]
                onCurrentIndexChanged: {
                    if(currentIndex === 0) {
                        memoryModel.setMemoryCellColumns(8);
                    } else if(currentIndex === 1) {
                        memoryModel.setMemoryCellColumns(16);
                    } else if(currentIndex === 2) {
                        memoryModel.setMemoryCellColumns(32);
                    }
                    memoryModel.refreshView()
                }
            }

            Label {
                text: "Mode:"
                verticalAlignment: Qt.AlignVCenter
                height: parent.height
            }

            ComboBox {
                id: modeComboBox
                implicitContentWidthPolicy: ComboBox.WidestText
                model: ["Hex", "Dec", "Bin"]
                onCurrentIndexChanged: {
                    if(currentIndex === 0) {
                        memoryModel.switchToHex();
                    } else if(currentIndex === 1) {
                        memoryModel.switchToDec();
                    } else if(currentIndex === 2) {
                        memoryModel.switchToBin();
                    }

                }
            }
        }
    }

    // Background for the horizontal table header
    Rectangle {
        anchors.top: topRect.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 3
        visible: topHeaderVisible

        width: parent.width
        height: horizontalHeader.implicitHeight
        color: Colors.secondaryPanel
    }

    // Table
    Item  {
        anchors.top:  topRect.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        anchors.rightMargin: 3

        TableView {
            id: tableView
            anchors.left: verticalHeader.right
            anchors.right: parent.right
            anchors.top: (topHeaderVisible) ? horizontalHeader.bottom : parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 10
            clip: true
            columnSpacing: 10

            boundsBehavior: Flickable.StopAtBounds
            boundsMovement: Flickable.StopAtBounds

            model: memoryModel

            // The table cell:
            delegate: Rectangle {
                color: Colors.primaryPanel
                implicitWidth: itemText.width
                implicitHeight: itemText.height + 4

                TextEdit {
                    id: itemText
                    font.family: monospaceFont.family
                    font.bold: true
                    text: display
                    color: Colors.primaryText
                    verticalAlignment: Qt.AlignVCenter
                    readOnly: true
                    selectByMouse: true
                }
            }

            // Vetical scrolling:
            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded

                implicitWidth: 15
                implicitHeight: 20

                contentItem: Rectangle {
                    implicitWidth: 6
                    implicitHeight: 20
                    color: Colors.primaryText
                    opacity: (tableView.movingVertically || parent.active) ? 0.5 : 0.0
                    Behavior on opacity {
                        OpacityAnimator {
                            duration: 500
                        }
                    }
                }
            }

            // Horizontal scrolling:
            ScrollBar.horizontal: ScrollBar {
                policy: ScrollBar.AsNeeded
                visible: parent.contentWidth > parent.width
                contentItem: Rectangle {
                    implicitHeight: 7.5
                    color: Colors.primaryText
                    opacity: parent.active ? 0.5 : 0.0
                    Behavior on opacity {
                        OpacityAnimator {
                            duration: 500
                        }
                    }
                }

            }
        }

        VerticalHeaderView {
            id: verticalHeader
            syncView: tableView
            anchors.top: tableView.top
            anchors.left: parent.left
            interactive: false
            resizableRows: false
            clip: true
            delegate: Rectangle {
                color: Colors.secondaryPanel
                implicitWidth: headerLabel.implicitWidth
                implicitHeight: 30

                Label {
                    id: headerLabel
                    anchors.fill: parent
                    text: display
                    leftPadding: 5
                    rightPadding: 5
                    font.family: monospaceFont.family
                    font.bold: true
                }
            }
        }

        HorizontalHeaderView {
            id: horizontalHeader
            syncView: tableView
            anchors.left: tableView.left
            clip: true
            interactive: false
            visible: topHeaderVisible
            resizableColumns: false

            delegate: Rectangle {
                color: Colors.secondaryPanel
                implicitWidth: tableView.width/tableView.columns
                implicitHeight: offsetLabel.implicitHeight + 7.5
                Label {
                    id: offsetLabel
                    anchors.fill: parent
                    text: display
                    font.family: monospaceFont.family
                    font.bold: true

                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                }
            }
        }


        TapHandler {
            acceptedButtons: Qt.RightButton
            onSingleTapped: (eventPoint, button) => {
                                switch (button) {
                                    case Qt.RightButton:
                                    contextMenu.popup()
                                    break;
                                }
                            }
        }

        // Menu so horizontal header can be enabled
        Menu {
            id: contextMenu
            MenuItem {
                text: (topHeaderVisible) ? "Disable address offsets" : "Show address offsets"
                onTriggered: {
                    topHeaderVisible = !topHeaderVisible;
                }
            }
        }
    }
}
