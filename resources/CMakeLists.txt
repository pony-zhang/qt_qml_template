# Resources module CMakeLists.txt
# QML files are now in top-level qml/ directory

qt_add_resources(QML_RESOURCES "qml_resources"
    PREFIX "/qml/"
    FILES "../qml/qml.qrc"
)

# Translation resources
qt_add_resources(TRANSLATION_RESOURCES "translation_resources"
    PREFIX "/i18n/"
    FILES "translation/translations.qrc"
)

# Create translation targets
find_package(Qt6 COMPONENTS LinguistTools REQUIRED)

# Update translation sources
qt_create_translation(QML_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/../qml"
    "${CMAKE_CURRENT_SOURCE_DIR}/translation"
    SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../qml/main.qml"
            "${CMAKE_CURRENT_SOURCE_DIR}/../qml/pages/MainPage.qml"
)

# Build translation targets
add_custom_target(translations
    COMMAND ${Qt6_LUPDATE_EXECUTABLE} ${QML_SOURCES} -ts "${CMAKE_CURRENT_SOURCE_DIR}/translation/app_en_US.ts"
    COMMAND ${Qt6_LUPDATE_EXECUTABLE} ${QML_SOURCES} -ts "${CMAKE_CURRENT_SOURCE_DIR}/translation/app_zh_CN.ts"
    COMMENT "Updating translation source files"
)

add_custom_target(release_translations
    COMMAND ${Qt6_LRELEASE_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/translation/app_en_US.ts" -qm "${CMAKE_CURRENT_SOURCE_DIR}/translation/app_en_US.qm"
    COMMAND ${Qt6_LRELEASE_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/translation/app_zh_CN.ts" -qm "${CMAKE_CURRENT_SOURCE_DIR}/translation/app_zh_CN.qm"
    DEPENDS translations
    COMMENT "Building translation binary files"
)