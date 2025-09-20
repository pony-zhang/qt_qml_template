# QML C++ Template Project

A modern Qt QML C++ template project following clean architecture and MVVM pattern for enterprise-scale development.

## Features

- **Clean Architecture**: Layered architecture with clear separation of concerns
- **MVVM Pattern**: Model-View-ViewModel implementation with QML integration
- **Modern CMake**: Hierarchical build configuration with Qt 6 support
- **Business Logic Layer**: Dedicated services and viewmodels for business rules
- **Data Layer**: Structured data models with repository pattern support
- **Plugin System**: Modular plugin architecture with dynamic loading
- **Logging System**: Intelligent logging with zero-overhead filtering
- **Theme Support**: Dark/Light theme switching with backend integration
- **Cross-platform**: Windows, macOS, and Linux support

## Project Structure

```
├── qml/                   # QML UI files (top-level for easy access)
│   ├── components/        # Reusable QML components
│   ├── pages/             # QML pages/screens
│   ├── theme/             # Theme definitions
│   ├── assets/            # Static assets (images, etc.)
│   ├── main.qml           # Main application entry point
│   └── qml.qrc            # QML resource configuration
├── src/                   # C++ source code
│   ├── core/              # Core application layer
│   │   ├── Application.h/cpp    # Application lifecycle & plugin management
│   │   └── CMakeLists.txt        # Core module build config
│   ├── business/          # Business logic layer
│   │   ├── services/           # Pure business logic (DataService)
│   │   ├── viewmodels/         # MVVM ViewModels (AppViewModel, DataViewModel)
│   │   └── CMakeLists.txt        # Business module build config
│   ├── data/              # Data layer
│   │   ├── models/             # Pure data models (DataModel)
│   │   ├── repositories/       # Data access layer (extensible)
│   │   └── CMakeLists.txt        # Data module build config
│   ├── presentation/       # Presentation layer
│   │   ├── delegates/          # Simple UI delegates (moved from ui/backend)
│   │   └── CMakeLists.txt        # Presentation module build config
│   ├── plugin/            # Plugin system
│   │   ├── IPlugin.h/BasePlugin.h   # Plugin interfaces
│   │   ├── PluginManager.h/cpp     # Plugin management
│   │   ├── log/SmartLogPlugin/      # Example plugin
│   │   └── CMakeLists.txt          # Plugin module build config
│   └── utils/             # Utility classes
├── tests/                 # Unit and integration tests
├── cmake/                 # CMake configuration files
├── dependencies/          # Third-party libraries
└── docs/                  # Documentation
```

## Prerequisites

- Qt 6.2 or later
- CMake 3.21 or later
- C++17 compatible compiler

## Building

### Using CMake directly:

```bash
mkdir build
cd build
cmake ..
make
```

### Using Qt Creator:

1. Open CMakeLists.txt in Qt Creator
2. Configure the project
3. Build and run

## Architecture Overview

### Clean Architecture Layers

**Core Layer** (`src/core/`)
- Application lifecycle and global state management
- Plugin system integration and management
- System-wide configuration and coordination

**Business Layer** (`src/business/`)
- **Services**: Pure business logic and domain rules
- **ViewModels**: MVVM pattern implementation for QML integration
- Data transformation and business workflow processing

**Data Layer** (`src/data/`)
- **Models**: Pure data structures and domain entities
- **Repositories**: Data access and persistence (extensible)
- Data validation and business rule enforcement

**QML Layer** (`qml/`)
- **Components**: Reusable QML UI components
- **Pages**: Application screens and views
- **Theme**: Theme definitions and styling
- **Assets**: Static assets and resources

**Presentation Layer** (`src/presentation/`)
- **Delegates**: Simple UI interaction handlers
- UI state coordination and backend integration

### Development Guidelines

#### Architecture Principles
- **Dependency Direction**: Presentation → Business → Data → Core
- **Separation of Concerns**: Each layer has distinct responsibilities
- **MVVM Pattern**: ViewModels bridge QML views with business services
- **Interface Segregation**: Use interfaces for loose coupling between modules

#### C++ Backend
- Use Qt's signal-slot mechanism for cross-layer communication
- Implement proper error handling and logging
- Follow RAII principles and modern C++ practices
- Use dependency injection for service management
- Keep business logic pure and testable

#### QML Frontend
- Delegate all business logic to C++ ViewModels
- Use property bindings for reactive UI updates
- Implement reusable components with clear interfaces
- Follow Qt Quick and Material Design guidelines
- Keep QML files focused on presentation only

#### Build System
- Hierarchical CMake structure with clear module dependencies
- Use `qt_add_resources` for QML resource management
- Implement proper dependency management and library linking
- Configure cross-platform build settings and deployment
- Enable unit testing with CTest integration

## Key Components

### AppViewModel
Main QML integration point:
- Theme management and UI coordination
- Toast notifications and user feedback
- URL handling and utility functions
- UI state management

### DataViewModel
Data presentation logic:
- Wraps DataModel for QML consumption
- Provides business logic for data operations
- Handles user interactions and validation
- Emits signals for UI updates

### DataService
Pure business logic:
- Data validation and business rule enforcement
- Data processing and transformation
- Filtering, sorting, and search operations
- Business workflow implementation

### Application
Core application management:
- Settings management and configuration
- Plugin system coordination
- Application lifecycle control
- Global event handling

### DataModel
Pure data model:
- Reactive data structure with Qt model interface
- CRUD operations and change notifications
- Role-based data access for QML integration
- Serialization and data persistence support

## Testing

Run tests with:
```bash
cd build
ctest
```

## QML Integration

### Registering ViewModels
ViewModels are automatically registered as QML elements and exposed to QML:

```qml
// In QML files
import QtQuick
import QtQuick.Controls

ApplicationWindow {
    // Access ViewModels through context properties
    property var appViewModel: appViewModel
    property var dataViewModel: dataViewModel

    // Use ViewModel functions
    Button {
        text: "Add Item"
        onClicked: dataViewModel.addItem("Test", "Description", 42)
    }
}
```

### Plugin Development
The plugin system supports dynamic loading and configuration:

```cpp
// Plugin example
class MyPlugin : public BasePlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.template.myplugin" FILE "myplugin.json")

public:
    QString name() const override { return "MyPlugin"; }
    QString version() const override { return "1.0.0"; }
    QString description() const override { return "Example plugin"; }

protected:
    bool onInitialize(const QVariantMap &config) override {
        // Plugin initialization logic
        return true;
    }

    void onShutdown() override {
        // Plugin cleanup
    }
};
```

## Contributing

1. **Follow Architecture Principles**: Maintain clean separation between layers
2. **Use MVVM Pattern**: Implement ViewModels for QML integration
3. **Add Logging**: Use the SmartLogPlugin for comprehensive logging
4. **Implement Error Handling**: Add proper error handling and validation
5. **Write Tests**: Create unit and integration tests for new features
6. **Update Documentation**: Keep README and code comments current

## License

This template is provided as-is for educational and development purposes.