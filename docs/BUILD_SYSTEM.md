# 构建系统指南

## 🏗️ 构建系统概述

本项目采用现代 CMake 构建系统，支持模块化构建、依赖管理和多平台部署。构建系统设计遵循以下原则：

- **模块化**: 每个模块独立的构建配置
- **层次化**: 清晰的依赖关系和构建顺序
- **可扩展**: 支持第三方库自动发现
- **跨平台**: 支持 Windows、macOS、Linux

## 📁 构建系统结构

```
project/
├── CMakeLists.txt              # 根构建文件
├── cmake/                      # CMake 模块
│   ├── BuildOptions.cmake      # 构建选项
│   ├── PlatformConfig.cmake    # 平台配置
│   ├── Dependencies.cmake      # 依赖管理
│   ├── BuildPresets.cmake      # 构建预设
│   └── Packaging.cmake         # 打包配置
├── src/
│   └── CMakeLists.txt          # 源代码构建配置
├── dependencies/
│   └── CMakeLists.txt          # 第三方依赖配置
└── docs/                       # 项目文档
```

## 🚀 基本构建流程

### 1. 环境准备

```bash
# 检查 CMake 版本
cmake --version

# 检查 Qt 版本
qmake --version

# 安装依赖 (Ubuntu/Debian)
sudo apt-get install build-essential qt6-base-dev qt6-declarative-dev

# 安装依赖 (macOS)
brew install qt6 cmake ninja

# 安装依赖 (Windows)
# 使用 Qt Maintenance Tool 或 vcpkg
```

### 2. 构建项目

```bash
# 创建构建目录
mkdir build && cd build

# 配置项目
cmake .. -DCMAKE_BUILD_TYPE=Release

# 编译项目
cmake --build . -j$(nproc)

# 运行应用程序
./QmlCppTemplate
```

### 3. 开发构建

```bash
# 调试构建
mkdir build-debug && cd build-debug
cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build . -j$(nproc)

# 运行测试
ctest --output-on-failure
```

## 🔧 构建配置选项

### CMake 选项

```bash
# 基本选项
cmake .. -DCMAKE_BUILD_TYPE=Release|Debug|RelWithDebInfo|MinSizeRel

# 功能选项
cmake .. -DBUILD_TESTING=ON              # 启用测试
cmake .. -DBUILD_EXAMPLES=ON             # 启用示例
cmake .. -DENABLE_INSTALL=ON              # 启用安装
cmake .. -DBUILD_SHARED_LIBS=ON           # 构建共享库

# 编译器选项
cmake .. -DCMAKE_CXX_COMPILER=clang++     # 指定编译器
cmake .. -DCMAKE_CXX_STANDARD=20          # C++20 标准

# Qt 选项
cmake .. -DQt6_DIR=/path/to/qt6/lib/cmake/Qt6  # 指定 Qt 路径
```

### 预设构建配置

```bash
# 使用构建预设
cmake --preset release
cmake --preset debug
cmake --preset test

# 查看可用预设
cmake --list-presets
```

## 📦 模块构建配置

### 1. 根模块配置

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.21)

project(QmlCppTemplate VERSION 1.0.0 LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含 cmake 模块
include(cmake/BuildOptions.cmake)
include(cmake/PlatformConfig.cmake)
include(cmake/Dependencies.cmake)

# 添加子目录
add_subdirectory(src)
add_subdirectory(resources)
add_subdirectory(dependencies)

# 创建可执行文件
add_executable(${PROJECT_NAME} src/main.cpp)

# 配置构建信息
configure_build_info(${PROJECT_NAME})

# 设置编译选项
set_project_warnings(${PROJECT_NAME})
set_project_features(${PROJECT_NAME})

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    app_sources
    third_party
    Qt6::Core
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Widgets
)
```

### 2. 源代码模块配置

```cmake
# src/CMakeLists.txt
# 添加子目录模块
add_subdirectory(core)
add_subdirectory(data)
add_subdirectory(business)
add_subdirectory(utils)
add_subdirectory(plugin)

# 创建静态库集合
add_library(app_sources STATIC)

# 设置包含目录
target_include_directories(app_sources PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/data
    ${CMAKE_CURRENT_SOURCE_DIR}/business
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin
)

# 链接模块库
target_link_libraries(app_sources PUBLIC
    core
    data
    business
    utils
    plugin
    Qt6::Core
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Widgets
)
```

### 3. 子模块配置

```cmake
# src/business/CMakeLists.txt
# 收集源文件
file(GLOB BUSINESS_SOURCES
    "*.cpp"
    "*.h"
)

