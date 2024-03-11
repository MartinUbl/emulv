import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../controls"
import EmulvQt

// Ensure bound component behavior
pragma ComponentBehavior: Bound

Rectangle {
    id: root
    anchors.fill: parent

    color: Colors.primaryPanel

    // Define SVG path specifications
    property string folderOpenSVG: "qrc:///assets/folder_open.svg"
    property string folderClosedSVG: "qrc:///assets/folder_closed.svg"
    property string fileSVG: "qrc:///assets/file_icon.svg"

    property string desktopSVG: "qrc:///assets/desktop_icon.svg"
    property string homeSVG: "qrc:///assets/home_icon.svg"
    property string upSVG: "qrc:///assets/up_icon.svg"
    property string driveSVG: "qrc:///assets/drive_icon.svg"
    property string downloadSVG: "qrc:///assets/download_icon.svg"
    property string appSVG: "qrc:///assets/ev_icon.svg"


    // Define color specifications
    property color itemBackGroundColor: Colors.primaryPanel
    property color itemTextColor: Colors.primaryText
    property color scrollBarColor: Colors.primaryText

    // Signal for file click event
    signal fileClicked(string filePath)

    // Alias for root index
    property alias rootIndex: fileSystemTreeView.rootIndex


    // Top row with combobox and button
    Rectangle {
        color: Colors.secondaryPanel
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 3
        anchors.leftMargin: 3
        height: navigationRow.implicitHeight
        clip: true

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 3

            id: navigationRow

            IconButton {
                height: topComboBox.height
                width: topComboBox.height
                source: upSVG
                onClicked: {
                    fileSystemTreeView.model.setParentDirectoryAsRoot()
                }
            }

            //Combobox for setting the root folder
            ComboBox {
                Layout.fillWidth: true
                id: topComboBox
                model: ["System Root", "Desktop", "App Directory", "Home", "Downloads"]
                property var icons: {
                    "System Root": driveSVG,
                    "Desktop": desktopSVG,
                    "App Directory": appSVG,
                    "Home": homeSVG,
                    "Downloads": downloadSVG
                }

                onActivated: function (index) {
                    switch (index) {
                        case 0:
                            fileSystemTreeView.model.setToFilesystemRoot();
                            break;
                        case 1:
                            fileSystemTreeView.model.setTreeRootDirectory(fileSystemTreeView.model.desktopPath);
                            break
                        case 2:
                            fileSystemTreeView.model.setTreeRootDirectory(fileSystemTreeView.model.appPath);
                            break
                        case 3:
                            fileSystemTreeView.model.setTreeRootDirectory(fileSystemTreeView.model.homePath);
                            break
                        case 4:
                            fileSystemTreeView.model.setTreeRootDirectory(fileSystemTreeView.model.downloadsPath);
                            break
                    }
                }

                delegate: ItemDelegate {
                    id: delegate

                    required property var model
                    required property int index

                    width: topComboBox.width
                    contentItem: Label {
                        IconImage {
                            id: comboItemImg
                            source: topComboBox.icons[parent.text]
                            sourceSize.width: 12
                            sourceSize.height: 12
                            //fillMode: Image.PreserveAspectFit
                            color: Colors.primaryText
                            //anchors.verticalCenter: parent.verticalCenter
                            anchors.bottom: parent.bottom
                            anchors.top: parent.top

                            asynchronous: true
                            verticalAlignment: Image.AlignVCenter
                            horizontalAlignment: Image.AlignHCenter
                        }
                        leftPadding: comboItemImg.width + 5
                        bottomPadding: 3
                        text: delegate.model[topComboBox.textRole]
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter

                    }
                    highlighted: topComboBox.highlightedIndex === index
                }

                contentItem: Label {
                    id: comboContentLabel

                    leftPadding: 4
                    rightPadding: topComboBox.indicator.width + topComboBox.spacing

                    text: "Filesystem Root"

                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitWidth: 120
                    implicitHeight: 40

                    color: parent.pressed ? "transparent" : parent.hovered ? Qt.rgba(255, 255, 255, 0.175) : "transparent"
                    border.color: parent.hovered ? Colors.borderColor : "transparent"
                }

                popup: Popup {
                    y: topComboBox.height - 1
                    width: topComboBox.width
                    implicitHeight: contentItem.implicitHeight
                    padding: 1

                    contentItem: ListView {
                        clip: true
                        implicitHeight: contentHeight
                        model: topComboBox.popup.visible ? topComboBox.delegateModel : null
                        currentIndex: topComboBox.highlightedIndex

                        ScrollIndicator.vertical: ScrollIndicator {
                        }
                    }

                    background: Rectangle {
                        border.color: Colors.borderColor
                        radius: 2
                    }
                }
            }


        }

    }


    // TreeView representing the file system structure
    TreeView {
        id: fileSystemTreeView
        property int lastIndex: -1

        onRootIndexChanged: {
            var pth = model.getCurrentRootPath();
            if (pth === "")
                comboContentLabel.text = "Filesystem Root"
            else
                comboContentLabel.text = pth
        }

        // File system model
        model: UiController.fileSystemModel
        rootIndex: UiController.fileSystemModel.rootIndex

        // Flickable behavior
        boundsBehavior: Flickable.StopAtBounds
        boundsMovement: Flickable.StopAtBounds
        clip: true
        anchors.topMargin: topComboBox.height
        anchors.rightMargin: 3
        anchors.leftMargin: 3
        anchors.fill: parent

        // Initialize tree view
        Component.onCompleted: fileSystemTreeView.toggleExpanded(0)

        // Delegate representing each entry in the file system
        delegate: Rectangle {
            //By how many pixels is each tree level (item) indented
            readonly property int indentationConstant: 8

            // Assigned to by TreeView:
            required property TreeView treeView
            required property bool isTreeNode
            required property bool expanded
            required property int hasChildren
            required property int depth
            required property int row
            required property int column
            required property bool current

            id: treeDelegate
            implicitWidth: fileSystemTreeView.width > 0 ? fileSystemTreeView.width : 250
            implicitHeight: 25

            // From model:
            required property int index
            required property url filePath
            required property string fileName

            color: (treeDelegate.index === fileSystemTreeView.lastIndex) ? Qt.lighter(itemBackGroundColor) : itemBackGroundColor

            Row {
                id: itemRow
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                //Indentation spacer
                leftPadding: (depth * 8) + indentationConstant

                Image {
                    id: directoryIcon
                    anchors.bottomMargin: 3
                    anchors.bottom: parent.bottom

                    source: {
                        if (treeDelegate.hasChildren) {
                            return treeDelegate.expanded ? folderOpenSVG : folderClosedSVG;
                        } else {
                            return fileSVG;
                        }
                    }
                    sourceSize.width: 12
                    sourceSize.height: 12
                    fillMode: Image.PreserveAspectFit
                    horizontalAlignment: Image.AlignRight

                    width: 29

                    smooth: true
                    antialiasing: true
                    asynchronous: true
                }

                Label {
                    leftPadding: 5
                    anchors.bottom: parent.bottom
                    text: treeDelegate.fileName
                    verticalAlignment: Text.AlignBottom
                }

                Menu {
                    id: itemMenu
                    MenuItem {
                        text: "Show in file manager"
                        onTriggered: {
                            var path = String(treeDelegate.filePath);
                            var fileName = path.replace(/^.*(\\|\/|\:)/, '')
                            path = path.replace(fileName, "")
                            Qt.openUrlExternally(path)
                        }
                    }
                }

                TapHandler {
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    onSingleTapped: (eventPoint, button) => {
                        switch (button) {
                            case Qt.LeftButton:
                                fileSystemTreeView.toggleExpanded(treeDelegate.row)
                                fileSystemTreeView.lastIndex = treeDelegate.index
                                // If this model item doesn't have children, it means it's representing a file.
                                if (!treeDelegate.hasChildren) {
                                    root.fileClicked(treeDelegate.filePath)
                                }
                                break;
                            case Qt.RightButton:
                                itemMenu.popup()
                                break;
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
                color: scrollBarColor
                opacity: fileSystemTreeView.movingVertically ? 0.5 : 0.0
                Behavior on opacity {
                    OpacityAnimator {
                        duration: 500
                    }
                }
            }
        }
    }
}
