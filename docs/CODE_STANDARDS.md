# 代码规范指南

## 🎯 编码原则

本项目遵循现代 C++ 开发最佳实践，结合 Qt 框架特性，确保代码质量和可维护性。

### 核心原则

- **可读性优先**: 代码应该易于理解和维护
- **一致性**: 遵循统一的编码风格和模式
- **安全性**: 避免常见的 C++ 安全隐患
- **性能**: 在不影响可读性的前提下优化性能
- **现代 C++**: 使用 C++17 特性和最佳实践

## 📝 命名约定

### 文件命名

```cpp
// 类文件：PascalCase
MyClass.h
MyClass.cpp

// 测试文件：Test + 类名
TestMyClass.cpp

// 工具文件：功能描述
FileUtils.h
NetworkUtils.cpp
```

### 类和类型命名

```cpp
// 类名：PascalCase
class ApplicationManager;
class DataViewModel;

// 结构体：PascalCase
struct UserInfo;
struct ConfigSettings;

// 枚举类：PascalCase
enum class LogLevel;
enum class HttpStatus;
```

### 函数和方法命名

```cpp
// 普通函数：camelCase
void calculateTotal();
QString formatMessage();

// 类方法：camelCase
void initialize();
bool isValid() const;

// 私有成员：m_camelCase
class MyClass {
private:
    QString m_name;
    int m_value;
};

// 静态变量：s_camelCase
static Logger* s_instance;
```

### 变量和常量命名

```cpp
// 局部变量：camelCase
QString userName;
int itemCount;

// 常量：kPascalCase 或 UPPER_SNAKE_CASE
const int kMaxItems = 100;
const double PI = 3.14159;

// 全局变量：g_camelCase (避免使用)
QString g_applicationPath;

// Qt 属性：camelCase
Q_PROPERTY(QString name READ name WRITE name NOTIFY nameChanged)
```

### 宏定义命名

```cpp
// 宏定义：UPPER_SNAKE_CASE
#define MAX_BUFFER_SIZE 4096
#define VERSION_STRING "1.0.0"

// Qt 信号槽宏：PascalCase

Q_OBJECT
Q_PROPERTY
```

## 🏗️ 代码结构

### 头文件结构

```cpp
#pragma once  // 现代头文件保护

// Qt includes
#include <QObject>
#include <QString>
#include <QLoggingCategory>

// Standard library includes
#include <memory>
#include <vector>

// Forward declarations
class DataService;



/**
 * @brief Brief class description
 *
 * Detailed description of the class purpose,
 * functionality, and usage examples.
 */
class MyClass : public QObject
{
    Q_OBJECT
    QML_ELEMENT  // For QML integration

    Q_PROPERTY(QString name READ name WRITE name NOTIFY nameChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE enabled NOTIFY enabledChanged)

public:
    explicit MyClass(QObject *parent = nullptr);
    ~MyClass();

    // Public methods
    QString name() const;
    bool enabled() const;

    Q_INVOKABLE void performAction(const QString &param);
    Q_INVOKABLE bool validateInput(const QVariant &data);

signals:
    void nameChanged(const QString &name);
    void enabledChanged(bool enabled);
    void actionCompleted(bool success);
    void errorOccurred(const QString &error);

public slots:
    void handleResponse(const QByteArray &data);

private:
    // Private methods
    void initialize();
    void cleanup();
    bool process_data(const QString &input);

    // Private member variables
    QString m_name;
    bool m_enabled;
    DataService* m_dataService;
};
```

### 源文件结构

```cpp
#include "MyClass.h"
#include "DataService.h"

#include <QDebug>
#include <QTimer>

Q_LOGGING_CATEGORY(myCategory, "my.category")

MyClass::MyClass(QObject *parent)
    : QObject(parent)
    , m_enabled(false)
    , m_dataService(new DataService(this))
{
    initialize();
}

MyClass::~MyClass()
{
    cleanup();
}

void MyClass::initialize()
{
    qDebug(myCategory) << "Initializing MyClass";

    // Initialization code
    connect(m_dataService, &DataService::dataReady,
            this, &MyClass::handleResponse);

    m_enabled = true;
    emit enabledChanged(true);
}

QString MyClass::name() const
{
    return m_name;
}

void MyClass::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged(name);
    }
}

void MyClass::performAction(const QString &param)
{
    qDebug(myCategory) << "Performing action with param:" << param;

    if (!validateInput(param)) {
        emit errorOccurred("Invalid input");
        return;
    }

    // Action implementation
    try {
        // Process action
        emit actionCompleted(true);
    } catch (const std::exception& e) {
        qCCritical(myCategory) << "Action failed:" << e.what();
        emit errorOccurred(QString::fromUtf8(e.what()));
    }
}
```

