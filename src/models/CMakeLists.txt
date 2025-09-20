# Models module CMakeLists.txt
file(GLOB MODELS_SOURCES
    "*.cpp"
    "*.h"
)

add_library(models STATIC ${MODELS_SOURCES})

target_include_directories(models PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../core
)

target_link_libraries(models PUBLIC
    core
    Qt6::Core
    Qt6::Qml
)