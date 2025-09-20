cmake_minimum_required(VERSION 3.21)

project(QmlCppTemplate VERSION 1.0.0 LANGUAGES CXX)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set CMake policies
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.18)
    cmake_policy(SET CMP0094 NEW)
endif()

if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.15)
    cmake_policy(SET CMP0087 NEW)
endif()

# Include cmake modules
include(cmake/BuildOptions.cmake)
include(cmake/PlatformConfig.cmake)
include(cmake/Dependencies.cmake)
include(cmake/BuildPresets.cmake)

# Add subdirectories for modular build
add_subdirectory(src)
add_subdirectory(resources)
add_subdirectory(dependencies)

# Create executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    qml/qml.qrc
)

# Configure build info
configure_build_info(${PROJECT_NAME})

# Set up warnings and features
set_project_warnings(${PROJECT_NAME})
set_project_features(${PROJECT_NAME})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    app_sources
    third_party
    Qt6::Core
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Widgets
)

# Set platform-specific properties
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
        LINK_FLAGS "/ENTRY:mainCRTStartup"
    )
elseif(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE OFF
    )
endif()

# Include packaging configuration
include(cmake/Packaging.cmake)

# Enable testing
option(BUILD_TESTING "Build tests" OFF)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# Enable examples
option(BUILD_EXAMPLES "Build examples" OFF)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Enable installation
option(ENABLE_INSTALL "Enable installation targets" ON)
if(ENABLE_INSTALL)
    # Installation targets are handled in Packaging.cmake
endif()

# Print build configuration
message(STATUS "Build configuration:")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "  C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Build testing: ${BUILD_TESTING}")
message(STATUS "  Build examples: ${BUILD_EXAMPLES}")
message(STATUS "  Enable install: ${ENABLE_INSTALL}")