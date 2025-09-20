import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Window

Window {
    id: window
    width: 1200
    height: 800
    visible: true
    title: "QML C++ Template"

    property string currentTheme: "light"

    onCurrentThemeChanged: {
        if (currentTheme === "dark") {
            Material.theme = Material.Dark
            Material.accent = Material.Teal
        } else {
            Material.theme = Material.Light
            Material.accent = Material.Blue
        }
    }

    Connections {
        target: appViewModel
        function onThemeChanged(theme) {
            window.currentTheme = theme
        }
    }

    Component.onCompleted: {
        currentTheme = window.currentTheme
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: mainPage
    }

    Loader {
        id: mainPage
        source: "qrc:/qml/pages/MainPage.qml"
        onLoaded: {
            if (item && item.themeToggleRequested) {
                item.themeToggleRequested.connect(function() {
                    window.currentTheme = currentTheme === "light" ? "dark" : "light"
                })
            }
        }
    }
}