# 常见问题解答 (FAQ)

本文档收集了项目开发过程中的常见问题和解决方案。

## 🚀 项目设置和构建

### Q: 如何开始一个新项目？

**A:**
1. 克隆项目模板
2. 运行 `mkdir build && cd build && cmake .. && make`
3. 参考 [开发指南](./DEVELOPMENT.md) 了解项目结构

```bash
git clone <repository-url>
cd project-name
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Q: CMake 配置失败怎么办？

**A:** 常见解决方案：

1. **检查 Qt 安装**:
```bash
qmake --version
# 确保 Qt 6.2+ 已安装
```

2. **指定 Qt 路径**:
```bash
cmake .. -DQt6_DIR=/path/to/qt6/lib/cmake/Qt6
```

3. **安装依赖**:
```bash
# Ubuntu/Debian
sudo apt-get install build-essential qt6-base-dev qt6-declarative-dev

# macOS
brew install qt6 cmake
```

### Q: 如何添加新的模块？

**A:** 参考 [架构指南](./ARCHITECTURE.md#-新功能开发指南)：

1. 在相应目录创建文件
2. 更新 `src/CMakeLists.txt`
3. 在模块中创建 `CMakeLists.txt`
4. 更新 `CLAUDE.md`

## 🏗️ 架构和设计

### Q: 为什么要移除 presentation 层？

**A**:
- 避免功能重复（Business 层的 ViewModel 已经提供 UI 后端功能）
- 简化架构，使 Business 层直接与 QML 对话
- 符合标准 MVVM 模式，减少不必要的层次转换
- 提高代码可维护性

### Q: 各模块的职责是什么？

**A**: 简要总结：
- **Core**: 应用程序生命周期、全局状态、系统服务
- **Data**: 数据模型、持久化、数据验证
- **Business**: 业务逻辑、ViewModels、UI 后端服务
- **Utils**: 通用工具、跨平台抽象
- **Plugin**: 插件系统、功能扩展

### Q: 如何实现模块间通信？

**A**:
- **Business → Data**: 直接调用接口
- **QML → Business**: 通过 ViewModel 调用
- **插件间通信**: 使用信号槽或事件总线
- **跨模块事件**: 使用 Qt 信号槽机制

## 🔌 插件开发

### Q: 如何创建一个新的插件？

**A**: 参考 [插件开发指南](./PLUGIN_GUIDE.md)：

1. 创建插件类继承 `BasePlugin`
2. 实现必需的方法（name、version、description、author）
3. 创建插件元数据 JSON 文件
4. 配置 CMake 构建文件
5. 注册到插件系统

### Q: 插件加载失败怎么办？

**A**: 检查清单：
1. 插件文件是否在 `plugins/` 目录
2. 插件是否实现了 `IPlugin` 接口
3. 插件元数据 JSON 文件是否正确
4. 依赖库是否满足
5. 查看日志输出中的具体错误信息

### Q: 如何在 QML 中使用插件？

**A**:
1. 创建 QML 控制器类
2. 在控制器中获取插件实例
3. 通过 `Q_INVOKABLE` 方法暴露插件功能
4. 在 QML 中使用控制器

```cpp
// 控制器示例
Q_INVOKABLE void callPluginMethod() {
    auto* plugin = PluginManager::instance()->getPlugin("MyPlugin");
    if (plugin) {
        // 调用插件方法
    }
}
```

## 📦 第三方依赖

### Q: 如何添加第三方库？

**A**: 参考 [构建系统指南](./BUILD_SYSTEM.md#-第三方依赖管理)：

**自动发现方式**：
```bash
# CMake 项目
git clone https://github.com/example/library.git dependencies/my-library

# QMake 项目
cp -r /path/to/qmake-project dependencies/my-qmake-project
```

**手动配置方式**：
```cmake
add_prebuilt_third_party(mylib
    LIBRARY_PATH prebuilt/libmylib.a
    INCLUDE_DIR prebuilt/include
)
```

### Q: CMake 项目和 QMake 项目有什么区别？

**A**:
- **CMake 项目**: 自动检测 `CMakeLists.txt`，独立构建安装
- **QMake 项目**: 自动检测 `.pro` 文件，原地构建，复制库文件
- **预编译库**: 直接使用已编译的库文件

### Q: 如何处理依赖冲突？

**A**:
1. 使用不同版本的库在不同目录
2. 修改构建顺序确保正确的链接顺序
3. 使用符号链接或别名
4. 在 CMake 中设置不同的命名空间

## 🎨 QML 开发

### Q: ViewModel 如何与 QML 集成？

**A**:
1. 在 ViewModel 中使用 `QML_ELEMENT` 宏
2. 定义 `Q_PROPERTY` 属性
3. 添加 `Q_INVOKABLE` 方法
4. 在 QML 中直接使用

```qml
// QML 中使用
import QtQuick 2.15

ApplicationWindow {
    MyViewModel {
        id: viewModel
    }

    Text {
        text: viewModel.status
    }

    Button {
        onClicked: viewModel.performAction("hello")
    }
}
```

### Q: 数据如何在 QML 和 C++ 间传递？

**A**: 使用 Qt 的元类型系统：
- **基本类型**: int, QString, bool, double
- **容器**: QVariantList, QVariantMap
- **自定义类型**: 使用 `Q_DECLARE_METATYPE` 注册

```cpp
// 注册自定义类型
Q_DECLARE_METATYPE(UserInfo)

