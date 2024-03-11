import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import EmulvQt

Button {
    id: root
    padding: 7

    property alias source: btnImage.source
    property alias backgroundState: backgroundRect.state

    property string borderColor: Colors.borderColor

    background:  Item {
        anchors.fill: root

        // Background
        Rectangle {
            id: backgroundRect
            anchors.fill: parent
            color: root.pressed ? "transparent" : root.hovered ? Qt.rgba(255, 255, 255, 0.175) : "transparent"
            border.color: "transparent"
            border.width: 0

            state: "CLOSED"

            states: [
                State {
                    name: "CLOSED"
                    PropertyChanges { target: backgroundRect; color: root.pressed ? "transparent" : root.hovered ? Qt.rgba(255, 255, 255, 0.175) : "transparent"}
                },
                State {
                    name: "OPENED"
                    PropertyChanges { target: backgroundRect; color: Colors.secondaryPanel}
                }
            ]
        }

        //Bar at bottom of the Button
        Rectangle {
            anchors.bottom: parent.bottom
            color: Colors.borderColor
            width: root.width
            height: root.height/10
        }
    }

    contentItem: RowLayout {
        spacing: 5

        Image {
            id: btnImage
            height: parent.height/1.4
            width: height
            sourceSize: Qt.size(width, height)
        }

        Text {
            text: root.text
            font: root.font
            height: root.height
            verticalAlignment: Text.AlignVCenter
            color: Colors.primaryText
        }
    }

}
