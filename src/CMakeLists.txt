# Source module CMakeLists.txt
# main.cpp is handled directly by the main CMakeLists.txt

# Add subdirectories for specific modules
add_subdirectory(core)
add_subdirectory(data)
add_subdirectory(business)
add_subdirectory(utils)
add_subdirectory(plugin)

# Define app sources library (for any remaining source files in src root)
add_library(app_sources STATIC)

target_include_directories(app_sources PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/data
    ${CMAKE_CURRENT_SOURCE_DIR}/business
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin
)

target_link_libraries(app_sources PUBLIC
    core
    data
    business
    utils
    plugin
    Qt6::Core
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Widgets
)