import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Borderless_lib_test

Rectangle {
    id: root

    readonly property int breakpointAreaWidth: 25
    readonly property int textAreaLeftPadding: lineNumPanelFlickable.width + breakpointAreaWidth
    // Height of each row calculated from text area metrics
    property int rowHeight: Math.ceil(fontMetrics.lineSpacing)

    // Background color
    color: "transparent"


    Menu {
        id: contextMenu
        MenuItem {
            text: "Remove all breakpoints"
            onTriggered: {
                removeAllBreakpoints()
            }
        }
    }

    // Background of line numbers side panel
    Rectangle {
        id: lineNumBackgroundRect
        color: Colors.primaryPanel
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: textAreaLeftPadding + 2
    }

    // Separator line between numbers and editor
    Rectangle {
        width: 1
        height: parent.height
        color: Qt.darker(Colors.primaryText, 3)
        anchors.right: lineNumBackgroundRect.right
    }

    // Layout for line numbers and text editor
    RowLayout {
        anchors.fill: parent
        clip: true
        //##############################################################
        //# Line numbers side panel
        //##############################################################
        Flickable {
            id: lineNumPanelFlickable

            // Calculate width based on logarithmic scale
            Layout.preferredWidth: fontMetrics.averageCharacterWidth * (Math.floor(Math.log10(textArea.lineCount)) + 1) + 10
            Layout.fillHeight: true

            interactive: false
            contentY: textAreaFlickable.contentY
            visible: textArea.text !== ""

            Column {
                anchors.fill: parent

                // Repeat for each line
                Repeater {
                    model: 101 // Temporarily hardcoded, should be based on actual line count

                    // Delegate representing items of the panel in each line
                    delegate: Row {
                        id: delegateRow

                        required property int index
                        width: parent.width
                        height: root.rowHeight

                        // Breakpoint area
                        Item {
                            id: breakpointArea
                            width: breakpointAreaWidth
                            height: parent.height
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    toggleBreakpoint(parent, delegateRow.index)
                                }
                            }
                        }

                        // Line number
                        Label {
                            text: parent.index
                            width: parent.width
                            height: parent.height
                            horizontalAlignment: Text.AlignRight
                            verticalAlignment: Text.AlignVCenter
                            rightPadding: 2
                            leftPadding: 1

                            color: Colors.primaryText
                            font: textArea.font
                        }
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
        }

        //##############################################################
        //# Text area panel
        //##############################################################
        Flickable {
            id: textAreaFlickable

            Layout.fillHeight: true
            Layout.fillWidth: true

            boundsBehavior: Flickable.StopAtBounds

            ScrollBar.horizontal: TextAreaScrollBar {}
            ScrollBar.vertical: TextAreaScrollBar {}

            // TextArea component
            TextArea.flickable: TextArea {
                id: textArea
                anchors.fill: parent

                readOnly: true // Make the TextArea read-only

                // Load content here once from a model or a file
                // For example, if you're loading from a file:
                Component.onCompleted: {
                    // Load content from file
                    // textArea.text = FileSystem.loadContent("filename.txt");
                }

                topPadding: 0
                leftPadding: textAreaLeftPadding // TODO?

                text: "test"

                // Text appearance properties
                color: Colors.primaryText
                selectedTextColor: Colors.primaryText
                selectionColor: "blue"
                textFormat: TextEdit.PlainText
                renderType: Text.QtRendering
                selectByMouse: true
                antialiasing: true
                background: null
            }

            // Font metrics for text area
            FontMetrics {
                id: fontMetrics
                font: textArea.font
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
    Component{
        id: breakpoint

        Image {
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.topMargin: 2
            anchors.bottomMargin: 2
            anchors.rightMargin: 2

            sourceSize.width: parent.height
            sourceSize.height: parent.height
            width: parent.width
            height: parent.height
            fillMode: Image.PreserveAspectFit
            asynchronous: true
            verticalAlignment: Image.AlignVCenter
            horizontalAlignment: Image.AlignRight
        }
    }

    //##############################################################
    //# Javascript
    //##############################################################
    property var breakpoints: []

    function toggleBreakpoint(senderObject, index) {
        print(index)

        if(senderObject.data.length === 1) {
            // Breakpoint is NOT inserted
            var bp = breakpoint.createObject(senderObject, {source: "qrc:///assets/breakpoint.svg"})
            breakpoints.push(bp)

            //TODO: BP created at index

        } else {
            // Breakpoint IS inserted at index 1
            senderObject.data[1].destroy()

            //TODO: BP removed at index
        }

    }

    function removeAllBreakpoints() {
        for(var i = 0; i < breakpoints.length; i++) {
            try {
                breakpoints[i].destroy()
                //TODO: BP removed at index
            } catch(e) {}
        }
        breakpoints.length = 0
    }

}
