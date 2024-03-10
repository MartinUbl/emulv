import QtQuick
import QtQuick.Layouts

GridLayout {
    id: root
    rows: 4
    columns: 3
    rowSpacing: 0
    columnSpacing: 0

    //Exposed properties for adding elements into layout
    property alias topBarContent: topBar.children
    property alias leftMiddleBarContent: leftMiddleBar.children
    property alias rightMiddleBarContent: rightMiddleBar.children
    property alias centerContent: center.children
    property alias bottomBarContent: bottomBar.children
    property alias bottomStatusBarContent: bottomStatusBar.children

    //********************************************************************************
    //* TOP
    //********************************************************************************

    //Top utility bar
    Item {
        id: topBar
        height: 40
        Layout.row: 0
        Layout.columnSpan: 3
        Layout.fillWidth: true
    }

    //********************************************************************************
    //* CENTER
    //********************************************************************************

    //Middle left utility bar
    Item {
        id: leftMiddleBar
        width: 32.5
        Layout.row: 1
        Layout.column: 0
        Layout.rowSpan: 2
        Layout.fillHeight: true
    }

    //Middle Right utility bar
    Item {
        id: rightMiddleBar
        width: 32.5
        Layout.row: 1
        Layout.column: 2
        Layout.rowSpan: 2
        Layout.fillHeight: true
    }

    //Center content area
    Item {
        id: center
        Layout.row: 1
        Layout.column: 1
        Layout.fillWidth: true
        Layout.fillHeight: true
    }

    //********************************************************************************
    //* BOTTOM
    //********************************************************************************

    //Botom utility bar
    Item {
        id: bottomBar
        height: 40
        Layout.row: 2
        Layout.column: 1
        Layout.fillWidth: true
    }

    //Botom status bar
    Item {
        id: bottomStatusBar
        height: 20
        Layout.row: 3
        Layout.column: 0
        Layout.columnSpan: 3
        Layout.fillWidth: true
    }

}
