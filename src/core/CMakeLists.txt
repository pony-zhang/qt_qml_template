# Core module CMakeLists.txt
file(GLOB CORE_SOURCES
    "*.cpp"
    "*.h"
)

add_library(core STATIC ${CORE_SOURCES})

target_include_directories(core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../plugin>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(core PUBLIC
    Qt6::Core
    Qt6::Qml
)