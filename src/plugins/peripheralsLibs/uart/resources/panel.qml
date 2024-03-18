import QtQuick

Item {
    // POINTER TO C++ OBJECT CONTAINING SIGNALS (initialized on construction)
    property var uart
    Connections {
        target: uart
        // CONNECT TO C++ SIGNALS HERE
        function onDataUpdated(data) {resultDisplay.text = data}
    }

    anchors.fill: parent

    Column {
        spacing: 10
        Text {

            Component.onCompleted: {
                uart.dataUpdated.connect(function(data) {resultDisplay.text = data})
            }

            id: resultDisplay
            color: "blue"
            text: "No input data."

        }

        Image {
            width: 30
            height: 30
            source: "qrc:///uart/resources/picture.svg"
        }
    }

}
