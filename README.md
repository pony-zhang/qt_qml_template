# QML C++ Template Project

A modern Qt QML C++ template project following best practices for medium to large-scale development.

## Features

- **Separation of Concerns**: Clear separation between QML UI and C++ backend
- **Modern CMake**: Professional build configuration with Qt 6 support
- **MVVM Architecture**: Model-View-ViewModel pattern implementation
- **Modular Structure**: Organized directory structure for scalability
- **Logging System**: Comprehensive logging with category-based filtering
- **Theme Support**: Dark/Light theme switching
- **Cross-platform**: Windows, macOS, and Linux support

## Project Structure

```
├── src/                    # C++ source code
│   ├── core/              # Core application classes
│   ├── ui/                # UI-related C++ code
│   │   └── backend/      # QML backend implementations
│   ├── models/            # Data models
│   └── utils/             # Utility classes
├── resources/qml/         # QML resources
│   ├── components/        # Reusable QML components
│   ├── pages/             # QML pages/screens
│   └── theme/             # Theme definitions
├── tests/                 # Unit and integration tests
├── cmake/                 # CMake configuration files
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

## Development Guidelines

### C++ Backend
- Use Qt's signal-slot mechanism for communication
- Implement proper error handling
- Follow RAII principles
- Use smart pointers where appropriate

### QML Frontend
- Keep business logic in C++
- Use property bindings for reactive UI
- Implement reusable components
- Follow Material Design guidelines

### Build System
- Use `qt_add_resources` for QML resources
- Implement proper dependency management
- Configure cross-platform build settings
- Enable unit testing

## Key Components

### UiBackend
Main interface between QML and C++:
- Theme management
- Notifications
- Utility functions
- Global settings

### DataModel
Reactive data model with:
- CRUD operations
- Change notifications
- Role-based data access
- Serialization support

### Application
Core application class providing:
- Settings management
- Logging configuration
- Application lifecycle management

## Testing

Run tests with:
```bash
cd build
ctest
```

## Contributing

1. Follow the established code structure
2. Add appropriate logging
3. Implement proper error handling
4. Write tests for new features
5. Update documentation

## License

This template is provided as-is for educational and development purposes.