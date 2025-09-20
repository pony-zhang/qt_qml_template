# UI module CMakeLists.txt
file(GLOB UI_SOURCES
    "backend/*.cpp"
    "backend/*.h"
    "*.cpp"
    "*.h"
)

add_library(ui STATIC ${UI_SOURCES})

target_include_directories(ui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../core
    ${CMAKE_CURRENT_SOURCE_DIR}/../models
)

target_link_libraries(ui PUBLIC
    core
    models
    Qt6::Core
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Widgets
)