import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: root
    anchors.fill: parent
    anchors.topMargin: -lab1.height*2
    property real globalOpacity: 0.70

    Image {
        id: img
        opacity: globalOpacity
        source: "qrc:///assets/placeholder_image.svg"
        anchors.centerIn: parent

        height: 300
        width: height
        sourceSize: Qt.size(width, height)
    }
    Label {
        id: lab1
        opacity: globalOpacity
        anchors.horizontalCenter: img.horizontalCenter
        anchors.top: img.bottom
        anchors.topMargin: -40
        text: "Press <b>Ctrl + O</b> to open a file"
        font.pixelSize: 15
    }

    Label {
        opacity: globalOpacity
        anchors.horizontalCenter: lab1.horizontalCenter
        anchors.top: lab1.bottom
        anchors.topMargin: 10
        text: "Or use the File Explorer"
        font.pixelSize: 15
    }

}
