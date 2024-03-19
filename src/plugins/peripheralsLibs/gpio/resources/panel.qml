import QtQuick
Item {
    // POINTER TO C++ OBJECT CONTAINING SIGNALS (initialized on construction)
    property var gpio
    Connections {
        target: gpio
        // CONNECT TO C++ SIGNALS HERE
        function onDataUpdated(data) {resultDisplay.text = data}
    }

    anchors.fill: parent

    Column {
        spacing: 10
        Text {
            id: resultDisplay
            color: "blue"
            text: "No input data."

        }

        Image {
            width: 30
            height: 30
            source: "qrc:///gpio/resources/picture.svg"
        }

    }

}
