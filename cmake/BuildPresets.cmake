# Build presets and utilities

# Function to set up compiler warnings
function(set_project_warnings target)
    if(MSVC)
        # MSVC warnings
        target_compile_options(${target} PRIVATE
            /W4
            /permissive-
            /Zc:__cplusplus
        )
    else()
        # GCC/Clang warnings
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wcast-align
            -Wunused
            -Woverloaded-virtual
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wnull-dereference
            -Wdouble-promotion
            -Wformat=2
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op
            -Wuseless-cast
            -Wno-unknown-pragmas
        )

        # Additional Clang warnings
        if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            target_compile_options(${target} PRIVATE
                -Wdocumentation
                -Wdocumentation-unknown-command
                -Wmissing-prototypes
                -Wstrict-prototypes
            )
        endif()
    endif()
endfunction()

# Function to set up project features
function(set_project_features target)
    # C++ standard
    target_compile_features(${target} PRIVATE
        cxx_std_17
    )

    # Position independent code
    set_target_properties(${target} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
    )

    # Set output name
    set_target_properties(${target} PROPERTIES
        OUTPUT_NAME ${target}
    )
endfunction()

# Function to set up Qt resources
function(setup_qt_resources target)
    # Add QML resources
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/qml/qml.qrc")
        qt_add_resources(${target}_qml_resources "qml_resources"
            PREFIX "/qml/"
            FILES "qml/qml.qrc"
        )
        target_sources(${target} PRIVATE ${${target}_qml_resources})
    endif()

    # Add other resource files
    file(GLOB_RECURSE resource_files "${CMAKE_CURRENT_SOURCE_DIR}/resources/*.qrc")
    if(resource_files)
        foreach(resource_file ${resource_files})
            get_filename_component(resource_name ${resource_file} NAME_WE)
            qt_add_resources(${target}_${resource_name} "${resource_name}_resources"
                FILES "${resource_file}"
            )
            target_sources(${target} PRIVATE ${${target}_${resource_name}})
        endforeach()
    endif()
endfunction()

# Function to create tests
function(add_qt_test test_name)
    cmake_parse_arguments(PARSE_ARGV 1 TEST
        ""
        ""
        "SOURCES;LIBRARIES"
    )

    # Create test executable
    add_executable(${test_name} ${TEST_SOURCES})

    # Link Qt Test
    target_link_libraries(${test_name} PRIVATE
        Qt6::Test
        ${TEST_LIBRARIES}
    )

    # Set test properties
    set_target_properties(${test_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests
    )

    # Add to test suite
    add_test(NAME ${test_name} COMMAND ${test_name})

    # Enable automoc for tests
    set_target_properties(${test_name} PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON
    )
endfunction()

# Function to create plugins
function(create_plugin plugin_name)
    cmake_parse_arguments(PARSE_ARGV 1 PLUGIN
        ""
        ""
        "SOURCES;LIBRARIES"
    )

    # Create plugin library
    add_library(${plugin_name} SHARED ${PLUGIN_SOURCES})

    # Set plugin properties
    set_target_properties(${plugin_name} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
    )

    # Link libraries
    target_link_libraries(${plugin_name} PRIVATE
        ${PLUGIN_LIBRARIES}
    )

    # Set up Qt resources for plugin
    setup_qt_resources(${plugin_name})

    # Set warnings
    set_project_warnings(${plugin_name})

    # Set features
    set_project_features(${plugin_name})
endfunction()

# Function to create example
function(add_example example_name)
    cmake_parse_arguments(PARSE_ARGV 1 EXAMPLE
        ""
        ""
        "SOURCES;LIBRARIES"
    )

    # Create example executable
    add_executable(${example_name} ${EXAMPLE_SOURCES})

    # Link libraries
    target_link_libraries(${example_name} PRIVATE
        ${EXAMPLE_LIBRARIES}
    )

    # Set output directory
    set_target_properties(${example_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/examples
    )

    # Set up Qt resources
    setup_qt_resources(${example_name})

    # Set warnings
    set_project_warnings(${example_name})

    # Set features
    set_project_features(${example_name})
endfunction()

# Function to configure build info
function(configure_build_info target)
    # Generate build info header
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/BuildInfo.h.in
        ${CMAKE_BINARY_DIR}/include/BuildInfo.h
        @ONLY
    )

    # Add include directory
    target_include_directories(${target} PRIVATE
        ${CMAKE_BINARY_DIR}/include
    )
endfunction()

# Function to add precompiled headers
function(add_precompiled_headers target)
    if(MSVC)
        # Windows precompiled headers
        target_precompile_headers(${target} PRIVATE
            <vector>
            <string>
            <memory>
            <algorithm>
        )
    else()
        # GCC/Clang precompiled headers
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h")
            target_precompile_headers(${target} PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h
            )
        endif()
    endif()
endfunction()