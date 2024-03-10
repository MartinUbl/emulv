import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Borderless_lib_test

Button {
    id: root
    padding: 7

    property alias source: btnImage.source
    property string borderColor: Colors.borderColor

    background: Rectangle {
        color: root.pressed ? "transparent" : root.hovered ? Qt.rgba(255, 255, 255, 0.175) : "transparent"
        border.color: root.hovered ? root.borderColor : "transparent"
        border.width: 1
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