## 🎨 格式化规范

### 缩进和空格

```cpp
// 使用 4 个空格缩进
class MyClass {
public:
    void myMethod() {
        if (condition) {
            // Code here
        } else {
            // Code here
        }
    }
};

// 运算符周围有空格
int result = a + b * c;
bool isValid = (value > 0) && (value < 100);

// 逗号后有空格
void myFunction(int param1, QString param2, double param3);

// 指针和引用：* 和 & 靠近类型名
char* text = "Hello";
const QString& reference = getString();
```

### 括号和换行

```cpp
// 左大括号不换行
if (condition) {
    // Code
}

// 函数声明和定义
void myFunction(int param1, int param2)
{
    // Implementation
}

// 长参数换行
void veryLongFunctionName(int param1,
                         const QString& param2,
                         double param3,
                         bool param4 = false);

// 条件语句换行
if (veryLongConditionThatExceedsLineLength &&
    anotherLongCondition &&
    thirdCondition) {
    // Code
}
```

### 注释规范

```cpp
// 单行注释：解释代码意图
void processData() {
    // Validate input before processing
    if (!validateInput(data)) {
        return;
    }

    // Process data in chunks
    for (const auto& item : data) {
        processItem(item);
    }
}

// 多行注释：详细说明
/**
 * Processes the input data and returns the calculated result.
 *
 * This function performs the following steps:
 * 1. Validates the input parameters
 * 2. Transforms the data structure
 * 3. Calculates the result using the specified algorithm
 *
 * @param input The input data to process
 * @param options Processing options (optional)
 * @return The calculated result
 * @throws InvalidInputException If input validation fails
 */
ResultType processData(const InputType& input,
                      const Options& options = Options::Default());
```

## 🛡️ 安全性规范

### 内存管理

```cpp
// 使用智能指针
std::unique_ptr<MyClass> ptr = std::make_unique<MyClass>();
std::shared_ptr<Resource> resource = std::make_shared<Resource>();

// Qt 对象树管理
MyClass* obj = new MyClass(parent);  // parent 负责删除

// 避免裸指针
void badFunction() {
    MyClass* ptr = new MyClass();  // 内存泄漏
}

void goodFunction() {
    auto ptr = std::make_unique<MyClass>();  // 自动管理
}
```

### 错误处理

```cpp
// 使用异常处理
void riskyOperation() {
    try {
        performRiskyOperation();
    } catch (const std::exception& e) {
        qWarning() << "Operation failed:" << e.what();
        emit errorOccurred(e.what());
    }
}

// 返回错误码
ResultType operationWithResult() {
    if (!validateInput()) {
        return ResultType::InvalidInput;
    }

    // Process and return success
    return ResultType::Success;
}

// Qt 风格错误处理
bool processFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << filePath;
        return false;
    }

    // Process file
    return true;
}
```

### 输入验证

```cpp
// 验证函数参数
void processUserInput(const QString& input) {
    if (input.isEmpty()) {
        qWarning() << "Empty input provided";
        return;
    }

    if (input.length() > kMaxInputLength) {
        qWarning() << "Input too long:" << input.length();
        return;
    }

    // Process valid input
}

// 使用 Qt 的类型系统
void setAge(int age) {
    if (age < 0 || age > 150) {
        qWarning() << "Invalid age:" << age;
        return;
    }

    m_age = age;
}
```

## ⚡ 性能优化

### 算法和数据结构

