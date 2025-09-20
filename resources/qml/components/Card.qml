import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Rectangle {
    id: root
    color: Material.backgroundColor
    radius: 8
    border.width: 1
    border.color: Material.hintTextColor

    property alias content: contentItem.data

    default property alias contentData: contentItem.data

    Behavior on color {
        ColorAnimation {
            duration: 200
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            root.clicked()
        }

        onPressed: {
            root.scale = 0.98
        }

        onReleased: {
            root.scale = 1.0
        }

        onEntered: {
            root.color = Qt.darker(Material.backgroundColor, 1.05)
        }

        onExited: {
            root.color = Material.backgroundColor
            root.scale = 1.0
        }
    }

    Behavior on scale {
        NumberAnimation {
            duration: 100
            easing.type: Easing.OutQuad
        }
    }

    Item {
        id: contentItem
        anchors.fill: parent
        anchors.margins: 10
    }

    signal clicked()
}