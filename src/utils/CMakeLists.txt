# Utils module CMakeLists.txt
file(GLOB UTILS_SOURCES
    "*.cpp"
    "*.h"
)

add_library(utils STATIC ${UTILS_SOURCES})

target_include_directories(utils PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../core>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(utils PUBLIC
    core
    Qt6::Core
)