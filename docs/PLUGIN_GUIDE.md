# 插件开发指南

## 🎯 插件系统概述

本项目提供了一个完整的插件系统，支持动态加载、配置管理和功能扩展。插件系统设计遵循以下原则：

- **模块化**: 每个插件都是独立的功能模块
- **标准化**: 统一的接口和生命周期管理
- **可扩展**: 支持运行时加载和卸载
- **配置化**: 支持动态配置和参数调整

## 📁 插件结构

### 标准插件目录结构

```
my-plugin/
├── CMakeLists.txt              # 构建配置
├── MyPlugin.h                  # 插件主类
├── MyPlugin.cpp                # 插件实现
├── myplugin.json               # 插件元数据
└── README.md                   # 插件文档
```

### 实际示例结构

```
src/plugin/log/
├── CMakeLists.txt
├── SmartLogPlugin.h
├── SmartLogPlugin.cpp
├── LogController.h
├── LogController.cpp
├── smartlog.json
└── README.md
```

## 🔧 插件开发步骤

### 1. 创建插件类

```cpp
// MyPlugin.h
#pragma once

#include "../BasePlugin.h"

class MyPlugin : public BasePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.template.myplugin" FILE "myplugin.json")

public:
    QString name() const override { return "MyPlugin"; }
    QString version() const override { return "1.0.0"; }
    QString description() const override { return "My custom plugin"; }
    QString author() const override { return "Developer Name"; }

    // 可选：提供 QML 接口
    Q_INVOKABLE void performAction(const QString &param);

protected:
    bool onInitialize(const QVariantMap &config) override;
    void onShutdown() override;
    bool onSetSettings(const QVariantMap &settings) override;
    QVariantMap onGetSettings() const override;

private:
    QString m_setting1;
    bool m_setting2 = false;
};
```

### 2. 实现插件功能

```cpp
// MyPlugin.cpp
#include "MyPlugin.h"
#include <QDebug>

bool MyPlugin::onInitialize(const QVariantMap &config)
{
    qDebug() << "Initializing MyPlugin";

    // 读取配置
    if (config.contains("setting1")) {
        m_setting1 = config["setting1"].toString();
    }

    // 初始化资源
    // ... 初始化代码

    return true;
}

void MyPlugin::onShutdown()
{
    qDebug() << "Shutting down MyPlugin";

    // 清理资源
    // ... 清理代码
}

bool MyPlugin::onSetSettings(const QVariantMap &settings)
{
    if (settings.contains("setting1")) {
        m_setting1 = settings["setting1"].toString();
    }

    if (settings.contains("setting2")) {
        m_setting2 = settings["setting2"].toBool();
    }

    return true;
}

QVariantMap MyPlugin::onGetSettings() const
{
    QVariantMap settings;
    settings["setting1"] = m_setting1;
    settings["setting2"] = m_setting2;
    return settings;
}

void MyPlugin::performAction(const QString &param)
{
    qDebug() << "Performing action with param:" << param;

    // 实现具体功能
    // ... 功能代码
}
```

### 3. 创建插件元数据

```json
{
    "IID": "com.template.myplugin",
    "MetaData": {
        "name": "MyPlugin",
        "version": "1.0.0",
        "description": "My custom plugin for specific functionality",
        "author": "Developer Name",
        "category": "utility",
        "dependencies": [],
        "settings": {
            "setting1": {
                "type": "string",
                "default": "default_value",
                "description": "Setting 1 description"
            },
            "setting2": {
                "type": "boolean",
                "default": false,
                "description": "Setting 2 description"
            }
        }
    }
}
```

### 4. 配置构建文件

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.21)

project(MyPlugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

set(PLUGIN_SOURCES
    MyPlugin.cpp
)

set(PLUGIN_HEADERS
    MyPlugin.h
)

add_library(MyPlugin SHARED ${PLUGIN_SOURCES} ${PLUGIN_HEADERS})

target_link_libraries(MyPlugin PRIVATE
    plugin  # 基础插件库
    Qt6::Core
)

set_target_properties(MyPlugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)
```

## 🎨 QML 集成

### 1. 创建 QML 控制器

```cpp
// MyController.h
#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QVariantMap>

class MyController : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit MyController(QObject *parent = nullptr);

    Q_INVOKABLE void executeCommand(const QString &command);
    Q_INVOKABLE QVariantMap getPluginInfo();
    Q_INVOKABLE void setPluginSettings(const QVariantMap &settings);

signals:
    void commandExecuted(const QString &result);
    void settingsChanged(const QVariantMap &settings);

private:
    // 获取插件实例
    QObject* getPluginInstance();
};
```

### 2. 在 QML 中使用插件

```qml
import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    // 使用插件控制器
    MyController {
        id: pluginController
    }

    Button {
        text: "Execute Plugin Command"
        onClicked: {
            pluginController.executeCommand("my_command")
        }
    }

    Text {
        text: "Plugin Status: " + pluginController.getPluginInfo().name
    }
}
```

## 🔌 插件间通信

### 1. 信号槽机制

```cpp
// 在插件中定义信号
class MyPlugin : public BasePlugin
{
    Q_OBJECT

signals:
    void dataReady(const QVariantMap &data);
    void progressUpdated(int progress);
    void errorOccurred(const QString &error);
};

