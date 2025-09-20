# Core module CMakeLists.txt
file(GLOB CORE_SOURCES
    "*.cpp"
    "*.h"
)

add_library(core STATIC ${CORE_SOURCES})

target_include_directories(core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../plugin
)

target_link_libraries(core PUBLIC
    Qt6::Core
)