# Source module CMakeLists.txt
# main.cpp is handled directly by the main CMakeLists.txt

# Add subdirectories for specific modules
add_subdirectory(core)
add_subdirectory(models)
add_subdirectory(ui)
add_subdirectory(utils)
add_subdirectory(plugin)

# Define app sources library (for any remaining source files in src root)
add_library(app_sources STATIC)

target_include_directories(app_sources PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin
)

target_link_libraries(app_sources PUBLIC
    core
    models
    ui
    utils
    plugin
    Qt6::Core
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Widgets
)