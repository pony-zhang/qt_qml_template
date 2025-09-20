import QtQuick

QtObject {
    readonly property color primary: "#2196F3"
    readonly property color primaryDark: "#1976D2"
    readonly property color accent: "#00BCD4"
    readonly property color background: "#F5F5F5"
    readonly property color surface: "#FFFFFF"
    readonly property color error: "#F44336"
    readonly property color onPrimary: "#FFFFFF"
    readonly property color onSurface: "#000000"
    readonly property color onBackground: "#000000"

    readonly property font heading: Qt.font({
        family: "Roboto",
        pixelSize: 24,
        weight: Font.Bold
    })

    readonly property font title: Qt.font({
        family: "Roboto",
        pixelSize: 20,
        weight: Font.Bold
    })

    readonly property font subtitle: Qt.font({
        family: "Roboto",
        pixelSize: 16,
        weight: Font.Medium
    })

    readonly property font body: Qt.font({
        family: "Roboto",
        pixelSize: 14,
        weight: Font.Normal
    })

    readonly property font caption: Qt.font({
        family: "Roboto",
        pixelSize: 12,
        weight: Font.Normal
    })

    readonly property int spacingSmall: 4
    readonly property int spacingMedium: 8
    readonly property int spacingLarge: 16
    readonly property int spacingXLarge: 24
    readonly property int spacingXXLarge: 32

    readonly property int borderRadius: 8
    readonly property int elevation: 2
}