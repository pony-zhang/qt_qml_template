import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Page {
    id: root
    title: "Main Page"

    signal themeToggleRequested()

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            spacing: 20

            Label {
                text: "QML C++ Template"
                font.pixelSize: 20
                font.bold: true
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                icon.name: "view-refresh"
                onClicked: dataModel.clear()
                ToolTip.visible: hovered
                ToolTip.text: "Clear all items"
            }

            ToolButton {
                icon.name: "document-new"
                onClicked: dataModel.addItem("New Item", "Description", 0)
                ToolTip.visible: hovered
                ToolTip.text: "Add new item"
            }

            ToolButton {
                icon.name: "edit-select"
                onClicked: root.themeToggleRequested()
                ToolTip.visible: hovered
                ToolTip.text: "Toggle theme"
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 20
        anchors.margins: 20

        RowLayout {
            spacing: 20
            Layout.fillWidth: true

            Label {
                text: "Total Items: " + dataModel.getCount()
                font.pixelSize: 16
                font.bold: true
            }

            Item {
                Layout.fillWidth: true
            }

            Button {
                text: "Add Sample Data"
                onClicked: {
                    dataModel.addItem("Project Alpha", "First test project", 100)
                    dataModel.addItem("Project Beta", "Second test project", 250)
                    dataModel.addItem("Project Gamma", "Third test project", 150)
                }
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: listView
                model: dataModel
                spacing: 10
                clip: true

                delegate: ItemDelegate {
                    width: listView.width
                    height: 100
                    highlighted: ListView.isCurrentItem

                    Card {
                        anchors.fill: parent
                        anchors.margins: 5

                        RowLayout {
                            anchors.fill: parent
                            spacing: 15
                            anchors.margins: 10

                            ColumnLayout {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                spacing: 5

                                Label {
                                    text: name
                                    font.pixelSize: 16
                                    font.bold: true
                                    elide: Label.ElideRight
                                    Layout.fillWidth: true
                                }

                                Label {
                                    text: description
                                    font.pixelSize: 12
                                    color: Material.secondaryTextColor
                                    elide: Label.ElideRight
                                    Layout.fillWidth: true
                                }

                                RowLayout {
                                    spacing: 10

                                    Label {
                                        text: "Value: " + value
                                        font.pixelSize: 14
                                    }

                                    Switch {
                                        checked: enabled
                                        onToggled: {
                                            dataModel.setItemEnabled(id, checked)
                                        }
                                    }
                                }
                            }

                            RowLayout {
                                spacing: 10

                                Button {
                                    text: "+"
                                    onClicked: {
                                        dataModel.updateItemValue(id, value + 10)
                                    }
                                }

                                Button {
                                    text: "-"
                                    onClicked: {
                                        if (value > 0) {
                                            dataModel.updateItemValue(id, value - 10)
                                        }
                                    }
                                }

                                Button {
                                    text: "Remove"
                                    Material.background: Material.Red
                                    onClicked: {
                                        dataModel.removeItem(id)
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}