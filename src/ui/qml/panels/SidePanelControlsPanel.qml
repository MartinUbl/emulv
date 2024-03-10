import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../controls"

Row {
    anchors.right: parent.right
    anchors.left: parent.left
    anchors.top: parent.top
    spacing: 2.5
    height: 25
    layoutDirection: Qt.RightToLeft
    clip: true

    property alias closeButton: closeBtn
    property alias popupButton: popupBtn

    IconButton {
        id: closeBtn
        height: 25
        source: "qrc:///assets/close.svg"
        opacity: hovered ? 1 : 0.60
    }

    IconButton {
        id: popupBtn
        height: 25
        source: "qrc:///assets/popup_icon.svg"
        opacity: hovered ? 1 : 0.60
    }
}
