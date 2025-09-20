# 开发指南

## 🚀 快速开始

### 环境要求

- **Qt 6.2+**: Core, Quick, QuickControls2, Widgets
- **CMake 3.21+**: 现代构建系统
- **C++17**: 现代C++标准
- **编译器**: GCC 9+, Clang 10+, MSVC 2019+

### 构建项目

```bash
# 创建构建目录
mkdir build && cd build

# 配置项目
cmake ..

# 编译
make -j$(nproc)

# 运行
./QmlCppTemplate
```

## 📁 模块开发指南

### 1. Core 模块开发

Core 模块负责应用程序的基础设施，应该保持轻量级和系统级。

#### 文件结构
```
src/core/
├── Application.h           # 应用程序主类
├── Application.cpp
├── ConfigManager.h         # 配置管理
├── ConfigManager.cpp
├── SystemService.h         # 系统服务
└── SystemService.cpp
```

#### 开发模板
```cpp
// Application.h
#pragma once

#include <QObject>
#include <QSettings>

class Application : public QObject
{
    Q_OBJECT

public:
    static Application* instance();

    void initialize();
    void shutdown();

    QSettings* settings();

signals:
    void initialized();
    void aboutToQuit();

private:
    Application(QObject *parent = nullptr);
    ~Application();

    static Application* s_instance;
    QSettings* m_settings;
};
```

#### 开发要点
- 使用单例模式管理全局状态
- 提供系统级服务初始化
- 管理应用程序生命周期
- 避免包含业务逻辑

### 2. Data 模块开发

Data 模块负责数据持久化和模型定义。

#### 文件结构
```
src/data/
├── models/
│   ├── BaseModel.h         # 基础模型类
│   ├── DataModel.h         # 数据模型实现
│   └── UserModel.h         # 用户模型
└── repositories/
    ├── DataRepository.h    # 数据仓库接口
    └── DataRepository.cpp
```

#### 开发模板
```cpp
// DataModel.h
#pragma once

#include <QObject>
#include <QAbstractListModel>
#include <QList>

class DataItem
{
public:
    QString id;
    QString name;
    QVariant data;
};

class DataModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        DataRole
    };

    explicit DataModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addItem(const QString &name, const QVariant &data);
    Q_INVOKABLE void removeItem(int index);
    Q_INVOKABLE void clear();

private:
    QList<DataItem> m_items;
};
```

#### 开发要点
- 使用 Qt 模型系统实现 QML 集成
- 实现数据验证和约束
- 提供数据持久化接口
- 使用信号槽通知数据变化

### 3. Business 模块开发

Business 模块包含业务逻辑和 ViewModels，是 UI 层的后端。

#### 文件结构
```
src/business/
├── viewmodels/
│   ├── AppViewModel.h     # 应用程序视图模型
│   ├── AppViewModel.cpp
│   ├── FeatureViewModel.h  # 功能视图模型
│   └── FeatureViewModel.cpp
└── services/
    ├── DataService.h       # 数据服务
    └── DataService.cpp
```

#### ViewModel 开发模板
```cpp
// FeatureViewModel.h
#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QLoggingCategory>

#include "../services/DataService.h"



class FeatureViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(QVariantList items READ items NOTIFY itemsChanged)

public:
    explicit FeatureViewModel(QObject *parent = nullptr);
    ~FeatureViewModel();

    bool busy() const;
    QString status() const;
    QVariantList items() const;

    Q_INVOKABLE void loadItems();
    Q_INVOKABLE void createItem(const QString &name);
    Q_INVOKABLE void deleteItem(const QString &id);

signals:
    void busyChanged(bool busy);
    void statusChanged(const QString &status);
    void itemsChanged(const QVariantList &items);
    void errorOccurred(const QString &error);

private:
    bool m_busy = false;
    QString m_status;
    QVariantList m_items;
    DataService* m_dataService;

    void setBusy(bool busy);
    void setStatus(const QString &status);
};
```

#### 开发要点
- 继承 QObject 并使用 QML_ELEMENT 宏
- 使用 Q_PROPERTY 定义 QML 属性
- 通过 Q_INVOKABLE 暴露方法
- 使用 Qt 属性系统实现数据绑定
- 在构造函数中初始化依赖服务
- 实现异步操作和错误处理

### 4. Utils 模块开发

Utils 模块提供通用工具函数和跨平台抽象。

#### 文件结构
```
src/utils/
├── FileUtils.h           # 文件操作工具
├── FileUtils.cpp
├── NetworkUtils.h        # 网络工具
├── NetworkUtils.cpp
├── PlatformUtils.h      # 平台抽象
└── PlatformUtils.cpp
```

#### 开发模板
```cpp
// FileUtils.h
#pragma once

#include <QString>
#include <QByteArray>

class FileUtils
{
public:
    static bool readFile(const QString &path, QByteArray &data);
    static bool writeFile(const QString &path, const QByteArray &data);
    static bool ensureDirectory(const QString &path);
    static QString getAppDataDir();
    static QString getConfigDir();
};
```

