import QtQuick
import QtQuick.Controls
import EmulvQt
import QtQuick.Effects

Button {
    id: root
    width: height * 1.5
    leftPadding: 0
    topPadding: 0
    rightPadding: 0
    bottomPadding: 0
    leftInset: 0
    topInset: 0
    rightInset: 0
    bottomInset: 0
    property alias source: image.source
    contentItem: IconImage {
        id: image
        anchors.centerIn: parent
        mipmap: true
        width: 12
        height: 12
        color: Colors.primaryText
    }


    background: Rectangle {
        color: {
            if (!root.enabled) {
                return Colors.windowButtonDisabled;
            }
            if (root.pressed) {
                return Colors.windowButtonPressed;
            }
            if (root.hovered) {
                return Colors.windowButtonHovered;
            }
            return Colors.windowButtonDefault;
        }
    }
}