# 创建库
add_library(business STATIC ${BUSINESS_SOURCES})

# 设置包含目录
target_include_directories(business PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 链接依赖
target_link_libraries(business PUBLIC
    core
    data
    utils
    Qt6::Core
    Qt6::Quick
    Qt6::QuickControls2
)

# 设置编译特性
target_compile_features(business PUBLIC cxx_std_17)
```

## 🔌 第三方依赖管理

### 1. 自动发现机制

```cmake
# dependencies/CMakeLists.txt
# 自动发现 CMake 项目
function(auto_discover_dependencies)
    file(GLOB PROJECT_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

    foreach(PROJECT_DIR ${PROJECT_DIRS})
        if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_DIR})
            if(NOT PROJECT_DIR MATCHES "^(include|lib|bin|\\.|CMakeFiles)$")
                if(NOT PROJECT_DIR MATCHES "_build$" AND NOT PROJECT_DIR MATCHES "_install$")
                    message(STATUS "Auto-discovering project: ${PROJECT_DIR}")
                    add_third_party(${PROJECT_DIR})
                endif()
            endif()
        endif()
    endforeach()
endfunction()

# 执行自动发现
auto_discover_dependencies()
```

### 2. CMake 项目支持

```cmake
# 添加 CMake 项目
function(add_third_party PROJECT_NAME)
    set(PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME})

    if(EXISTS ${PROJECT_PATH}/CMakeLists.txt)
        message(STATUS "Found CMakeLists.txt for ${PROJECT_NAME}")

        # 创建构建目录
        set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_build)
        file(MAKE_DIRECTORY ${BUILD_DIR})

        # 配置和构建
        add_custom_command(
            OUTPUT ${INSTALL_DIR}/CMakeCache.txt
            COMMAND ${CMAKE_COMMAND}
                -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
                -DCMAKE_BUILD_TYPE=Release
                -DBUILD_SHARED_LIBS=OFF
                ${PROJECT_PATH}
            COMMAND ${CMAKE_COMMAND} --build . --target install
            WORKING_DIRECTORY ${BUILD_DIR}
        )

        # 创建构建目标
        add_custom_target(${PROJECT_NAME}_build
            DEPENDS ${INSTALL_DIR}/CMakeCache.txt
        )

        # 添加到全局依赖
        add_dependencies(third_party ${PROJECT_NAME}_build)
    endif()
endfunction()
```

### 3. QMake 项目支持

```cmake
# 添加 QMake 项目
elseif(EXISTS ${PROJECT_PATH}/${PROJECT_NAME}.pro)
    message(STATUS "Found QMake project for ${PROJECT_NAME}")

    # 使用 QMake 构建
    add_custom_command(
        OUTPUT ${DEPENDENCIES_LIB_DIR}/lib${PROJECT_NAME}.${CMAKE_STATIC_LIBRARY_SUFFIX}
        COMMAND cd ${PROJECT_PATH} &&
                qmake ${PROJECT_NAME}.pro "CONFIG+=staticlib" "CONFIG+=release" &&
                make
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${PROJECT_PATH}/lib${PROJECT_NAME}.${CMAKE_STATIC_LIBRARY_SUFFIX}
                ${DEPENDENCIES_LIB_DIR}/
    )
endif()
```

### 4. 预编译库支持

```cmake
# 添加预编译库
function(add_prebuilt_third_party PROJECT_NAME)
    set(oneValueArgs LIBRARY_PATH INCLUDE_DIR)
    cmake_parse_arguments(ADD_PREBUILT "" "${oneValueArgs}" "" ${ARGN})

    set(LIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${ADD_PREBUILT_LIBRARY_PATH})

    # 创建导入目标
    add_library(${PROJECT_NAME} STATIC IMPORTED)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        IMPORTED_LOCATION ${LIBRARY_PATH}
    )

    # 添加包含目录
    if(ADD_PREBUILT_INCLUDE_DIR)
        set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${ADD_PREBUILT_INCLUDE_DIR})
        set_target_properties(${PROJECT_NAME} PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES ${INCLUDE_DIR}
        )
    endif()

    # 链接到全局依赖
    target_link_libraries(third_party INTERFACE ${PROJECT_NAME})
endfunction()
```

## 🎯 构建目标

### 1. 默认目标

```bash
# 构建主项目
cmake --build . --target QmlCppTemplate

# 构建所有库
cmake --build . --target app_sources
cmake --build . --target core
cmake --build . --target business
```

### 2. 第三方依赖目标

```bash
# 构建特定第三方库
cmake --build . --target mylibrary_build

# 清理第三方库
cmake --build . --target deps-clean

