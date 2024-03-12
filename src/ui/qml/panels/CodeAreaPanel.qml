import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import EmulvQt

Rectangle {
    id: root

    property var codeAreaModel: UiController.codeAreaModel

    readonly property int breakpointClickAreaWidth: 25

    // Set to true when error reading file happens, reset to false when TableView rows count is > 0
    property bool isErrored: false;

    // Background color
    color: Colors.primaryPanel

    Menu {
        id: contextMenu
        MenuItem {
            text: "Remove all breakpoints"
            onTriggered: {
                codeAreaModel.removeAllBreakpoints();
            }
        }
    }

    // Displays errors
    Item {
        visible: isErrored
        anchors.centerIn: parent
        width: parent.width/2
        height: parent.height/2
        Rectangle {
            color: Colors.secondaryPanel
            anchors.fill: parent
            border.color: Colors.borderColor
            border.width: 1
            ColumnLayout  {
                anchors.fill: parent
                Label {
                    Layout.fillWidth: true
                    font.bold: true
                    text: "Could not load file."
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                    wrapMode: "WordWrap"
                }
                Label {
                    Layout.fillWidth: true
                    id: errorLabel
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                    wrapMode: "WordWrap"
                }
            }
        }
    }

    // Contains table
    Item {
        anchors.fill: parent
        visible: !isErrored

        TableView {
            id: tableView
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: verticalHeader.implicitWidth
            clip: true

            boundsBehavior: Flickable.StopAtBounds
            boundsMovement: Flickable.StopAtBounds

            model: codeAreaModel

            // Background
            Rectangle {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                width: root.width
                color: Colors.primaryWindowBackgroud
            }

            delegate: Rectangle {
                id: containerRect

                color: (column == 0) ? Colors.primaryPanel : "transparent"
                implicitWidth: (column == 0) ? itemText.implicitWidth : itemText.implicitWidth;
                implicitHeight: itemText.height + 4

                TextEdit {
                    id: itemText
                    anchors.fill: parent
                    //anchors.leftMargin: 5
                    leftPadding: 5
                    rightPadding: 5

                    font.family: monospaceFont.family
                    font.bold: true
                    text: display
                    color: Colors.primaryText
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignLeft
                    readOnly: true
                    selectByMouse: true
                }

                Rectangle {
                    visible: column == 0
                    width: 1
                    height: parent.height
                    color: Qt.darker(Colors.primaryText, 3)
                    anchors.right: parent.right
                }
            }

            // Reset the errored variable, makes tableview visible
            onRowsChanged: {
                if(tableView.rows > 0) {
                    isErrored = false;
                }
            }

            // Custom scroll indicators
            ScrollBar.vertical: TextAreaScrollBar{}
            ScrollBar.horizontal: TextAreaScrollBar{}

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
                color: Colors.primaryPanel
                implicitWidth: breakpointClickAreaWidth
                implicitHeight: 50
                Breakpoint {
                    visible: display
                    id: bp
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        print("clicked breakpoint " + row )
                        codeAreaModel.toggleBreakpoint(row);
                    }
                }
            }
            // Remove breakpoint right click context menu
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
        }

    }




    //##############################################################
    //# Custom scrollbar
    //##############################################################
    component TextAreaScrollBar: ScrollBar {
        id: scrollBar
        background: Rectangle {
            implicitWidth: scrollBar.interactive ? 8 : 4
            implicitHeight: scrollBar.interactive ? 8 : 4

            opacity: scrollBar.active && scrollBar.size < 1.0 ? 1.0 : 0.0
            color: Colors.primaryPanel
            Behavior on opacity {
                OpacityAnimator {
                    duration: 500
                }
            }
        }
        contentItem: Rectangle {
            implicitWidth: scrollBar.interactive ? 8 : 4
            implicitHeight: scrollBar.interactive ? 8 : 4
            opacity: scrollBar.active && scrollBar.size < 1.0 ? 1.0 : 0.0
            color: Colors.primaryText
            Behavior on opacity {
                OpacityAnimator {
                    duration: 1000
                }
            }
        }
    }
    //##############################################################
    //# Breakpoint component
    //##############################################################
    component Breakpoint: Image {
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 4
        anchors.bottomMargin: 4
        anchors.rightMargin: 4

        source: "qrc:///assets/breakpoint.svg"
        sourceSize.width: parent.height
        sourceSize.height: parent.height
        width: parent.width
        height: parent.height
        fillMode: Image.PreserveAspectFit
        asynchronous: true
        verticalAlignment: Image.AlignVCenter
        horizontalAlignment: Image.AlignRight
    }

    //##############################################################
    //# Constructor
    //##############################################################
    Component.onCompleted: {
        // Catch the error signal
        UiController.errorLoadingFile.connect(function(error) {
            isErrored = true;
            errorLabel.text = error;
        });
    }

    //##############################################################
    //# Javascript
    //##############################################################
}
