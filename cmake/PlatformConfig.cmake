# Platform-specific configurations

# Windows configuration
if(WIN32)
    # Windows specific settings
    set(WIN32_EXECUTABLE TRUE)

    # Output directories
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

    # Windows SDK paths
    if(CMAKE_SYSTEM_VERSION MATCHES "10\\.0.*")
        add_compile_definitions(WINVER=0x0A00 _WIN32_WINNT=0x0A00)
    endif()

    # Disable Windows min/max macros
    add_compile_definitions(NOMINMAX)

    # Use Unicode character set
    add_compile_definitions(UNICODE _UNICODE)

endif()

# macOS configuration
if(APPLE)
    # macOS specific settings
    set(CMAKE_MACOSX_RPATH TRUE)

    # Output directories
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

    # Framework search paths
    set(CMAKE_FRAMEWORK_PATH
        ${CMAKE_FRAMEWORK_PATH}
        /usr/local/Frameworks
        /opt/homebrew/Frameworks
    )

    # Enable automatic reference counting for Objective-C
    if(CMAKE_OBJCXX_COMPILER)
        set(CMAKE_OBJCXX_FLAGS "${CMAKE_OBJCXX_FLAGS} -fobjc-arc")
    endif()

    # macOS deployment target
    if(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
        set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0")
    endif()

endif()

# Linux configuration
if(UNIX AND NOT APPLE)
    # Linux specific settings
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

    # Output directories
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

    # RPATH settings
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

    # Linux specific defines
    add_compile_definitions(_LINUX)

    # Enable as-needed linker flag
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--as-needed")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--as-needed")
    endif()

endif()

# Common configuration for all platforms
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)