// 在 ViewModel 中返回
Q_INVOKABLE QVariantList getUserList();
```

### Q: 如何处理 QML 中的异步操作？

**A**:
1. 在 ViewModel 中使用 `QFuture` 或 `QThread`
2. 通过信号槽通知 QML 完成状态
3. 使用加载状态指示器

```cpp
class MyViewModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)

public:
    Q_INVOKABLE void loadAsync() {
        setBusy(true);
        QtConcurrent::run([this]() {
            // 耗时操作
            QMetaObject::invokeMethod(this, "onLoadCompleted",
                                      Qt::QueuedConnection);
        });
    }

signals:
    void busyChanged(bool busy);
    void loadCompleted(const QVariant& result);

private:
    void onLoadCompleted() {
        setBusy(false);
        emit loadCompleted(result);
    }
};
```

## 🧪 测试和调试

### Q: 如何编写单元测试？

**A**: 参考 [开发指南](./DEVELOPMENT.md#-测试开发)：

```cpp
#include <QtTest>
#include "MyClass.h"

class TestMyClass : public QObject {
    Q_OBJECT

private slots:
    void testBasicFunctionality();
};

void TestMyClass::testBasicFunctionality() {
    MyClass myClass;
    QVERIFY(myClass.isValid());
    QCOMPARE(myClass.name(), QString("default"));
}

QTEST_APPLESS_MAIN(TestMyClass)
#include "TestMyClass.moc"
```

### Q: 如何调试插件加载问题？

**A**:
1. 启用详细日志：
```bash
export QT_LOGGING_RULES="plugin.debug=true"
```

2. 在插件构造函数中添加调试信息：
```cpp
MyPlugin::MyPlugin(QObject *parent) : BasePlugin(parent) {
    qDebug() << "MyPlugin loading...";
}
```

3. 检查插件依赖和路径

### Q: 内存泄漏如何检测？

**A**:
1. 使用 Valgrind (Linux)：
```bash
valgrind --leak-check=full ./your-application
```

2. 使用 AddressSanitizer：
```bash
cmake .. -DCMAKE_CXX_FLAGS="-fsanitize=address -g"
```

3. Qt 内存检查工具：
```cpp
// 在 main.cpp 中
qInstallMessageHandler(myMessageHandler);
```

## 🌍 跨平台问题

### Q: Windows 上构建失败怎么办？

**A**: 常见解决方案：
1. **Qt 路径问题**：
```bash
cmake .. -DQt6_DIR=C:/Qt/6.5.0/msvc2019_64/lib/cmake/Qt6
```

2. **依赖缺失**：使用 vcpkg 安装依赖
3. **链接问题**：检查库文件路径和顺序

### Q: macOS 上应用打包问题？

**A**:
1. 使用 `macdeployqt`：
```bash
macdeployqt QmlCppTemplate.app -dmg
```

2. 手动添加框架和资源
3. 检查 Info.plist 配置

### Q: Linux 上依赖问题？

**A**:
1. 安装开发包：
```bash
sudo apt-get install libqt6core6 libqt6gui6 libqt6widgets6
```

2. 使用 AppImage 或 Flatpak 打包
3. 检查库路径和 rpath

## 📊 性能优化

### Q: 应用启动慢怎么办？

**A**: 优化建议：
1. 延迟加载非关键资源
2. 减少启动时的插件加载
3. 优化 QML 加载时间
4. 使用 Qt Quick Compiler

### Q: UI 响应慢如何处理？

**A**:
1. 将耗时操作移到工作线程
2. 使用 `QtConcurrent` 进行异步处理
3. 优化数据模型和委托
4. 减少信号槽连接的复杂度

### Q: 内存使用过高怎么办？

**A**:
1. 检查是否有内存泄漏
2. 优化数据结构和算法
3. 使用对象池和缓存
4. 及时释放不再使用的资源

## 🔧 工具和环境

### Q: 推荐的开发环境是什么？

**A**:
- **IDE**: Qt Creator 或 VS Code + C++ 扩展
- **构建工具**: CMake 3.21+, Ninja
- **调试工具**: GDB, LLDB, Qt Creator Debugger
- **代码检查**: clang-format, clang-tidy

### Q: 如何配置代码格式化？

**A**: 创建 `.clang-format` 文件：
```yaml
BasedOnStyle: LLVM
IndentWidth: 4
ColumnLimit: 100
AllowShortIfStatementsOnASingleLine: false
```

### Q: Git 工作流程建议？

**A**:
1. 创建功能分支：`git checkout -b feature/new-feature`
2. 小步提交，清晰信息
3. 推送前运行测试
4. 使用 Pull Request 进行代码审查

## 📚 文档和社区

### Q: 如何贡献代码？

**A**:
1. Fork 项目仓库
2. 创建功能分支
3. 遵循代码规范
4. 添加测试和文档
5. 提交 Pull Request

### Q: 文档更新流程？

**A**:
1. 在 `docs/` 目录中修改文档
2. 确保信息准确和完整
3. 提交时包含文档变更
4. 通知团队审查

### Q: 获取技术支持的途径？

**A**:
1. 查看项目文档和 FAQ
2. 搜索 Issue Tracker
3. 创建新的 Issue 描述问题
4. 联系项目维护者

---

如果本文档没有解答您的问题，请：

1. 查看项目中的其他文档
2. 搜索项目的 Issue Tracker
3. 创建新的 Issue 描述具体问题
4. 联系项目技术负责人