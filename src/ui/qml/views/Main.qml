import QtQuick
import EmulvQt
import "../windows"
import "../layouts"
import "../controls"
import "../panels"

import QtQuick.Dialogs
import QtQuick.Effects
import QtQuick.Controls
import QtQuick.Layouts
import QtCore

BorderlessWindow {
    id: root
    title: qsTr("EMULV: RISC-V Emulator")
    minimumWidth: 690
    minimumHeight: 360

    //#######################################################
    //############### Window GUI structure
    //#######################################################

    //###########################
    //############# Window Header
    //###########################

    //################
    //#### Left side of the header
    //################
    headerLeftContent: [
        Image {
            height: headerHeight/1.4
            width: headerHeight
            sourceSize: Qt.size(width, height)
            mipmap: true
            source: "qrc:///assets/app_logo.svg"
        },
        Item {
            //Spacer
            width: 1
        },
        MenuBar {
            background: null
            Menu {
                title: qsTr("&File")
                id: fileMenu
                Shortcut {
                    sequences: ["Ctrl+F"]
                    onActivated: fileMenu.open()
                }

                Action {
                    text: qsTr("&Open...")
                    shortcut: "Ctrl+O"
                    onTriggered: {
                        fileDialogElf.open()
                    }
                }
                MenuSeparator { }
                MenuItem {
                    text: qsTr("Quit")
                    onTriggered: {
                        Qt.quit();
                    }
                }
            }

            FileDialog {
                id: fileDialogElf
                currentFolder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]
                onAccepted: clickedFile(selectedFile)
                nameFilters: ["Binary ELF (*.elf)", "All files (*)"]
            }

            Menu {
                id: toolsMenu
                title: qsTr("&Tools")
                Shortcut {
                    sequences: ["Ctrl+T"]
                    onActivated: toolsMenu.open()
                }

                Action {
                    id: selectConfiguration
                    text: qsTr("&Select configuration")
                    shortcut: "Ctrl+S"
                    onTriggered: {
                        fileDialogJson.open()
                    }
                }

            }
            Menu {
                id: themesMenu
                title: qsTr("T&hemes")
                Shortcut {
                    sequences: ["Ctrl+H"]
                    onActivated: themesMenu.open()
                }

                MenuItem {
                    CheckBox {
                        id: checkboxDarkTheme
                        text: "Dark"
                        checked: Colors.currentTheme == Colors.themes.dark
                        bottomPadding: 2
                        // Handler for checkbox state change
                        onCheckedChanged: {
                            if(!checkboxLightTheme.checked) {
                                checked = true;
                                return;
                            }

                            if(checked) {
                                checkboxLightTheme.checked = false;
                                Colors.currentTheme = Colors.themes.dark
                                themesMenu.close()
                            }
                        }
                    }
                }
                MenuItem {
                    CheckBox {
                        id: checkboxLightTheme
                        checked: Colors.currentTheme == Colors.themes.light  // Set initial state of checkbox
                        text: "Light"

                        bottomPadding: 2
                        // Handler for checkbox state change
                        onCheckedChanged: {
                            if(!checkboxDarkTheme.checked) {
                                checked = true;
                                return;
                            }

                            if(checked) {
                                checkboxDarkTheme.checked = false;
                                Colors.currentTheme = Colors.themes.light
                                themesMenu.close()
                            }
                        }
                    }
                }
            }

            Menu {
                title: qsTr("Help")
                MenuItem {
                    text: qsTr("About")
                    onTriggered: {
                        var component = Qt.createComponent("About.qml")
                        var window = component.createObject(root)
                        window.show()
                    }
                }
            }

            FileDialog {
                id: fileDialogJson
                currentFolder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]
                onAccepted: clickedConfigFile(selectedFile)
                nameFilters: ["JSON (*.json)", "All files (*)"]
            }
        }
    ]

    //################
    //#### Center of the header
    //################
    headerCenterContent: [
        Text {
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: root.title
            font.pixelSize: 14
            color: Colors.primaryText
        }
    ]

    //###########################
    //############# Window Body
    //###########################
    windowContent: [
        MainWindowLayout {
            anchors.fill: parent

            //################
            //#### Top bar content
            //################
            topBarContent: [
                Rectangle {
                    color: Colors.primaryPanel
                    anchors.fill: parent
                    id: topBarRect

                    //Background of this rect is a gradient indicating if program is stopped or running
                    gradient: Gradient {
                        orientation : Gradient.Horizontal
                        GradientStop { position: 0.33; color: Colors.primaryPanel }
                        GradientStop {id: topBarGradient; position: 2; color: Colors.primaryPanel }
                    }

                    Item {
                        anchors.fill: parent
                        anchors.leftMargin: 32.5

                        id: programControls
                        //The states change properies of this panel depending on current action
                        states: [
                            State {
                                name: "IDLE"
                                PropertyChanges { target: idleProgramLayout; visible: true}
                                PropertyChanges { target: runningProgramLayout; visible: false}
                                PropertyChanges { target: debuggingProgramLayout; visible: false}
                            },
                            State {
                                name: "RUNNING"
                                PropertyChanges { target: idleProgramLayout; visible: false}
                                PropertyChanges { target: runningProgramLayout; visible: true}
                                PropertyChanges { target: debuggingProgramLayout; visible: false}
                            },
                            State {
                                name: "DEBUGGING"
                                PropertyChanges { target: idleProgramLayout; visible: false}
                                PropertyChanges { target: runningProgramLayout; visible: false}
                                PropertyChanges { target: debuggingProgramLayout; visible: true}
                            }
                        ]

                        //IDLE state
                        RowLayout {
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            id: idleProgramLayout

                            ActionButton {
                                id: runButton
                                source: "qrc:///assets/run_button.svg"
                                height: 30
                                text: "Run"
                                enabled: isFileLoaded
                                //onClicked: runProgram();
                            }

                            ActionButton {
                                id: debugButton
                                source: "qrc:///assets/debug_button.svg"
                                height: 30
                                text: "Debug"
                                enabled: isFileLoaded
                                //onClicked: debugProgram();
                            }
                        }

                        //RUNNING state
                        RowLayout {
                            visible: false
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            id: runningProgramLayout

                            ActionButton {
                                id: terminateButton1
                                source: "qrc:///assets/stop_button.svg"
                                height: 30
                                text: "Terminate"
                                //onClicked: stopProgram();
                            }
                        }

                        //DEBUGGING state
                        RowLayout {
                            visible: false
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            id: debuggingProgramLayout

                            ActionButton {
                                id: terminateButton2
                                source: "qrc:///assets/stop_button.svg"
                                height: 30
                                text: "Terminate"
                                //onClicked: stopProgram();
                            }

                            ActionButton {
                                id: continueButton
                                source: "qrc:///assets/continue_button.svg"
                                height: 30
                                text: "Continue"
                                //onClicked: continueProgram();
                            }

                            ActionButton {
                                id: stepButton
                                source: "qrc:///assets/step_button.svg"
                                height: 30
                                text: "Step"
                                //onClicked: stepProgram();
                            }
                        }

                    }
                }
            ]

            //################
            //#### Left bar content
            //################
            leftMiddleBarContent: [
                Rectangle {
                    color: Colors.primaryPanel
                    anchors.fill: parent

                    RowLayout {
                        id: leftSidebar
                        spacing: 10
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.leftMargin: 2.5
                        anchors.topMargin: 0

                        // This row needs to be rotated 90 degrees
                        transform: Rotation {
                            angle: 90
                            origin.x: 30/2
                            origin.y: 30/2
                        }
                    }
                }
            ]

            //################
            //#### Right bar content
            //################
            rightMiddleBarContent: [
                Rectangle {
                    color: Colors.primaryPanel
                    anchors.fill: parent
                    id: rightMiddleRect

                    RowLayout {
                        id: rightSidebar
                        spacing: 10
                        anchors.top: parent.bottom
                        anchors.left: parent.left

                        //This row needs to be rotated -90 degrees and position adjusted
                        transform: [
                            Rotation {
                                angle: -90
                            }, Translate {
                                y: -(rightMiddleRect.height - rightSidebar.width)
                            }

                        ]
                    }

                }
            ]

            //################
            //#### Bottom bar content
            //################
            bottomBarContent: [
                Rectangle {
                    color: Colors.primaryPanel
                    anchors.fill: parent

                    RowLayout {
                        spacing: 10
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.leftMargin: 2.5
                        anchors.topMargin: 0
                        id: bottomSidebar
                    }
                }
            ]

            //################
            //#### Center area content
            //################
            centerContent: [
                SplitView {
                    anchors.fill: parent
                    orientation: Qt.Vertical

                    SplitView {
                        SplitView.fillHeight: true
                        SplitView.minimumHeight: 200

                        // Left center area
                        Rectangle {
                            implicitWidth: 175
                            visible: false
                            color: Colors.primaryPanel

                            SidePanelControlsPanel{
                                id: spControlLeft
                                closeButton.onClicked: {closeSidePanel(Main.SidePanelPositions.LEFT)}
                                popupButton.onClicked: {createSidebarFloatingWindow(Main.SidePanelPositions.LEFT)}
                            }

                            Item {
                                id: leftCenterPanel
                                anchors.topMargin: spControlLeft.height
                                anchors.fill: parent
                            }
                        }

                        // Center area
                        Loader {
                            id: centerLoader
                            SplitView.fillWidth: true
                            SplitView.minimumWidth: 220
                            SplitView.minimumHeight: 220
                            asynchronous: false

                            // By default an image with logo and text is shown
                            source: "../panels/CenterPlaceholderPanel.qml"
                        }

                        // Right center area
                        Rectangle {
                            implicitWidth: 225
                            visible: false
                            color: Colors.primaryPanel

                            SidePanelControlsPanel{
                                id: spControlRight
                                layoutDirection: Qt.LeftToRight
                                closeButton.onClicked: {closeSidePanel(Main.SidePanelPositions.RIGHT)}
                                popupButton.onClicked: {createSidebarFloatingWindow(Main.SidePanelPositions.RIGHT)}
                            }
                            Item {
                                id: rightCenterPanel
                                anchors.fill: parent
                                anchors.topMargin: spControlRight.height
                            }
                        }
                    }

                    // Bottom center area
                    Rectangle {
                        implicitHeight: 100
                        SplitView.minimumHeight: 50
                        visible: false
                        color: Colors.primaryPanel

                        SidePanelControlsPanel{
                            id: spControlBottom
                            closeButton.onClicked: {closeSidePanel(Main.SidePanelPositions.BOTTOM)}
                            popupButton.onClicked: {createSidebarFloatingWindow(Main.SidePanelPositions.BOTTOM)}
                        }
                        Item {
                            id: bottomCenterPanel
                            anchors.fill: parent
                            anchors.topMargin: spControlBottom.height
                        }
                    }
                }

            ]

            //################
            //#### Status bar content
            //################
            bottomStatusBarContent: [
                Rectangle {
                    id:statusBar
                    color: Colors.tertiaryPanel
                    anchors.fill: parent
                    StatusBarPanel {
                        id: statusBarPanel
                    }
                },
                MultiEffect {
                    source: statusBar
                    anchors.fill: statusBar
                    shadowBlur: 1
                    shadowEnabled: true
                    shadowColor: Colors.shadowColor
                    shadowVerticalOffset: -2
                    //visible: Colors.currentTheme == Colors.themes.dark
                }
            ]

        }
    ]

    //#######################################################
    //############### Properties
    //#######################################################
    // Dynamically created side panels
    property var fileExplorerPanel;
    property var lastOpenedPanel;
    property var memoryAreaPanel;
    property var debugInfoPanel;
    property var registersPanel;

    property bool isFileLoaded: false
    property string lastOpenedFile: ""

    property list<QtObject> sidebarItemArray // Holds existing sidebarItem objects
    // The following properties store index of sidebarItemArray pointing to a displayed object
    // (-1 = no object is displayed)
    property int leftDisplayedItem: -1
    property int rightDisplayedItem: -1
    property int bottomDisplayedItem: -1

    property var lastOpenedFiles: []

    property var activePeripheralsPanels: []

    //#######################################################
    //############### Settings - remain state after window exit
    //#######################################################
    Settings {
        id: settings
        // Items for the LastOpenedFilesPanel (max length = 30)
        property var lastOpenedFiles
    }

    //#######################################################
    //############### Window constructor / destructor
    //#######################################################
    Component.onCompleted: {
        // Dynamic construction of items should be delayed until the window is fully drawn
        Qt.callLater(()=>{
                         Qt.callLater(()=>{
                                          // Connect signals & slots from UiController

                                          // Program control buttons
                                          runButton.clicked.connect(UiController.runProgram);
                                          terminateButton1.clicked.connect(UiController.terminateProgram);
                                          terminateButton2.clicked.connect(UiController.terminateProgram);
                                          debugButton.clicked.connect(UiController.debugProgram);
                                          continueButton.clicked.connect(UiController.debugContinue);
                                          stepButton.clicked.connect(UiController.debugStep);

                                          // Emulator states
                                          UiController.emulatorReadyState.connect(function() {
                                              programReady();
                                          })
                                          UiController.emulatorRunningState.connect(function() {
                                              runProgram();
                                          })
                                          UiController.emulatorRunningDebugState.connect(function() {
                                              debugProgram();
                                          })
                                          UiController.emulatorDebugPausedState.connect(function() {
                                              debugPauseProgram();
                                          })
                                          UiController.emulatorTerminatedState.connect(function() {
                                              stopProgram();
                                          })

                                          // Adding peripherals panels
                                          UiController.newPanelAdded.connect(function(name, panel) {
                                              addSideBarItem(name, panel, null, Main.SidePanelPositions.BOTTOM);
                                              // Save this peripheral panel
                                              activePeripheralsPanels.push(sidebarItemArray[sidebarItemArray.length-1])
                                          })

                                          // Removing all peripheral panels
                                          UiController.removeAllPeripherals.connect(function() {
                                              if(activePeripheralsPanels.length == 0)
                                                  return;

                                              for (const panel of activePeripheralsPanels){
                                                  removeSideBarItem(panel.index);
                                              }

                                              activePeripheralsPanels = [];
                                          })

                                          UiController.failedToLoadConfig.connect(function(errorString) {
                                              statusBarPanel.leftSideModel = [errorString]
                                          })

                                      })
                         Qt.callLater(()=>{
                                          // Create File Explorer side bar item
                                          fileExplorerPanel = createSidePanel("../panels/FileExplorerPanel.qml", "File Explorer", "qrc:///assets/folder_yellow.svg", Main.SidePanelPositions.LEFT)
                                          fileExplorerPanel.fileClicked.connect(clickedFile);
                                      })

                         Qt.callLater(()=>{
                                          // Load last opened files from settings
                                          try {
                                              lastOpenedFiles = JSON.parse(settings.lastOpenedFiles)
                                          } catch(e) {
                                              lastOpenedFiles = []
                                          }

                                          //Create Last Opened side bar item
                                          lastOpenedPanel = createSidePanel("../panels/LastOpenedFilesPanel.qml", "Last Opened", "qrc:///assets/folder_blue.svg", Main.SidePanelPositions.LEFT)
                                          lastOpenedPanel.itemsModel = lastOpenedFiles
                                          lastOpenedPanel.clearList.connect(clearRecentlyOpenedList)
                                          lastOpenedPanel.itemClicked.connect(clickedFile)
                                      })

                         Qt.callLater(()=>{
                                          // Create memory area
                                          memoryAreaPanel = createSidePanel("../panels/MemoryAreaPanel.qml", "Memory", "qrc:///assets/memory_icon.svg", Main.SidePanelPositions.BOTTOM)
                                      })

                         Qt.callLater(()=>{
                                          // Create debug information
                                          debugInfoPanel = createSidePanel("../panels/DebugInfoPanel.qml", "Debug Info", "qrc:///assets/info_icon.svg", Main.SidePanelPositions.RIGHT)
                                      })

                         Qt.callLater(()=>{
                                          // Create registers
                                          registersPanel = createSidePanel("../panels/RegistersPanel.qml", "Registers", "qrc:///assets/registers_icon.svg", Main.SidePanelPositions.RIGHT)
                                      })
                     })
    }

    Component.onDestruction: {
        settings.lastOpenedFiles = JSON.stringify(lastOpenedFiles)
    }

    //#######################################################
    //############### Javascript
    //#######################################################

    //###########################
    //############# State callbacks (only changing UI state) (btn signals are connected to Controller in Component.onCompleted)
    //###########################
    function programReady() {
        isFileLoaded = true;
        statusBarPanel.rightSideModel = ["Ready"]
        statusBarPanel.leftSideModel = [lastOpenedFile]
    }

    function runProgram() {
        if(!isFileLoaded)
            return;

        programControls.state = "RUNNING"

        //TODO: Change gradient color to YELLOW when stopped in debugging mode
        topBarGradient.color = "red"
        statusBarPanel.leftSideModel = [statusBarPanel.leftSideModel[0]]
        statusBarPanel.rightSideModel = ["Running"]
    }

    function debugProgram() {
        if(!isFileLoaded)
            return;

        programControls.state = "DEBUGGING"

        topBarGradient.color = "red"
        statusBarPanel.leftSideModel = [statusBarPanel.leftSideModel[0]]
        statusBarPanel.rightSideModel = ["Debugging"]
    }

    function stopProgram() {
        programControls.state = "IDLE"

        topBarGradient.color = Qt.binding(function () {return Colors.primaryPanel})
        statusBarPanel.rightSideModel = ["Idle"]
        statusBarPanel.leftSideModel = [statusBarPanel.leftSideModel[0],("Program exited at: " + Qt.formatTime(new Date(), "hh:mm:ss"))];
    }

    function continueProgram() {
        topBarGradient.color = "red"
    }

    function debugPauseProgram() {
        programControls.state = "DEBUGGING"

        topBarGradient.color = "orange"
        statusBarPanel.rightSideModel = ["Paused"]
    }

    // Clicked a file in file explorer / file dialog
    function clickedFile(path) {
        isFileLoaded = false;

        centerLoader.source = "../panels/CodeAreaPanel.qml"

        // Make sure path is of string type, and remove the "file:///" prefix, if it's present
        path = String(path).replace(/^file:\/\/\//, '')
        print(path)

        lastOpenedFile = path

        //# 1) Save path into lastOpenedFiles
        //###################################

        // Check if lastOpenedFiles already contains this item
        var alreadyContains = false
        for (const arrayItem of lastOpenedFiles) {
            if(arrayItem.fullPath == path) {
                alreadyContains = true
            }
        }

        // Add item to lastOpenedFiles
        if(!alreadyContains) {
            lastOpenedFiles.unshift({"name": path.replace(/^.*[\\/]/, ''), "fullPath": path})
            if(lastOpenedFiles.length > 30) {
                lastOpenedFiles.pop();
            }
            lastOpenedPanel.itemsModel = lastOpenedFiles
        }

        //# 2) Call the openFile function
        //############################
        UiController.openFile(path)
    }

    function clickedConfigFile(path) {
        // Make sure path is of string type, and remove the "file:///" prefix, if it's present
        path = String(path).replace(/^file:\/\/\//, '')
        print(path)

        UiController.openConfigurationJson(path);
    }

    function clearRecentlyOpenedList() {
        lastOpenedFiles.length = 0
        lastOpenedPanel.itemsModel = lastOpenedFiles
    }

    //###########################
    //############# Functions for handling side panels
    //###########################
    enum SidePanelPositions {
        LEFT,
        RIGHT,
        BOTTOM
    }

    // An object that holds the full state of a sidebarItem
    Component {
        // Object type name
        id: sidebarItem
        QtObject {
            // Object fields
            property int index // Index of this object in sidebarItemArray (used as identifier)
            property int position // One of the sidePanelPositions values
            property var content // Holds reference to the content object of this panel
            property var button // Holds reference to the SidebarButton responsible for this item
        }
    }

    // For dynamic creation of side panels
    function createSidePanel(qmlFilePath, nameString, iconUri, location) {
        var panel
        const panelComponent = Qt.createComponent(qmlFilePath)
        if (panelComponent.status == Component.Ready) {
            panel = panelComponent.createObject(null);
            addSideBarItem(nameString, panel, iconUri, location);
        }
        return panel;
    }

    // Adds item into sidebarItemArray, constructs SidebarButton and displays it
    // Icon is used for making the button, if null, a default icon is used
    function addSideBarItem(textLabel, content, iconSource, position) {
        const buttonComponent = Qt.createComponent("../controls/SidebarButton.qml")

        // Figure out in which panel to put the button
        var parentObj
        switch(position) {
        case Main.SidePanelPositions.LEFT:
            parentObj = leftSidebar
            break;
        case Main.SidePanelPositions.RIGHT:
            parentObj = rightSidebar
            break;
        case Main.SidePanelPositions.BOTTOM:
            parentObj = bottomSidebar
            break;
        }

        // Now we can create the button
        if (buttonComponent.status == Component.Ready) {
            const index = sidebarItemArray.length

            var button = buttonComponent.createObject(parentObj);
            if(iconSource !== null) {
                button.source = iconSource
            } else {
                button.source = "qrc:///assets/default_icon.svg"
            }
            button.height = 30
            button.text = textLabel
            button.clicked.connect(function() {sidebarButtonClicked(index)})

            sidebarItemArray.push(sidebarItem.createObject(null, {index, position, content, button}))
        }
    }

    function removeSideBarItem(index) {
        var sidebarItem = sidebarItemArray[index]

        switch(sidebarItem.position) {
        case Main.SidePanelPositions.LEFT:
            if(leftDisplayedItem === index) {
                closeSidePanel(sidebarItem.position)
                leftDisplayedItem = -1
            }
            break;
        case Main.SidePanelPositions.RIGHT:
            if(rightDisplayedItem === index) {
                closeSidePanel(sidebarItem.position)
                rightDisplayedItem = -1
            }
            break;
        case Main.SidePanelPositions.BOTTOM:
            if(bottomDisplayedItem === index) {
                closeSidePanel(sidebarItem.position)
                bottomDisplayedItem = -1
            }
            break;
        }

        sidebarItem.content.parent = null;
        sidebarItem.button.parent = null;
        sidebarItemArray[index] = null;
    }

    // Displays item in one of the side panels (or hides it)
    function sidebarButtonClicked(index) {
        print("Clicked button " + index)

        var sidebarItem = sidebarItemArray[index]
        var parentPanel
        var isDisplayed = false

        switch(sidebarItem.position) {
        case Main.SidePanelPositions.LEFT:
            parentPanel = leftCenterPanel;
            isDisplayed = leftDisplayedItem === index

            break;
        case Main.SidePanelPositions.RIGHT:
            parentPanel = rightCenterPanel
            isDisplayed = rightDisplayedItem === index

            break;
        case Main.SidePanelPositions.BOTTOM:
            parentPanel = bottomCenterPanel
            isDisplayed = bottomDisplayedItem === index

            break;
        }


        if(!isDisplayed) {
            // Remove children of the panel, reset buttons
            clearSidePanel(sidebarItem.position)

            switch(sidebarItem.position) {
            case Main.SidePanelPositions.LEFT:
                leftDisplayedItem = index
                break;
            case Main.SidePanelPositions.RIGHT:
                rightDisplayedItem = index
                break;
            case Main.SidePanelPositions.BOTTOM:
                bottomDisplayedItem = index
                break;
            }

            // Put content into the panel, show it
            parentPanel.children = [sidebarItem.content];
            parentPanel.parent.visible = true

            sidebarItem.button.backgroundState = "OPENED"
        } else {
            // Content IS displayed in the panel, close panel
            closeSidePanel(sidebarItem.position)
        }
    }

    // Makes a floating window out of LEFT / RIGHT / BOTTOM side panel content
    function createSidebarFloatingWindow(position) {
        // Get panel content
        var panel
        var panelItem
        switch(position) {
        case Main.SidePanelPositions.LEFT:
            panelItem = sidebarItemArray[leftDisplayedItem]
            panel = leftCenterPanel
            break;
        case Main.SidePanelPositions.RIGHT:
            panelItem = sidebarItemArray[rightDisplayedItem]
            panel = rightCenterPanel
            break;
        case Main.SidePanelPositions.BOTTOM:
            panelItem = sidebarItemArray[bottomDisplayedItem]
            panel = bottomCenterPanel
            break;
        }

        // Spawn window
        const windowComponent = Qt.createComponent("../windows/BorderlessPopup.qml")
        if (windowComponent.status == Component.Ready) {
            var window = windowComponent.createObject(root, {"transientParent": root});
            //var window = windowComponent.createObject(null, {});

            window.windowContent = [panelItem.content]
            window.height = panel.height
            window.width = panel.width

            if(position === Main.SidePanelPositions.RIGHT) {
                window.x = root.x + (root.width - panel.width) - 10
                window.y = root.y + (root.height - panel.height) - 10
            } else {
                window.x = root.x + 10
                window.y = root.y + (root.height - panel.height) - 10
            }
            window.title = panelItem.button.text
            window.closing.connect(function() {
                closingSidebarFloatingWindow(panelItem.index)
            })

            window.show()
        }

        closeSidePanel(position)
        // Hide the button responsible for this item
        panelItem.button.visible = false
    }

    // Sidebar floating window has been closed, show button in the sidebar once again
    function closingSidebarFloatingWindow(index) {
        sidebarItemArray[index].button.visible = true
    }

    // Closes the LEFT / RIGHT / BOTTOM side panel (makes it invisible)
    function closeSidePanel(position) {
        var clearedPanel = clearSidePanel(position)
        clearedPanel.parent.visible = false

        return clearedPanel;
    }

    // Clears the panel and resets button state
    function clearSidePanel(position) {
        var panel
        var itemIndex

        switch(position) {
        case Main.SidePanelPositions.LEFT:
            if(leftDisplayedItem == -1) {
                return;
            }
            panel = leftCenterPanel
            itemIndex = leftDisplayedItem
            leftDisplayedItem = -1

            break;
        case Main.SidePanelPositions.RIGHT:
            if(rightDisplayedItem == -1) {
                return;
            }
            panel = rightCenterPanel
            itemIndex = rightDisplayedItem
            rightDisplayedItem = -1

            break;
        case Main.SidePanelPositions.BOTTOM:
            if(bottomDisplayedItem == -1) {
                return;
            }
            panel = bottomCenterPanel
            itemIndex = bottomDisplayedItem
            bottomDisplayedItem = -1

            break;
        }

        panel.children.length = 0
        sidebarItemArray[itemIndex].button.backgroundState = "CLOSED"

        return panel;
    }

}// End BorderlessWindow
