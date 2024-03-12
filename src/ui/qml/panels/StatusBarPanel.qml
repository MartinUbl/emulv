import QtQuick
import EmulvQt
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    color: "transparent"
    anchors.fill: parent

    property var leftSideModel: []
    property var rightSideModel: []

    // Left side text
    ListView {
        id: leftSideList
        anchors.left: parent.left
        anchors.leftMargin: 3
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        interactive: false
        height: parent.height
        width: parent.width

        orientation: ListView.Horizontal

        model: leftSideModel
        delegate: Item {
            height: (parent != null) ? parent.height : 0
            width: textLabel.contentWidth + textLabel.rightPadding + textLabel.leftPadding
            Label {
                id: textLabel
                text: modelData
                color: "white"
                leftPadding: 5
                rightPadding: 5
            }
            Rectangle {
                anchors.right: textLabel.right
                width: 1
                height: parent.height
                color: "gray"
                opacity: 0.5
            }
        }
    }


    //Right side text
    ListView {
        id: rightSideList

        anchors.right: parent.right
        anchors.rightMargin: 3
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        layoutDirection: Qt.RightToLeft
        interactive: false
        orientation: ListView.Horizontal
        height: parent.height
        width: parent.width

        visible: !(childrenRect.width + leftSideList.childrenRect.width + 5 > parent.width)

        model: rightSideModel
        delegate: Item {
            height: parent.height
            width: textLabel2.contentWidth + textLabel2.rightPadding + textLabel2.leftPadding
            Label {
                id: textLabel2
                text: modelData
                color: "white"
                leftPadding: 5
                rightPadding: 5
            }
            Rectangle {
                anchors.left: textLabel2.left
                width: 1
                height: parent.height
                color: "gray"
                opacity: 0.5
            }
        }

    }
}
