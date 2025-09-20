cmake_minimum_required(VERSION 3.21)

find_package(Qt6 REQUIRED COMPONENTS Core Quick QuickControls2 Widgets)

function(qmlcpp_add_resources TARGET_NAME)
    file(GLOB_RECURSE QML_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/qml/*.qml"
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/qml/*.js"
    )

    qt_add_resources(QML_RESOURCES "qml_resources"
        PREFIX "/"
        FILES ${QML_FILES}
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/qml/qmldir"
    )

    target_sources(${TARGET_NAME} PRIVATE ${QML_RESOURCES})
endfunction()

function(qmlcpp_setup_target TARGET_NAME)
    set_target_properties(${TARGET_NAME} PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Info.plist.in"
    )

    if(WIN32)
        set_target_properties(${TARGET_NAME} PROPERTIES
            WIN32_EXECUTABLE TRUE
            LINK_FLAGS "/ENTRY:mainCRTStartup"
        )
    endif()

    target_link_libraries(${TARGET_NAME} PRIVATE
        Qt6::Core
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Widgets
    )
endfunction()