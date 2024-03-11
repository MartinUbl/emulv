import QtQuick
import EmulvQt
import QtQuick.Controls
import "../controls"

Rectangle {
    anchors.fill: parent
    color: Colors.primaryPanel

    signal itemClicked(string filePath)
    signal clearList()

    property var itemsModel;


    property var lastActiveItemBackground: null

    Item  {
        anchors.fill: parent
        anchors.rightMargin: 3
        anchors.leftMargin: 6

        Label {
            id: infoLabel
            anchors.centerIn: parent
            text: "No recently opened items."
            wrapMode: "WordWrap"
            width: parent.width
            clip: true
            verticalAlignment: Qt.AlignVCenter
            horizontalAlignment: Qt.AlignHCenter
        }

        ListView {
            id: listView
            spacing: 0
            boundsBehavior: Flickable.StopAtBounds
            boundsMovement: Flickable.StopAtBounds

            // anchors.topMargin: buttonsRow.height
            anchors.topMargin: 5
            anchors.fill: parent
            clip: true
            model: itemsModel

            // Show or hide infoLabel depending on the amount of items
            onCountChanged: {
                if(count == 0) {
                    infoLabel.visible = true;
                    return;
                }
                infoLabel.visible = false;
            }

            delegate: Rectangle {
                id: itemBackgroundRect

                color: "transparent"
                anchors.left: parent.left
                anchors.right: parent.right
                height: itemTextLabel.height + 8

                Row {
                    id: itemRow
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 5

                    Image {
                        id: directoryIcon
                        anchors.bottomMargin: 3
                        anchors.bottom: parent.bottom

                        source: "qrc:///assets/file_icon.svg"
                        sourceSize.width: 12
                        sourceSize.height: 12
                        fillMode: Image.PreserveAspectFit
                        horizontalAlignment: Image.AlignRight

                        width: 12

                        smooth: true
                        antialiasing: true
                        asynchronous: true
                    }

                    Label {
                        id: itemTextLabel
                        leftPadding: 5
                        anchors.bottom: parent.bottom
                        text: {
                            var a = itemsModel
                            return itemsModel[index].name
                        }
                        verticalAlignment: Text.AlignBottom

                        // ToolTip.text: itemsModel[index].fullPath
                        // ToolTip.visible: ma.containsMouse
                        // ToolTip.delay: 1500

                        // MouseArea {
                        //     propagateComposedEvents: true
                        //     id: ma
                        //     anchors.fill: parent
                        //     hoverEnabled: true
                        // }

                    }

                    TapHandler {
                        acceptedButtons: Qt.LeftButton | Qt.RightButton
                        onSingleTapped: (eventPoint, button) => {
                                            switch (button) {
                                                case Qt.LeftButton:

                                                itemClicked(itemsModel[index].fullPath)

                                                itemBackgroundRect.color = Qt.lighter(Colors.primaryPanel)
                                                if(lastActiveItemBackground != null) {
                                                    lastActiveItemBackground.color = "transparent"
                                                }
                                                lastActiveItemBackground = itemBackgroundRect
                                                break;
                                                case Qt.RightButton:
                                                    contextMenu.popup()
                                                break;
                                            }
                                        }
                    }

                    Menu {
                        id: contextMenu
                        MenuItem {
                            text: "Clear list"
                            onTriggered: {
                                clearList()
                            }
                        }
                        MenuItem {
                            text: "Show in file manager"
                            onTriggered: {
                                var path = String(itemsModel[index].fullPath);
                                var fileName = path.replace(/^.*(\\|\/|\:)/, '')
                                path = path.replace(fileName, "")
                                Qt.openUrlExternally(path)
                            }
                        }
                    }
                }


            }

            // Custom scroll indicator
            ScrollIndicator.vertical: ScrollIndicator {
                active: true
                implicitWidth: 15

                contentItem: Rectangle {
                    implicitWidth: 6
                    implicitHeight: 6
                    color: Colors.primaryText
                    opacity: listView.movingVertically ? 0.5 : 0.0
                    Behavior on opacity {
                        OpacityAnimator {
                            duration: 500
                        }
                    }
                }
            }
        }
    }



}
