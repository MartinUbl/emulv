import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform
import QWindowKit
import "../controls"
import "../styles"
import Borderless_lib_test

Window {
    id: window
    width: 800
    height: 600
    color: Colors.primaryWindowBackgroud
    palette: Colors.currentPalette

    //Exposed public properties
    property alias windowAgent: windowAgent
    property alias headerHeight: windowHeader.height
    //Header content public properties
    property alias headerLeftContent: headerLeft.children
    property alias headerCenterContent: headerCenter.children
    property alias headerRightContent: headerRight.children
    //Window content
    property alias windowContent: windowContent.children


    // This will restore window to fullscreen if it was minimized in fullscreen mode
    property bool wasFullScreen
    property bool wasMinimized
    onWindowStateChanged: function(state){
        // State 1 means window is now minimized
        if(state === 1) {
            wasFullScreen = (root.visibility == Qt.WindowFullScreen)
            wasMinimized = true
            return
        }

        // Restore window to fullScreen if it was in fullScreen before
        if(wasMinimized && wasFullScreen) {
            root.visibility = Qt.WindowFullScreen;
            wasMinimized = false;
            wasFullScreen = false;
        }
    }

    Component.onCompleted: {
        windowAgent.setup(window)
        window.visible = true
    }
    WindowAgent {
        id: windowAgent
    }

    Rectangle {
        id: windowHeader;
        anchors {
            top: parent.top
            topMargin: 1
            left: parent.left
            right: parent.right
        }
        height: 20
        color: window.active ? Colors.activeHeader : Colors.inactiveHeader
        Component.onCompleted: windowAgent.setTitleBar(windowHeader)

        GridLayout {
            uniformCellHeights: true
            uniformCellWidths: true
            columns: 3
            rows: 1
            anchors.fill: parent

            // Window tools area (left side of bar)
            RowLayout {
                Layout.leftMargin: 10
                Layout.fillWidth: true
                Layout.fillHeight: true
                id: headerLeft
                Component.onCompleted: {if(headerLeft.children.length !== 0) windowAgent.setHitTestVisible(headerLeft, true)}
            }

            // Window title area (center of bar)
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                RowLayout {
                    anchors.centerIn: parent
                    height: parent.height
                    id: headerCenter
                }
            }

            // Window buttons area (right side of bar)
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                Row {
                    anchors.top: parent.top
                    anchors.right: parent.right
                    height: parent.height
                    id: headerRight

                    WindowButton {
                        id: minButton
                        height: parent.height
                        source: "qrc:///assets/minimize.svg"
                        onClicked: window.showMinimized()
                        Component.onCompleted: windowAgent.setSystemButton(WindowAgent.Minimize, minButton)
                    }

                    WindowButton {
                        id: maxButton
                        height: parent.height
                        source: window.visibility === Window.Maximized ? "qrc:///assets/restore.svg" : "qrc:///assets/maximize.svg"
                        onClicked: {
                            if (window.visibility === Window.Maximized) {
                                window.showNormal()
                            } else {
                                window.showMaximized()
                            }
                        }
                        Component.onCompleted: windowAgent.setSystemButton(WindowAgent.Maximize, maxButton)
                    }

                    WindowButton {
                        id: closeButton
                        height: parent.height
                        source: "qrc:///assets/close.svg"
                        background: Rectangle {
                            color: {
                                if (!closeButton.enabled) {
                                    return Colors.closeButtonDisabled;
                                }
                                if (closeButton.pressed) {
                                    return Colors.closeButtonPressed;
                                }
                                if (closeButton.hovered) {
                                    return Colors.closeButtonHovered;
                                }
                                return Colors.closeButtonDefault;
                            }
                        }
                        onClicked: window.close()
                        Component.onCompleted: windowAgent.setSystemButton(WindowAgent.Close, closeButton)
                    }
                }
            }
        }
    }

    Item {
        anchors.topMargin: windowHeader.height
        anchors.fill: parent
        id: windowContent
    }

}
