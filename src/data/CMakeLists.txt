# Data layer CMakeLists.txt
# Define data library
add_library(data STATIC
    models/DataModel.cpp
)

target_include_directories(data PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/models>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/repositories>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(data PUBLIC
    Qt6::Core
)