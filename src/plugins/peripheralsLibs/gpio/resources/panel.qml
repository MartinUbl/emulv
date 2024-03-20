import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


Item {
    id: root

    readonly property string gpioInputLowSVG: "qrc:///gpio/resources/gpio_input_low.svg";
    readonly property string gpioOutputLowSVG: "qrc:///gpio/resources/gpio_output_low.svg";
    readonly property string gpioInputHighSVG: "qrc:///gpio/resources/gpio_input_high.svg";
    readonly property string gpioOutputHighSVG: "qrc:///gpio/resources/gpio_output_high.svg";
    readonly property int gpioRowLength: 16

    // POINTER TO C++ OBJECT CONTAINING SIGNALS (initialized on construction)
    property var gpio
    Connections {
        target: gpio

        // CONNECT TO C++ SIGNALS HERE
        function onPinStateChanged(pin, level, mode) {
            var newImageSrc;
            if (level === false && mode === false) {
                newImageSrc = gpioInputLowSVG
            } else if (level === false && mode === true) {
                newImageSrc = gpioOutputLowSVG
            } else if (level === true && mode === false) {
                newImageSrc = gpioInputHighSVG
            } else if (level === true && mode === true) {
                newImageSrc = gpioOutputHighSVG
            }

            if (pin < gpioRowLength) {
                gpioRowA.gpioBtnRow.data[pin].imageSource = newImageSrc;
            } else if (pin < gpioRowLength * 2) {
                gpioRowB.gpioBtnRow.data[pin - gpioRowLength].imageSource = newImageSrc;
            } else if (pin < gpioRowLength * 3) {
                gpioRowC.gpioBtnRow.data[pin - gpioRowLength * 2].imageSource = newImageSrc;
            }
        }
    }

    anchors.fill: parent
    anchors.topMargin: 3
    anchors.leftMargin: 10
    anchors.rightMargin: 10

    ColumnLayout {
        GpioRow {
            id: gpioRowA
            title: "GPIO A"
            btnPinOffset: 0
        }
        GpioRow {
            id: gpioRowB
            title: "GPIO B"
            btnPinOffset: gpioRowLength
        }
        GpioRow {
            id: gpioRowC
            title: "GPIO C"
            btnPinOffset: gpioRowLength * 2
        }
    }

    component GpioButton: ColumnLayout {
        property int gpioPinIndex: 0
        property int pinOffset: 0
        property string imageSource: gpioInputLowSVG

        Label {
            text: parent.gpioPinIndex
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: true
        }

        Image {
            width: 30
            height: 30
            source: parent.imageSource
            fillMode: Image.PreserveAspectFit
            sourceSize.width: 30
            sourceSize.height: 30
            asynchronous: true
            verticalAlignment: Image.AlignVCenter
            horizontalAlignment: Image.AlignHCenter
            MouseArea {
                anchors.fill: parent
                onClicked: root.gpio.togglePinLevel(pinOffset + gpioPinIndex)
            }
        }
    }

    component GpioRow: GroupBox {
        title: "GPIO"
        property alias gpioBtnRow: gpioBtnRow
        property int btnPinOffset: 0
        RowLayout {
            id: gpioBtnRow
            spacing: 10

            Repeater {
                model: gpioRowLength
                GpioButton {
                    pinOffset: btnPinOffset
                    gpioPinIndex: index
                }
            }
        }

    }
}