// 其他插件连接信号
void OtherPlugin::onInitialize(const QVariantMap &config)
{
    MyPlugin* myPlugin = getPlugin<MyPlugin>();
    if (myPlugin) {
        connect(myPlugin, &MyPlugin::dataReady,
                this, &OtherPlugin::handleData);
    }
}
```

### 2. 事件总线

```cpp
// 创建事件总线
class PluginEventBus : public QObject
{
    Q_OBJECT

public:
    static PluginEventBus* instance();

    void emitEvent(const QString &eventType, const QVariantMap &data);
    void connectToEvent(const QString &eventType, QObject *receiver, const char *slot);

signals:
    void eventOccurred(const QString &eventType, const QVariantMap &data);
};

// 使用事件总线
PluginEventBus::instance()->connectToEvent("data.ready",
    this, SLOT(handleDataReady(QString, QVariantMap)));
```

## ⚙️ 配置管理

### 1. 运行时配置

```cpp
// 从 UiBackend 配置插件
void configurePlugin()
{
    QVariantMap config;
    config["enabled"] = true;
    config["logLevel"] = "debug";
    config["api_key"] = "your_api_key";

    auto *manager = PluginManager::instance();
    manager->initializePlugin("MyPlugin", config);
}
```

### 2. 持久化配置

```cpp
// 保存配置到文件
void savePluginConfig()
{
    auto *manager = PluginManager::instance();
    QVariantMap settings = manager->getPluginInfo("MyPlugin");

    QSettings settings("MyApp", "Plugins");
    settings.beginGroup("MyPlugin");

    for (auto it = settings.constBegin(); it != settings.constEnd(); ++it) {
        settings.setValue(it.key(), it.value());
    }

    settings.endGroup();
}
```

## 🧪 插件测试

### 1. 单元测试

```cpp
// TestMyPlugin.cpp
#include <QtTest>
#include "MyPlugin.h"

class TestMyPlugin : public QObject
{
    Q_OBJECT

private slots:
    void testInitialization();
    void testSettings();
    void testFunctionality();
};

void TestMyPlugin::testInitialization()
{
    MyPlugin plugin;

    QVariantMap config;
    config["setting1"] = "test_value";

    QVERIFY(plugin.initialize(config));
    QCOMPARE(plugin.getSettings()["setting1"].toString(), "test_value");
}
```

### 2. 集成测试

```cpp
// TestPluginIntegration.cpp
#include <QtTest>
#include "PluginManager.h"
#include "MyPlugin.h"

class TestPluginIntegration : public QObject
{
    Q_OBJECT

private slots:
    void testPluginLoading();
    void testPluginCommunication();
};

void TestPluginIntegration::testPluginLoading()
{
    auto *manager = PluginManager::instance();

    QVERIFY(manager->loadPlugin("path/to/myplugin"));
    QVERIFY(manager->initializePlugin("MyPlugin"));

    auto *plugin = manager->getPlugin("MyPlugin");
    QVERIFY(plugin != nullptr);
}
```

## 🔍 调试技巧

### 1. 插件加载调试

```cpp
// 在插件构造函数中添加调试信息
MyPlugin::MyPlugin(QObject *parent) : BasePlugin(parent)
{
    qDebug() << "MyPlugin instance created";
}

// 在初始化方法中添加详细日志
bool MyPlugin::onInitialize(const QVariantMap &config)
{
    qDebug(pluginLog) << "Initializing with config:" << config;

    // ... 初始化代码

    qDebug(pluginLog) << "Initialization completed successfully";
    return true;
}
```

### 2. 插件通信调试

```cpp
// 使用 QSignalSpy 测试信号发射
void testSignalEmission()
{
    MyPlugin plugin;

    QSignalSpy spy(&plugin, &MyPlugin::dataReady);

    plugin.triggerDataReady();

    QVERIFY(spy.wait(1000));
    QCOMPARE(spy.count(), 1);
}
```

## 📦 发布和部署

### 1. 插件打包

```bash
# 构建插件
cmake --build . --target MyPlugin

# 复制到插件目录
cp build/plugins/libMyPlugin.so app/plugins/
```

### 2. 依赖检查

```bash
# 检查插件依赖
ldd build/plugins/libMyPlugin.so

# 使用 Qt 依赖工具
windeployqt --plugindir build/plugins build/plugins/libMyPlugin.dll
```

## 🚀 最佳实践

### 1. 设计原则

- **单一职责**: 每个插件只负责一个功能领域
- **松耦合**: 通过接口通信，避免直接依赖
- **可配置**: 提供运行时配置选项
- **错误处理**: 实现完善的错误处理机制

### 2. 性能优化

- **延迟加载**: 只在需要时初始化资源
- **资源管理**: 及时释放不再使用的资源
- **异步操作**: 避免阻塞主线程
- **缓存策略**: 合理使用缓存提高性能

### 3. 安全考虑

- **输入验证**: 验证所有外部输入
- **权限检查**: 实现必要的权限控制
- **数据保护**: 保护敏感数据和信息

### 4. 维护性

- **文档完整**: 提供详细的插件文档
- **版本管理**: 使用语义化版本控制
- **兼容性**: 保持向后兼容性
- **测试覆盖**: 实现全面的测试覆盖

遵循这些指导原则，可以开发出高质量、易维护的插件。