```cpp
// 使用合适的容器
std::vector<int> numbers;              // 连续内存，快速访问
std::map<QString, QString> config;   // 有序键值对
std::unordered_map<QString, int> cache; // 哈希表，快速查找

// 预分配空间
void reserveSpace(int expectedSize) {
    std::vector<int> data;
    data.reserve(expectedSize);  // 避免多次重新分配
}

// 使用移动语义
std::vector<int> createLargeVector() {
    std::vector<int> result(1000000);
    // Fill vector
    return result;  // 移动语义，避免拷贝
}
```

### Qt 性能最佳实践

```cpp
// 使用信号槽的连接类型
// 自动连接 (推荐)
connect(sender, &Sender::signal, receiver, &Receiver::slot);

// 直接连接（线程安全时性能更好）
connect(sender, &Sender::signal, receiver, &Receiver::slot,
        Qt::DirectConnection);

// 队列连接（跨线程）
connect(sender, &Sender::signal, receiver, &Receiver::slot,
        Qt::QueuedConnection);

// 避免在主线程进行耗时操作
void processData() {
    // 使用 QThread 或 QtConcurrent
    QtConcurrent::run([this]() {
        // 耗时操作
        QMetaObject::invokeMethod(this, "updateUI",
                                  Qt::QueuedConnection);
    });
}

// 使用 QML 友好的数据类型
QVariantList getListData() const {
    QVariantList result;
    for (const auto& item : m_items) {
        result.append(QVariant::fromValue(item));
    }
    return result;
}
```

## 🧪 测试规范

### 单元测试结构

```cpp
#include <QtTest>
#include "MyClass.h"

class TestMyClass : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testInitialization();
    void testNameProperty();
    void testValidation_data();
    void testValidation();
    void testErrorHandling();
};

void TestMyClass::testInitialization()
{
    MyClass myClass;
    QVERIFY(myClass.name().isEmpty());
    QVERIFY(myClass.enabled());
}

void TestMyClass::testValidation_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<bool>("expected");

    QTest::newRow("valid") << "valid_input" << true;
    QTest::newRow("empty") << "" << false;
    QTest::newRow("too_long") << QString("x").repeat(1000) << false;
}

void TestMyClass::testValidation()
{
    QFETCH(QString, input);
    QFETCH(bool, expected);

    MyClass myClass;
    QCOMPARE(myClass.validateInput(input), expected);
}
```

## 📋 代码检查清单

### 提交前检查

- [ ] 代码编译无错误和警告
- [ ] 遵循命名约定和格式化规范
- [ ] 单元测试覆盖新增功能
- [ ] 内存管理正确（无泄漏）
- [ ] 错误处理完善
- [ ] 文档注释完整
- [ ] Qt 对象和智能指针使用正确
- [ ] 信号槽连接安全

### 架构检查

- [ ] 遵循分层架构原则
- [ ] 依赖关系正确
- [ ] 接口设计清晰
- [ ] 模块职责单一
- [ ] 插件接口标准

### 性能检查

- [ ] 避免不必要的拷贝
- [ ] 使用合适的数据结构和算法
- [ ] Qt 信号槽连接类型正确
- [ ] 耗时操作异步处理
- [ ] 资源使用合理

## 🛠️ 工具和配置

### Qt Creator 配置

```json
// .qtcreator/tools/clang-format
{
    "Language": "Cpp",
    "BasedOnStyle": "LLVM",
    "IndentWidth": 4,
    "TabWidth": 4,
    "UseTab": "Never",
    "ColumnLimit": 100,
    "AllowShortIfStatementsOnASingleLine": false,
    "IndentCaseLabels": true,
    "SortIncludes": true
}
```

### Git 钩子

```bash
#!/bin/bash
# .git/hooks/pre-commit

# 格式检查
if command -v clang-format &> /dev/null; then
    clang-format --dry-run --Werror $(git diff --cached --name-only --diff-filter=ACM | grep -E '\.(cpp|h)$')
    if [ $? -ne 0 ]; then
        echo "Code formatting issues found. Please run clang-format."
        exit 1
    fi
fi

# 构建检查
mkdir -p build-check
cd build-check
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc) 2>&1 | grep -i "error\|warning"
```

遵循这些代码规范，可以确保项目代码的一致性、可读性和可维护性。