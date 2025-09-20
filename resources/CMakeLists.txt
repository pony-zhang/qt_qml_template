# Resources module CMakeLists.txt
add_subdirectory(qml)

qt_add_resources(QML_RESOURCES "qml_resources"
    PREFIX "/qml/"
    FILES "qml/qml.qrc"
)