#### 开发要点
- 提供静态方法，不需要实例化
- 实现跨平台兼容性
- 添加错误处理和日志记录
- 保持函数通用性和可重用性

### 5. Plugin 模块开发

Plugin 模块实现插件系统，支持功能扩展。

#### 文件结构
```
src/plugin/
├── IPlugin.h             # 插件接口
├── BasePlugin.h          # 插件基类
├── PluginManager.h       # 插件管理器
└── specific-plugins/     # 具体插件实现
    ├── SmartLogPlugin/
    └── SamplePlugin/
```

#### 插件开发模板
```cpp
// MyPlugin.h
#pragma once

#include "BasePlugin.h"

class MyPlugin : public BasePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.template.myplugin" FILE "myplugin.json")

public:
    QString name() const override { return "MyPlugin"; }
    QString version() const override { return "1.0.0"; }
    QString description() const override { return "My custom plugin"; }
    QString author() const override { return "Developer"; }

protected:
    bool onInitialize(const QVariantMap &config) override;
    void onShutdown() override;
    bool onSetSettings(const QVariantMap &settings) override;
    QVariantMap onGetSettings() const override;
};
```

#### 开发要点
- 继承 BasePlugin 实现 IPlugin 接口
- 实现完整的生命周期方法
- 使用插件元数据 JSON 文件
- 提供配置管理和错误处理

## 🎨 QML 集成指南

### 1. 使用 ViewModel

```qml
import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: window

    // 使用 ViewModel
    property var featureViewModel: featureViewModel

    Button {
        text: "Load Data"
        onClicked: featureViewModel.loadItems()
    }

    ListView {
        model: featureViewModel.items
        delegate: Text {
            text: model.name
        }
    }

    // 监听状态变化
    Connections {
        target: featureViewModel
        function onBusyChanged(busy) {
            console.log("Busy state:", busy)
        }
    }
}
```

### 2. 属性绑定

```qml
// 绑定 ViewModel 属性
Text {
    text: featureViewModel.status
    visible: !featureViewModel.busy
}

// 条件绑定
Rectangle {
    color: featureViewModel.busy ? "gray" : "white"
}
```

## 🧪 测试开发

### 1. 单元测试

```cpp
// tests/test_featureviewmodel.cpp
#include <QtTest>
#include "business/viewmodels/FeatureViewModel.h"

class TestFeatureViewModel : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testLoadItems();
    void testCreateItem();
    void cleanupTestCase();
};

void TestFeatureViewModel::testLoadItems()
{
    FeatureViewModel viewModel;

    QSignalSpy spy(&viewModel, &FeatureViewModel::itemsChanged);

    viewModel.loadItems();

    QVERIFY(spy.wait(1000));
    QCOMPARE(viewModel.items().size(), > 0);
}
```

### 2. 集成测试

```cpp
// tests/test_integration.cpp
#include <QtTest>
#include "business/viewmodels/FeatureViewModel.h"
#include "data/repositories/DataRepository.h"

class TestIntegration : public QObject
{
    Q_OBJECT

private slots:
    void testViewModelDataIntegration();
};
```

## 🔧 调试技巧

### 1. 日志使用

```cpp
Q_LOGGING_CATEGORY(myCategory, "my.category")

void myFunction() {
    qDebug(myCategory) << "Debug message";
    qCInfo(myCategory) << "Info message";
    qCWarning(myCategory) << "Warning message";
    qCCritical(myCategory) << "Critical message";
}
```

### 2. QML 调试

```qml
// 启用 QML 调试
Button {
    onClicked: {
        console.log("Button clicked")
        console.debug("Debug info:", viewModel.status)
        console.warn("Warning:", viewModel.items.length)
    }
}
```

## 📦 构建配置

### 1. CMake 选项

```bash
# 启用测试
cmake .. -DBUILD_TESTING=ON

# 启用示例
cmake .. -DBUILD_EXAMPLES=ON

# 设置构建类型
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

### 2. 依赖管理

```cmake
# 添加第三方库
add_third_party(mylibrary
    CMAKE_ARGS "-DENABLE_FEATURES=ON"
)
```

## 🚀 部署指南

### 1. 构建发布版本

```bash
mkdir build-release && cd build-release
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### 2. 依赖打包

```bash
# 使用 LinuxDeploy
linuxdeploy --appdir AppDir --executable ./QmlCppTemplate

# 或使用 windeployqt (Windows)
windeployqt QmlCppTemplate.exe
```

## 📚 最佳实践总结

1. **模块化设计**: 每个模块职责单一，接口清晰
2. **MVVM 模式**: 使用 ViewModel 连接 QML 和业务逻辑
3. **错误处理**: 在所有层次实现一致的错误处理
4. **日志记录**: 使用结构化日志便于调试
5. **测试覆盖**: 为关键功能编写测试
6. **代码复用**: 在 Utils 模块实现通用功能
7. **插件化**: 考虑使用插件扩展功能
8. **性能优化**: 避免在主线程进行耗时操作

遵循这些指导原则可以开发出高质量、可维护的 Qt 应用程序。