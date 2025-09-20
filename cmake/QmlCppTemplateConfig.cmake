# QmlCppTemplateConfig.cmake
# Configuration file for QmlCppTemplate package

# Include helper functions
include(CMakeFindDependencyMacro)

# Find Qt dependencies
find_dependency(Qt6 REQUIRED COMPONENTS Core Quick QuickControls2 Widgets)

# Include targets
include("${CMAKE_CURRENT_LIST_DIR}/QmlCppTemplateTargets.cmake")

# Set include directories
get_target_property(QMLCPPTEMPLATE_INCLUDE_DIR QmlCppTemplate::core INTERFACE_INCLUDE_DIRECTORIES)

# Set Qt plugin path
if(Qt6_FOUND)
    get_target_property(QT_QML_PLUGIN_PATH Qt6::Qml IMPORTED_LOCATION_NOCONFIG)
    get_filename_component(QT_PLUGIN_DIR "${QT_QML_PLUGIN_PATH}" DIRECTORY)
endif()

# Provide convenience variables
set(QMLCPPTEMPLATE_FOUND TRUE)
set(QMLCPPTEMPLATE_VERSION ${PACKAGE_VERSION})
set(QMLCPPTEMPLATE_INCLUDE_DIRS ${QMLCPPTEMPLATE_INCLUDE_DIR})
set(QMLCPPTEMPLATE_LIBRARIES
    QmlCppTemplate::core
    QmlCppTemplate::data
    QmlCppTemplate::business
    QmlCppTemplate::utils
    QmlCppTemplate::plugin
)

# Provide compile definitions
get_target_property(QMLCPPTEMPLATE_DEFINITIONS QmlCppTemplate::core INTERFACE_COMPILE_DEFINITIONS)

# Provide Qt dependencies
set(QMLCPPTEMPLATE_QT_LIBRARIES
    Qt6::Core
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Widgets
)

# Check if all components were found
if(NOT TARGET QmlCppTemplate::core)
    set(QMLCPPTEMPLATE_FOUND FALSE)
    if(QmlCppTemplate_FIND_REQUIRED)
        message(FATAL_ERROR "QmlCppTemplate core library not found")
    endif()
endif()