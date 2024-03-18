import QtQuick
Item {
    property var gpio

    anchors.fill: parent

    Column {
        spacing: 10
        Text {

            Component.onCompleted: {
                gpio.dataUpdated.connect(function(data) {resultDisplay.text = data})
            }

            id: resultDisplay
            color: "blue"
            text: "No input data."

        }

        Image {
            width: 30
            height: 30
            source: "qrc:///qpio/resources/picture.svg"
        }
    }

}
