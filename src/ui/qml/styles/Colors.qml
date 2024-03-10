pragma Singleton

import QtQuick
import QtCore

Item{
    id: root

    Settings {
        id: settings
        property var currentTheme
    }

    QtObject{
        id: themes
        readonly property var dark: ["#1E1E1E", "#3C3C3C", "#505050", "#333333", "#252526", "#007ACC", "#ECECEC","#707070", "#000"]
        readonly property var light: ["#F5F5F5", "#E0E0E0", "#D3D3D3", "#FFFFFF", "#FAFAFA", "#007ACC", "#333333","#707070", "#FFF"]
    }
    property alias themes: themes

    property var currentTheme: {
        if(!settings.currentTheme) {
            print("Setting dark theme (null settings)")
            return themes.dark
        } else {
            if(settings.currentTheme == "dark") {
                print("setting dark theme (settings)")
                return themes.dark
            } else if (settings.currentTheme == "light") {
                print("setting light theme (settings)")
                return themes.light
            }

            print("setting dark theme (settings invalid value)")
            return themes.dark
        }
    }

    Component.onDestruction: {
        // Can't save theme array directly - they're saved by value
        if(currentTheme == themes.dark) {
            print("saving dark theme")
            settings.currentTheme = "dark"
        } else {
            print("saving light theme")
            settings.currentTheme = "light"
        }
    }

    //Colors effected by theme change
    //------------------------------------------------------
    readonly property string primaryWindowBackgroud: currentTheme[0]
    readonly property string activeHeader: currentTheme[1]
    readonly property string inactiveHeader: currentTheme[2]

    readonly property string primaryPanel: currentTheme[3]
    readonly property string secondaryPanel: currentTheme[4]
    readonly property string tertiaryPanel: currentTheme[5]

    readonly property string primaryText: currentTheme[6]

    //ActionButton color
    readonly property string borderColor: currentTheme[7]

    readonly property string shadowColor: currentTheme[8]

    //Universal colors
    //------------------------------------------------------
    readonly property string closeButtonDisabled: "gray"
    readonly property string closeButtonPressed: "#e81123"
    readonly property string closeButtonHovered: "#e81123"
    readonly property string closeButtonDefault: "transparent"

    readonly property string windowButtonDisabled: "gray"
    readonly property string windowButtonPressed: Qt.rgba(0, 0, 0, 0.15)
    readonly property string windowButtonHovered: Qt.rgba(0, 0, 0, 0.15)
    readonly property string windowButtonDefault: "transparent"

    //QtQuick.controls color configuration
    //------------------------------------------------------

    property var currentPalette: {
        if(!currentTheme) {
            print("setting dark palette (null)")
            return darkPalette
        }

        if(currentTheme == themes.dark) {
            print("setting dark palette")
            return darkPalette
        } else {
            print("setting ligt palette")
            return lightPalette
        }

    }


    Palette {
        id: darkPalette

        alternateBase: Qt.lighter(primaryPanel)
        base: primaryPanel
        button: "#111"
        buttonText: primaryText
        dark: Qt.darker("#111")
        highlight: "#6cbdeb"
        highlightedText: "#000"
        light: Qt.lighter("#111")
        mid: "#444"
        midlight: "#333"
        placeholderText: "gray"
        shadow: "#888"
        text: "#fff"
        window: primaryWindowBackgroud
        windowText: "#fff"
    }
    Palette {
        id: lightPalette
        alternateBase: "#F5F5F5"
        base: "#FFFFFF"
        button: "#F0F0F0"
        buttonText: "#000000"
        dark: "#D3D3D3"
        highlight: "#6cbdeb"
        highlightedText: "#000"
        light: "#FFFFFF"
        mid: "#E0E0E0"
        midlight: "#D3D3D3"
        placeholderText: "#80000000"
        shadow: "#A9A9A9"
        text: "#000000"
        window: "#F5F5F5"
        windowText: "#000000"
    }
}
