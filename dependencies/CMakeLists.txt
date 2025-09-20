cmake_minimum_required(VERSION 3.21)

project(ThirdPartyDependencies)

# Set output directories
set(DEPENDENCIES_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(DEPENDENCIES_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(DEPENDENCIES_BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Create output directories if they don't exist
foreach(DIR ${DEPENDENCIES_INCLUDE_DIR} ${DEPENDENCIES_LIB_DIR} ${DEPENDENCIES_BIN_DIR})
    if(NOT EXISTS ${DIR})
        file(MAKE_DIRECTORY ${DIR})
    endif()
endforeach()

# Create a dummy target to collect all third-party libraries
add_library(third_party INTERFACE)

# Set common include directories
target_include_directories(third_party INTERFACE
    ${DEPENDENCIES_INCLUDE_DIR}
)

# Global variables for third-party dependencies
set(THIRD_PARTY_INCLUDE_DIRS ${DEPENDENCIES_INCLUDE_DIR} CACHE PATH "Third-party include directories")
set(THIRD_PARTY_LIBRARIES third_party CACHE STRING "Third-party libraries")
set(THIRD_PARTY_LIBRARY_DIRS ${DEPENDENCIES_LIB_DIR} CACHE PATH "Third-party library directories")

# Function to add a third-party project
function(add_third_party PROJECT_NAME)
    set(options SKIP_BUILD FORCE_BUILD)
    set(oneValueArgs BUILD_TYPE INSTALL_DIR SOURCE_DIR)
    set(multiValueArgs CMAKE_ARGS BUILD_OPTIONS)
    cmake_parse_arguments(ADD_THIRD_PARTY "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME})

    if(NOT EXISTS ${PROJECT_PATH})
        message(FATAL_ERROR "Third-party project not found: ${PROJECT_PATH}")
    endif()

    message(STATUS "Processing third-party project: ${PROJECT_NAME}")

    # Set default build type
    if(NOT ADD_THIRD_PARTY_BUILD_TYPE)
        set(ADD_THIRD_PARTY_BUILD_TYPE "Release")
    endif()

    # Set default install directory
    if(NOT ADD_THIRD_PARTY_INSTALL_DIR)
        set(ADD_THIRD_PARTY_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_install)
    endif()

    # Check if project has CMakeLists.txt
    if(EXISTS ${PROJECT_PATH}/CMakeLists.txt)
        message(STATUS "Found CMakeLists.txt for ${PROJECT_NAME}")

        # Create build directory
        set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_build)
        file(MAKE_DIRECTORY ${BUILD_DIR})

        # Configure CMake options
        set(CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${ADD_THIRD_PARTY_INSTALL_DIR}
            -DCMAKE_BUILD_TYPE=${ADD_THIRD_PARTY_BUILD_TYPE}
            -DBUILD_SHARED_LIBS=OFF
            ${ADD_THIRD_PARTY_CMAKE_ARGS}
        )

        # Add build step
        add_custom_command(
            OUTPUT ${ADD_THIRD_PARTY_INSTALL_DIR}/CMakeCache.txt
            COMMAND ${CMAKE_COMMAND} ${CMAKE_ARGS} ${PROJECT_PATH}
            COMMAND ${CMAKE_COMMAND} --build . --target install --config ${ADD_THIRD_PARTY_BUILD_TYPE}
            WORKING_DIRECTORY ${BUILD_DIR}
            COMMENT "Building ${PROJECT_NAME}"
        )

        add_custom_target(${PROJECT_NAME}_build
            DEPENDS ${ADD_THIRD_PARTY_INSTALL_DIR}/CMakeCache.txt
        )

        # Add this target to the global third-party target
        add_dependencies(third_party ${PROJECT_NAME}_build)

        # Find and link installed libraries
        file(GLOB_RECURSE INSTALLED_LIBS "${ADD_THIRD_PARTY_INSTALL_DIR}/lib/*.${CMAKE_STATIC_LIBRARY_SUFFIX}")
        foreach(LIB ${INSTALLED_LIBS})
            get_filename_component(LIB_NAME ${LIB} NAME_WE)
            string(REGEX REPLACE "^lib" "" LIB_NAME ${LIB_NAME})

            # Create imported target
            add_library(${LIB_NAME} STATIC IMPORTED)
            set_target_properties(${LIB_NAME} PROPERTIES
                IMPORTED_LOCATION ${LIB}
                INTERFACE_INCLUDE_DIRECTORIES "${ADD_THIRD_PARTY_INSTALL_DIR}/include"
            )

            target_link_libraries(third_party INTERFACE ${LIB_NAME})
            message(STATUS "Added imported library: ${LIB_NAME}")
        endforeach()

    # Check for QMake project (.pro file)
    elseif(EXISTS ${PROJECT_PATH}/${PROJECT_NAME}.pro)
        message(STATUS "Found QMake project for ${PROJECT_NAME}")

        # Build with QMake
        add_custom_command(
            OUTPUT ${DEPENDENCIES_LIB_DIR}/lib${PROJECT_NAME}.${CMAKE_STATIC_LIBRARY_SUFFIX}
            COMMAND cd ${PROJECT_PATH} &&
                    qmake ${PROJECT_NAME}.pro "CONFIG+=staticlib" "CONFIG+=release" &&
                    make
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${PROJECT_PATH}/lib${PROJECT_NAME}.${CMAKE_STATIC_LIBRARY_SUFFIX}
                    ${DEPENDENCIES_LIB_DIR}/
            COMMENT "Building ${PROJECT_NAME} with QMake"
        )

        add_custom_target(${PROJECT_NAME}_build
            DEPENDS ${DEPENDENCIES_LIB_DIR}/lib${PROJECT_NAME}.${CMAKE_STATIC_LIBRARY_SUFFIX}
        )

        add_dependencies(third_party ${PROJECT_NAME}_build)

        # Copy headers if they exist
        if(EXISTS ${PROJECT_PATH}/include)
            file(COPY ${PROJECT_PATH}/include/ DESTINATION ${DEPENDENCIES_INCLUDE_DIR}/${PROJECT_NAME})
        endif()

    else()
        message(WARNING "No CMakeLists.txt or .pro file found for ${PROJECT_NAME}")
    endif()
endfunction()

# Function to add prebuilt third-party library
function(add_prebuilt_third_party PROJECT_NAME)
    set(oneValueArgs LIBRARY_PATH INCLUDE_DIR)
    cmake_parse_arguments(ADD_PREBUILT "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT ADD_PREBUILT_LIBRARY_PATH)
        message(FATAL_ERROR "LIBRARY_PATH is required for prebuilt libraries")
    endif()

    set(LIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${ADD_PREBUILT_LIBRARY_PATH})
    set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${ADD_PREBUILT_INCLUDE_DIR})

    if(NOT EXISTS ${LIBRARY_PATH})
        message(FATAL_ERROR "Prebuilt library not found: ${LIBRARY_PATH}")
    endif()

    # Create imported target
    add_library(${PROJECT_NAME} STATIC IMPORTED)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        IMPORTED_LOCATION ${LIBRARY_PATH}
    )

    # Add include directory if specified
    IF(ADD_PREBUILT_INCLUDE_DIR AND EXISTS ${INCLUDE_DIR})
        set_target_properties(${PROJECT_NAME} PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES ${INCLUDE_DIR}
        )
        # Copy headers to standard location
        file(COPY ${INCLUDE_DIR}/ DESTINATION ${DEPENDENCIES_INCLUDE_DIR}/${PROJECT_NAME})
    ENDIF()

    target_link_libraries(third_party INTERFACE ${PROJECT_NAME})
    message(STATUS "Added prebuilt library: ${PROJECT_NAME}")

    # Add copy target
    add_custom_target(${PROJECT_NAME}_copy
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${LIBRARY_PATH}
                ${DEPENDENCIES_LIB_DIR}/
        COMMENT "Copying prebuilt library: ${PROJECT_NAME}"
    )

    add_dependencies(third_party ${PROJECT_NAME}_copy)
endfunction()

# Function to scan and auto-add third-party projects
function(auto_discover_dependencies)
    file(GLOB PROJECT_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

    foreach(PROJECT_DIR ${PROJECT_DIRS})
        if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_DIR})
            # Skip system directories and hidden directories
            if(NOT PROJECT_DIR MATCHES "^(include|lib|bin|\\.|CMakeFiles)$")
                # Check if it's a build directory
                if(NOT PROJECT_DIR MATCHES "_build$" AND NOT PROJECT_DIR MATCHES "_install$")
                    message(STATUS "Auto-discovering project: ${PROJECT_DIR}")
                    add_third_party(${PROJECT_DIR})
                endif()
            endif()
        endif()
    endforeach()
endfunction()

# Auto-discover third-party projects
auto_discover_dependencies()

# Export dependencies for parent project
export(TARGETS third_party FILE ${CMAKE_CURRENT_BINARY_DIR}/ThirdPartyTargets.cmake)

# Create a config file for easy inclusion
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ThirdPartyConfig.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/ThirdPartyConfig.cmake @ONLY)

# Add custom targets for dependency management
add_custom_target(deps-clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${DEPENDENCIES_LIB_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${DEPENDENCIES_INCLUDE_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${DEPENDENCIES_BIN_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Cleaning all dependencies"
)

add_custom_target(deps-status
    COMMAND ${CMAKE_COMMAND} -E echo "Dependencies status:"
    COMMAND ${CMAKE_COMMAND} -E echo "  Include directory: ${DEPENDENCIES_INCLUDE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E echo "  Library directory: ${DEPENDENCIES_LIB_DIR}"
    COMMAND ${CMAKE_COMMAND} -E echo "  Binary directory: ${DEPENDENCIES_BIN_DIR}"
    COMMAND ${CMAKE_COMMAND} -E echo "  Available libraries:"
    COMMAND find ${DEPENDENCIES_LIB_DIR} -name "*.${CMAKE_STATIC_LIBRARY_SUFFIX}" -exec basename {} \; 2>/dev/null || echo "    (none)"
    COMMENT "Show dependency status"
)

add_custom_target(deps-rebuild
    COMMAND ${CMAKE_COMMAND} --build . --target deps-clean
    COMMAND ${CMAKE_COMMAND} --build .
    COMMENT "Rebuild all dependencies"
)