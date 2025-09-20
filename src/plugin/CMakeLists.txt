file(GLOB PLUGIN_SOURCES
    "*.cpp"
    "*.h"
)

add_library(plugin STATIC ${PLUGIN_SOURCES})

target_include_directories(plugin PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(plugin PUBLIC
    Qt6::Core
    Qt6::Quick
    Qt6::QuickControls2
)

add_subdirectory(log)