# Third-Party Dependencies

这个目录用于管理项目的第三方依赖，支持将完整的第三方项目直接放在此目录中，系统会自动识别并构建。

## 📁 目录结构

```
dependencies/
├── CMakeLists.txt              # 主构建文件
├── ThirdPartyConfig.cmake.in   # 配置模板
├── include/                    # 统一头文件目录（自动生成）
├── lib/                        # 统一库文件目录（自动生成）
├── bin/                        # 统一二进制文件目录（自动生成）
├── project-1/                  # 第一个第三方项目
│   ├── CMakeLists.txt          # 或 .pro 文件
│   ├── include/
│   ├── src/
│   └── ...
├── project-2/                  # 第二个第三方项目
│   ├── project-2.pro           # QMake 项目文件
│   ├── headers/
│   ├── sources/
│   └── ...
└── README.md                   # 本文件
```

## 🚀 使用方法

### 1. 添加第三方项目

将整个第三方项目目录复制到 `dependencies/` 目录下：

```bash
# 例如添加一个 CMake 项目
dependencies/my-cmake-lib/
├── CMakeLists.txt
├── include/mylib.h
├── src/mylib.cpp
└── ...

# 或者添加一个 QMake 项目
dependencies/my-qmake-lib/
├── my-qmake-lib.pro
├── headers/mylib.h
├── sources/mylib.cpp
└── ...
```

### 2. 自动发现和构建

系统会自动扫描 `dependencies/` 目录，发现具有 `CMakeLists.txt` 或 `.pro` 文件的项目：

- **CMake 项目**: 自动配置并构建，安装到独立目录
- **QMake 项目**: 自动使用 qmake 构建，复制库文件和头文件
- **预编译库**: 可以手动添加已编译好的库文件

### 3. 在主项目中使用

在主项目的 CMakeLists.txt 中：

```cmake
# 添加依赖子目录
add_subdirectory(dependencies)

# 链接所有第三方库
target_link_libraries(${PROJECT_NAME} PRIVATE
    third_party
)

# 包含头文件目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${THIRD_PARTY_INCLUDE_DIRS}
)
```

### 4. 手动添加特定项目

如果需要更精细的控制，可以手动添加：

```cmake
# 添加 CMake 项目，传递额外参数
add_third_party(myproject
    BUILD_TYPE Debug
    CMAKE_ARGS -DENABLE_FEATURES=ON -DBUILD_EXAMPLES=OFF
)

# 添加预编译库
add_prebuilt_third_party(mylib
    LIBRARY_PATH prebuilt/libmylib.a
    INCLUDE_DIR prebuilt/include
)
```

## 🛠️ 构建目标

### `deps-clean`
清理所有编译的依赖文件：
```bash
cmake --build . --target deps-clean
```

### `deps-status`
显示依赖状态和可用库：
```bash
cmake --build . --target deps-status
```

### `deps-rebuild`
重新构建所有依赖：
```bash
cmake --build . --target deps-rebuild
```

### `project-name_build`
构建特定项目：
```bash
cmake --build . --target myproject_build
```

## 📦 支持的项目类型

### CMake 项目
- 自动检测 `CMakeLists.txt`
- 支持传递 CMake 参数
- 自动安装到独立目录
- 自动发现生成的库文件

### QMake 项目
- 自动检测 `.pro` 文件
- 自动配置为静态库构建
- 复制生成的库文件到统一目录
- 自动复制头文件

### 预编译库
- 支持静态库文件
- 支持头文件目录
- 自动创建导入目标

## 🔧 配置选项

### CMake 项目参数
```cmake
add_third_party(project_name
    BUILD_TYPE Release|Debug          # 构建类型
    INSTALL_DIR /custom/path         # 安装目录
    CMAKE_ARGS "-DVAR=VALUE"         # 传递给 CMake 的参数
)
```

### 预编译库参数
```cmake
add_prebuilt_third_party(project_name
    LIBRARY_PATH path/to/lib.a       # 库文件路径
    INCLUDE_DIR path/to/include      # 头文件目录
)
```

## 📋 最佳实践

1. **项目命名**: 使用有意义的项目名称，建议包含版本号
2. **目录结构**: 保持第三方项目的原始结构
3. **依赖管理**: 尽量使用静态库以减少运行时依赖
4. **版本控制**: 在目录名中包含版本信息（如 `boost-1.83.0`）
5. **文档**: 在项目中添加 README 说明使用方法

## 🐛 故障排除

### 项目未被识别
- 检查项目目录中是否有 `CMakeLists.txt` 或 `.pro` 文件
- 确保目录名不包含特殊字符或空格
- 检查目录权限

### 构建失败
- 查看构建日志中的具体错误信息
- 检查是否缺少构建工具（如 qmake）
- 验证第三方项目的依赖是否满足

### 链接错误
- 确保库文件成功生成
- 检查库的类型（静态/动态）是否匹配
- 验证头文件路径是否正确

### CMake 版本
- 需要 CMake 3.21 或更高版本
- 某些功能可能需要更新版本的 CMake

## 🎯 高级特性

### 自动发现
系统会自动扫描目录中的所有项目，无需手动配置。

### 独立构建
每个第三方项目在独立的构建目录中构建，避免冲突。

### 统一接口
所有第三方库通过 `third_party` 目标统一链接，简化使用。

### 头文件管理
自动复制和管理头文件到统一目录。

## 📝 示例

```bash
# 添加一个 CMake 项目
git clone https://github.com/example/library.git dependencies/my-library

# 添加一个 QMake 项目
cp -r /path/to/qmake-project dependencies/my-qmake-project

# 构建项目
cmake --build . --target deps-clean
cmake --build .
```

系统会自动处理所有第三方项目的构建和集成。