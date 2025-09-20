# Packaging and installation configuration

# Enable packaging
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# Set package version
set(PACKAGE_VERSION ${PROJECT_VERSION})
set(PACKAGE_NAME ${PROJECT_NAME})

# Installation paths
if(WIN32)
    # Windows installation paths
    set(INSTALL_BINDIR "bin")
    set(INSTALL_LIBDIR "lib")
    set(INSTALL_INCLUDEDIR "include")
    set(INSTALL_DATADIR "share")
    set(INSTALL_DOCDIR "share/doc")
    set(INSTALL_CMAKEDIR "lib/cmake")
elseif(APPLE)
    # macOS installation paths
    set(INSTALL_BINDIR "bin")
    set(INSTALL_LIBDIR "lib")
    set(INSTALL_INCLUDEDIR "include")
    set(INSTALL_DATADIR "share")
    set(INSTALL_DOCDIR "share/doc")
    set(INSTALL_CMAKEDIR "lib/cmake")
elseif(UNIX)
    # Linux installation paths (follow FHS)
    set(INSTALL_BINDIR "${CMAKE_INSTALL_BINDIR}")
    set(INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}")
    set(INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}")
    set(INSTALL_DATADIR "${CMAKE_INSTALL_DATADIR}")
    set(INSTALL_DOCDIR "${CMAKE_INSTALL_DOCDIR}")
    set(INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake")
endif()

# Install executable
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${INSTALL_BINDIR}
    COMPONENT Runtime
)

# Install libraries
install(TARGETS core data business utils plugin
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${INSTALL_LIBDIR}
    RUNTIME DESTINATION ${INSTALL_BINDIR}
    COMPONENT Development
)

# Install headers
install(DIRECTORY src/
    DESTINATION ${INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    COMPONENT Development
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
    PATTERN "*.inl"
    PATTERN "*.hxx"
)

# Install QML resources
install(FILES qml/qml.qrc
    DESTINATION ${INSTALL_DATADIR}/${PROJECT_NAME}
    COMPONENT Runtime
)

# Install plugin directory
install(DIRECTORY ${CMAKE_BINARY_DIR}/plugins/
    DESTINATION ${INSTALL_LIBDIR}/${PROJECT_NAME}/plugins
    COMPONENT Runtime
)

# Install CMake config files
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${INSTALL_CMAKEDIR}/${PROJECT_NAME}
    COMPONENT Development
)

# Generate package version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Install package config files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    "cmake/${PROJECT_NAME}Config.cmake"
    DESTINATION ${INSTALL_CMAKEDIR}/${PROJECT_NAME}
    COMPONENT Development
)

# Package configuration
set(CPACK_PACKAGE_NAME ${PACKAGE_NAME})
set(CPACK_PACKAGE_VERSION ${PACKAGE_VERSION})
set(CPACK_PACKAGE_VENDOR "Example")
set(CPACK_PACKAGE_CONTACT "example@example.com")

# Package description
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Qt QML C++ Template Application")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

# Package installers
set(CPACK_COMPONENTS_ALL Runtime Development)

# Platform-specific packaging
if(WIN32)
    # Windows NSIS installer
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/app.ico")
    set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/app.ico")
    set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")

    # Create desktop shortcut
    set(CPACK_NSIS_CREATE_ICONS_EXTRA "
        CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PROJECT_NAME}.lnk' '$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe'
    ")

elseif(APPLE)
    # macOS Drag & Drop package
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}-${PACKAGE_VERSION}")
    set(CPACK_DMG_FORMAT "UDBZ")

    # Code signing configuration (if needed)
    # set(CPACK_OSX_PACKAGE_SIGNATURE "Developer ID Application: Your Name")

elseif(UNIX)
    # Linux packages
    set(CPACK_GENERATOR "DEB;RPM")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "example@example.com")
    set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
    set(CPACK_RPM_PACKAGE_GROUP "Development/Tools")

    # Desktop file
    install(FILES
        "resources/${PROJECT_NAME}.desktop"
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
        COMPONENT Runtime
    )

    # Icon
    install(FILES
        "resources/icons/${PROJECT_NAME}.png"
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/256x256/apps
        COMPONENT Runtime
    )
endif()

# Include CPack
include(CPack)