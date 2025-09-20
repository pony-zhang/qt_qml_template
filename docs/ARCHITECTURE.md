# 项目架构指南

## 🏗️ 总体架构

本项目采用清洁架构模式，结合 MVVM (Model-View-ViewModel) 设计模式，实现模块化的 Qt QML C++ 应用程序。

### 核心设计原则

- **分层架构**: 清晰的职责分离，各层之间通过接口通信
- **依赖倒置**: 高层模块不依赖低层模块，都依赖抽象
- **MVVM 模式**: ViewModels 作为 UI 和业务逻辑之间的桥梁
- **插件化**: 核心功能通过插件系统扩展
- **模块化构建**: 每个模块独立的 CMake 构建配置

## 📁 目录结构

```
project/
├── src/                     # 源代码目录
│   ├── core/               # 核心模块
│   ├── data/               # 数据层
│   ├── business/           # 业务逻辑层
│   ├── utils/              # 工具模块
│   └── plugin/             # 插件系统
├── dependencies/           # 第三方依赖
├── resources/              # 资源文件
├── docs/                   # 项目文档
└── cmake/                  # CMake 配置
```

## 🎯 模块职责

### Core 模块 (`src/core/`)

**职责**:
- 应用程序生命周期管理
- 全局状态和配置管理
- 核心服务初始化和协调
- 系统级事件处理

**开发指导**:
- 保持轻量级，只包含系统级功能
- 避免业务逻辑，专注基础设施
- 提供全局单例服务
- 管理插件系统初始化

### Data 模块 (`src/data/`)

**职责**:
- 数据模型定义
- 持久化层操作
- 数据访问对象 (DAO)
- 数据验证和业务规则

**开发指导**:
- 定义纯粹的数据结构，不包含业务逻辑
- 实现数据库操作和文件存储
- 提供数据验证接口
- 使用 Qt 模型系统实现 QML 集成

### Business 模块 (`src/business/`)

**职责**:
- 业务逻辑和用例处理
- MVVM ViewModels 实现
- UI 后端服务
- 数据转换和业务规则

**开发指导**:
- ViewModels 继承 QObject 并使用 QML_ELEMENT 宏
- 通过 Q_INVOKABLE 暴露方法给 QML
- 使用 Qt 属性系统和信号槽机制
- 调用 Data 层 API，处理业务逻辑
- 提供 UI 服务：主题管理、通知、URL 处理等

### Utils 模块 (`src/utils/`)

**职责**:
- 通用工具函数
- 跨平台抽象
- 共享算法和数据结构
- 系统集成助手

**开发指导**:
- 保持工具函数的通用性和可重用性
- 提供跨平台兼容性
- 避免依赖特定业务逻辑
- 实现常用的算法和数据结构

### Plugin 模块 (`src/plugin/`)

**职责**:
- 插件架构定义
- 插件生命周期管理
- 插件加载和通信
- 示例插件实现

**开发指导**:
- 继承 BasePlugin 实现 IPlugin 接口
- 使用插件元数据 JSON 文件
- 实现初始化、配置、清理生命周期
- 提供插件间通信机制

## 🔄 层次交互

```
QML UI ← → Business (ViewModels) ← → Data (Models) ← → Core
                      ↓
                Plugin System
                      ↓
              Third-party Libraries
```

### 通信规则

1. **QML → Business**: 直接调用 ViewModel 方法
2. **Business → Data**: 调用数据访问接口
3. **Data → Core**: 使用系统级服务
4. **Plugin Integration**: 通过 Core 层的插件管理器

## 🚀 新功能开发指南

### 1. 创建新功能模块

**步骤**:
1. 在 `src/business/` 创建 ViewModel
2. 在 `src/data/` 创建数据模型（如需要）
3. 在 QML 中使用 ViewModel
4. 添加相应的测试

**示例结构**:
```
src/business/viewmodels/FeatureViewModel.h
src/business/viewmodels/FeatureViewModel.cpp
src/data/models/FeatureModel.h (可选)
src/data/models/FeatureModel.cpp (可选)
```

### 2. 开发 ViewModel

**基本模板**:
```cpp
#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QLoggingCategory>



class FeatureViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString status READ status NOTIFY statusChanged)

public:
    explicit FeatureViewModel(QObject *parent = nullptr);
    ~FeatureViewModel();

    QString status() const;

    Q_INVOKABLE void performAction(const QString &param);

signals:
    void statusChanged(const QString &status);

private:
    QString m_status;
};
```

### 3. 数据层集成

**最佳实践**:
- 使用 Qt 信号槽响应数据变化
- 在 ViewModel 中处理数据转换
- 实现数据验证和错误处理
- 使用 QML 友好的数据类型

### 4. 插件开发

**插件结构**:
```
src/plugin/yourplugin/
├── YourPlugin.h
├── YourPlugin.cpp
├── yourplugin.json
└── CMakeLists.txt
```

## 🛠️ 构建和依赖

### 模块间依赖

```
Core (独立)
├── Data (依赖 Core)
├── Utils (独立)
├── Business (依赖 Core, Data, Utils)
├── Plugin (依赖 Core)
└── Main App (依赖所有模块)
```

### 第三方库集成

1. **自动发现**: 将项目放入 `dependencies/` 目录
2. **CMake 项目**: 自动检测 CMakeLists.txt
3. **QMake 项目**: 自动检测 .pro 文件
4. **预编译库**: 使用 `add_prebuilt_third_party()`

## 📝 代码规范

### 命名约定

- **类名**: PascalCase (e.g., `FeatureViewModel`)
- **方法名**: camelCase (e.g., `performAction()`)
- **变量名**: camelCase (e.g., `m_status`)
- **文件名**: PascalCase (e.g., `FeatureViewModel.h`)
- **日志类别**: 小写下划线 (e.g., `feature.viewmodel`)

### 文件组织

每个类应该有自己的头文件和实现文件，按功能分组在相应目录中。

### 错误处理

- 使用 Qt 的异常处理机制
- 在 ViewModel 中提供错误状态属性
- 使用日志系统记录错误信息
- 向用户显示友好的错误消息

## 🧪 测试策略

### 单元测试

- 测试单个组件功能
- 模拟依赖项
- 验证边界条件

### 集成测试

- 测试模块间交互
- 验证数据流
- 测试插件集成

## 📚 最佳实践总结

1. **保持分层**: 严格遵守层次职责，避免跨层调用
2. **使用接口**: 通过抽象接口定义层间通信
3. **ViewModel 模式**: ViewModels 应该轻薄，主要负责 UI 状态管理
4. **插件化设计**: 可扩展功能应该考虑插件实现
5. **数据验证**: 在 Data 层实现数据验证规则
6. **错误处理**: 在所有层次实现一致的错误处理机制
7. **日志记录**: 使用结构化日志，便于调试和监控
8. **测试覆盖**: 为关键功能编写单元测试和集成测试

遵循这些指导原则可以确保代码的可维护性、可扩展性和高质量。