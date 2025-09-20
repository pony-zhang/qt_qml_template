# Build options and configurations

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type (Debug, Release, RelWithDebInfo, MinSizeRel)" FORCE)
endif()

# Available build types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;MinSizeRel" CACHE STRING "Available build types" FORCE)

# Global compile options
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    # Common flags for GCC and Clang
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wno-unknown-pragmas
    )

    # Position independent code
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Platform-specific compile options
if(WIN32)
    add_compile_definitions(
        UNICODE
        _UNICODE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
elseif(APPLE)
    # macOS specific options
    set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum macOS version")

    # Enable modern C++ features on macOS
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(
            -stdlib=libc++
        )
    endif()
elseif(UNIX)
    # Linux specific options
    add_compile_definitions(_LINUX)
endif()

# Debug specific flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")

# Export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable colors in output
if(CMAKE_COLOR_MAKEFILE)
    set(CMAKE_COLOR_MAKEFILE ON)
endif()