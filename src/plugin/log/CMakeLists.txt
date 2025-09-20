cmake_minimum_required(VERSION 3.21)

project(SmartLogPlugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

set(SMART_LOG_SOURCES
    SmartLogPlugin.cpp
    LogFormatter.cpp
    LogController.cpp
)

set(SMART_LOG_HEADERS
    SmartLogPlugin.h
    LogMacros.h
    SmartLogHandler.h
    ZeroOverheadLog.h
    LogFormatter.h
    LogController.h
)

add_library(SmartLogPlugin SHARED ${SMART_LOG_SOURCES} ${SMART_LOG_HEADERS})

target_link_libraries(SmartLogPlugin PRIVATE
    plugin
    Qt6::Core
)

set_target_properties(SmartLogPlugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)

target_include_directories(SmartLogPlugin PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)