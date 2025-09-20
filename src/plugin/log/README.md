# Smart Log Plugin - 智能日志系统

## 🎯 核心特性

### ✍️ 极简使用方式
保持使用熟悉的 `qDebug() << "..."` 语法，无需传递任何参数

### 🚀 零开销性能
- 被禁用的日志模块完全不执行
- 不构造字符串
- 零运行时开销

### 🧭 智能分类
自动根据源文件路径分配日志类别：
- `/ui/` → `app.ui`
- `/network/` → `app.network`
- `/plugin/` → `app.plugin`
- `/core/` → `app.core`
- 等等...

### 🎛️ 动态控制
支持运行时通过环境变量控制：
```bash
export QT_LOGGING_RULES="app.ui.debug=true;app.network.info=true"
```

### 📂 统一管理
- 结构化输出格式
- 支持控制台和文件输出
- JSON格式支持

## 📖 使用方法

### 基本使用
```cpp
#include "plugin/log/LogMacros.h"

// 自动分类日志
LOG_DEBUG() << "Debug message";  // 根据文件路径自动分类
LOG_INFO() << "Info message";
LOG_WARNING() << "Warning message";
LOG_CRITICAL() << "Critical message";

// 零开销日志
ZLOG_DEBUG() << "Zero overhead debug";
ZLOG_INFO() << "Zero overhead info";
```

### 环境变量控制
```bash
# 启用所有UI模块的调试日志
export QT_LOGGING_RULES="app.ui.debug=true"

# 禁用网络模块的日志
export QT_LOGGING_RULES="app.network*=false"

# 设置全局日志级别
export QT_LOGGING_RULES="*.debug=false;*.info=true"
```

### 编程接口
```cpp
// 获取日志控制器
auto *controller = new LogController();

// 动态设置日志级别
controller->setLogLevel("app.ui", "debug");
controller->enableCategory("app.network", true);

// 控制输出格式
controller->setJsonFormat(true);
controller->enableFileLogging("/path/to/logfile.log");
```

## 🏗️ 架构设计

### 设计原则
1. **零开销**: 禁用的日志不产生任何运行时开销
2. **自动化**: 基于文件路径自动分类
3. **统一性**: 所有日志通过单一处理器管理
4. **灵活性**: 支持多种输出格式和目标

### 核心组件
- `SmartLogPlugin`: 主插件类
- `LogFormatter`: 日志格式化器
- `LogController`: QML控制接口
- `ZeroOverheadLog`: 零开销日志实现

### 性能优化
- 编译时日志级别检查
- 字符串构造延迟
- 线程安全设计
- 最小化锁竞争

## 🔧 配置选项

### 插件配置
```cpp
QVariantMap config;
config["logFile"] = "/path/to/logfile.log";
config["consoleLogging"] = true;
config["jsonFormat"] = false;
config["logRules"] = "app.ui.debug=true;app.network.info=true";
```

### 支持的格式
- **文本格式**: `[timestamp] [level] [category] file:line:function - message`
- **JSON格式**: 结构化JSON输出
- **自定义格式**: 支持自定义格式模式

## 📊 示例输出

### 文本格式
```
[2024-01-20 14:30:25.123] [DEBUG] [app.ui] mainwindow.cpp:42:void MainWindow::init() - Initializing UI components
[2024-01-20 14:30:25.456] [INFO] [app.network] networkmanager.cpp:78:void NetworkManager::connect() - Connection established
```

### JSON格式
```json
{"timestamp":"2024-01-20T14:30:25.123","level":"DEBUG","category":"app.ui","file":"mainwindow.cpp","line":42,"function":"void MainWindow::init()","message":"Initializing UI components"}
```

## 🚀 部署说明

1. 编译插件：`cmake --build . --target SmartLogPlugin`
2. 插件会自动输出到 `plugins/` 目录
3. 应用启动时自动加载
4. 通过环境变量或QML接口动态控制

## 📈 性能对比

| 日志方式 | 开销 | 使用复杂度 | 功能 |
|---------|------|-----------|------|
| 标准qDebug | 高 | 简单 | 基础 |
| QLoggingCategory | 中 | 复杂 | 灵活 |
| SmartLogPlugin | 低 | 极简 | 完整 |

在禁用状态下，SmartLogPlugin的性能开销接近于零。