# 重新构建第三方库
cmake --build . --target deps-rebuild

# 查看依赖状态
cmake --build . --target deps-status
```

### 3. 测试目标

```bash
# 运行所有测试
cmake --build . --target test

# 运行特定测试
cmake --build . --target TestMyFeature

# 生成测试覆盖率
cmake --build . --target coverage
```

## 🌍 跨平台构建

### 1. Linux 构建

```bash
# Ubuntu/Debian
sudo apt-get install build-essential qt6-base-dev qt6-declarative-dev cmake

mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
make -j$(nproc)
sudo make install
```

### 2. macOS 构建

```bash
# 使用 Homebrew
brew install qt6 cmake ninja

mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
cmake --build . -j$(sysctl -n hw.ncpu)

# 创建应用包
cmake --build . --target create-app-bundle
```

### 3. Windows 构建

```bash
# 使用 vcpkg 安装依赖
vcpkg install qt6-base:x64-windows qt6-declarative:x64-windows

# 配置构建
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake

# 构建
cmake --build . --config Release

# 部署
windeployqt Release/QmlCppTemplate.exe
```

## 📦 打包和部署

### 1. Linux 打包

```cmake
# cmake/Packaging.cmake
# 配置 Linux 打包
set(CPACK_GENERATOR "DEB;RPM")
set(CPACK_PACKAGE_NAME "qmlcpptemplate")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION "QML C++ Template Application")

include(CPack)
```

### 2. macOS 打包

```cmake
# 配置 macOS 应用包
set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE ON
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/cmake/Info.plist.in"
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.example.qmlcpptemplate"
    MACOSX_BUNDLE_BUNDLE_NAME "QmlCppTemplate"
)
```

### 3. Windows 打包

```cmake
# 配置 Windows 安装包
set(CPACK_GENERATOR "NSIS")
set(CPACK_PACKAGE_NAME "QmlCppTemplate")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "QmlCppTemplate")

include(CPack)
```

## 🛠️ 构建工具和脚本

### 1. 构建脚本

```bash
#!/bin/bash
# build.sh

set -e

BUILD_TYPE=${1:-Release}
BUILD_DIR="build-${BUILD_TYPE,,}"

echo "Building QmlCppTemplate in $BUILD_TYPE mode..."

# 创建构建目录
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# 配置项目
cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DBUILD_TESTING=ON

# 构建项目
cmake --build . -j$(nproc)

# 运行测试
if [ "$BUILD_TYPE" = "Debug" ]; then
    ctest --output-on-failure
fi

echo "Build completed successfully!"
```

### 2. CI/CD 配置

```yaml
# .gitlab-ci.yml
stages:
  - build
  - test
  - package

variables:
  BUILD_TYPE: "Release"

build:linux:
  stage: build
  script:
    - mkdir build && cd build
    - cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE
    - cmake --build . -j$(nproc)
  artifacts:
    paths:
      - build/

test:linux:
  stage: test
  script:
    - cd build
    - ctest --output-on-failure
  dependencies:
    - build:linux

package:linux:
  stage: package
  script:
    - cd build
    - cpack -G DEB
  artifacts:
    paths:
      - build/*.deb
```

## 🔍 调试构建系统

### 1. 构建调试

```bash
# 启用详细输出
cmake .. -DCMAKE_VERBOSE_MAKEFILE=ON

# 查看构建属性
cmake --build . --target help

# 检查依赖关系
cmake --build . --target graphviz
```

### 2. 常见问题

```bash
# Qt 未找到
cmake .. -DQt6_DIR=/path/to/qt6/lib/cmake/Qt6

# 编译器版本不兼容
cmake .. -DCMAKE_CXX_COMPILER=g++-11

# 依赖缺失
sudo apt-get install libx11-dev libxext-dev libxrender-dev
```

## 📚 最佳实践

### 1. 构建系统设计

- **模块化**: 每个模块独立的构建配置
- **可读性**: 使用清晰的变量名和注释
- **可维护性**: 避免硬编码路径和版本
- **可扩展性**: 支持新模块和依赖的添加

### 2. 性能优化

- **并行构建**: 使用 `-j$(nproc)` 并行编译
- **增量构建**: 合理使用依赖关系
- **缓存使用**: 启用 CMake 缓存
- **工具链选择**: 使用 Ninja 等高效构建工具

### 3. 版本管理

- **语义化版本**: 使用清晰的版本号
- **兼容性**: 保持 API 兼容性
- **更新策略**: 渐进式更新依赖
- **回滚机制**: 保留构建历史

遵循这些指导原则，可以维护一个高效、可靠的构建系统。