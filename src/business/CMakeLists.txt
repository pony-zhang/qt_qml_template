# Business layer CMakeLists.txt
# Define business library
add_library(business STATIC
    services/DataService.cpp
    viewmodels/AppViewModel.cpp
    viewmodels/DataViewModel.cpp
)

target_include_directories(business PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/services>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/viewmodels>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../data/models>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../core>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(business PUBLIC
    data
    core
    Qt6::Core
    Qt6::Quick
    Qt6::QuickControls2
)