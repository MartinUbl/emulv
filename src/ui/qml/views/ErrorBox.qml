import QtQuick
import EmulvQt
import "../windows"
import "../controls"

import QtQuick.Effects
import QtQuick.Controls
import QtQuick.Layouts
import QtCore

BorderlessWindow {
    id: root
    width: 300
    height: 150
    minimumHeight: height
    maximumHeight: height
    minimumWidth: width
    maximumWidth: width

    minButton.visible: false
    maxButton.visible: false
    property string message: ""
    title: "Error"

    ColumnLayout {
        anchors.topMargin: 40
        anchors.fill: parent
        Label {
            text: "<h1>Runtime Error</h1>"
            horizontalAlignment: Qt.AlignHCenter
            Layout.fillWidth: true
        }

        Label {
            text: "<b>Program terminated due to:</b>"
            horizontalAlignment: Qt.AlignHCenter
            Layout.fillWidth: true
        }

        ScrollView {
            id: view
            Layout.fillWidth: true
            Layout.fillHeight: true

            TextArea {
                readOnly: true
                text: message
                background: Rectangle {
                    anchors.fill: parent
                    color: root.active ? Colors.activeHeader : Colors.inactiveHeader
                }
            }
        }
    }
}
