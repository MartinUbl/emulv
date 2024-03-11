import QtQuick
import EmulvQt
import "../windows"
import "../controls"

import QtQuick.Effects
import QtQuick.Controls
import QtQuick.Layouts
import QtCore

BorderlessWindow {
    width: 200
    height: 300
    minimumHeight: height
    maximumHeight: height
    minimumWidth: width
    maximumWidth: width

    minButton.visible: false
    maxButton.visible: false

    title: "About"
    Column {
        anchors.centerIn: parent
        Label {
            text: "<h1>EMULV</h1>\n<p>Bakalářská práce 2024</p>\n<p>Jonáš Dufek</p>"
            horizontalAlignment: Qt.AlignHCenter
        }
    }
}
