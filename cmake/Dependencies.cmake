# Dependencies configuration

# Find Qt
find_package(Qt6 REQUIRED COMPONENTS Core Quick QuickControls2 Widgets)

# Qt specific settings
if(Qt6_FOUND)
    # Enable Qt features
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    # Set Qt standard path
    if(QT_VERSION_MAJOR EQUAL 6)
        qt_standard_project_setup()
    endif()

    # Set Qt policy
    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.15)
        cmake_policy(SET CMP0087 NEW)
    endif()
endif()

# Find additional dependencies (optional)
find_package(Threads QUIET)

# Platform-specific dependencies
if(WIN32)
    # Windows specific dependencies
    find_package(DirectX11 QUIET)
    if(DirectX11_FOUND)
        add_compile_definitions(HAS_DIRECTX)
    endif()

elseif(APPLE)
    # macOS specific dependencies
    find_package(Cocoa QUIET)
    find_package(Metal QUIET)

    # Enable Objective-C++ if needed
    enable_language(OBJCXX)

elseif(UNIX AND NOT APPLE)
    # Linux specific dependencies
    find_package(X11 QUIET)
    if(X11_FOUND)
        add_compile_definitions(HAS_X11)
    endif()

    # Find Vulkan (optional)
    find_package(Vulkan QUIET)
    if(Vulkan_FOUND)
        add_compile_definitions(HAS_VULKAN)
    endif()
endif()

# Set global include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data
    ${CMAKE_CURRENT_SOURCE_DIR}/src/business
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/plugin
)

# Add compiler-specific options
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Clang specific options
    add_compile_options(
        -Wdocumentation
        -Wdocumentation-unknown-command
        -Wmissing-prototypes
        -Wstrict-prototypes
    )

elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # GCC specific options
    add_compile_options(
        -Wsuggest-override
        -Wlogical-op
        -Wduplicated-cond
    )

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # MSVC specific options
    add_compile_options(
        /W4
        /permissive-
        /Zc:__cplusplus
    )
    add_compile_definitions(
        _CRT_SECURE_NO_WARNINGS
        _SILENCE_CXX17_C_HEADER_DEPRECATION_WARNING
    )
endif()

# Enable LTO (Link Time Optimization) for release builds
if(CMAKE_BUILD_TYPE MATCHES "Release|RelWithDebInfo")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        include(CheckIPOSupported)
        check_ipo_supported(RESULT IPO_SUPPORTED)
        if(IPO_SUPPORTED)
            set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
        endif()
    endif()
endif()

# Set optimization flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG _DEBUG)
    add_compile_options(-O0 -g)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(NDEBUG)
    add_compile_options(-O3)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_definitions(NDEBUG)
    add_compile_options(-O2 -g)
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    add_compile_definitions(NDEBUG)
    add_compile_options(-Os)
endif()