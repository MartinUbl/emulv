import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Borderless_lib_test

Button {
    id: root
    padding: 7
    height: width

    property var source
    property string iconColor: Colors.primaryText
    property string borderColor: Colors.borderColor

    icon.source: source
    icon.color: iconColor

    background: Rectangle {
        color: root.pressed ? "transparent" : root.hovered ? Qt.rgba(255, 255, 255, 0.175) : "transparent"
        border.color: root.hovered ? root.borderColor : "transparent"
        border.width: 1
    }
}
