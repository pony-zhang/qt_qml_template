# Utils module CMakeLists.txt
file(GLOB UTILS_SOURCES
    "*.cpp"
    "*.h"
)

add_library(utils STATIC ${UTILS_SOURCES})

target_include_directories(utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../core
)

target_link_libraries(utils PUBLIC
    core
    Qt6::Core
)