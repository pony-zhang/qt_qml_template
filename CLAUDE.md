# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

```bash
# Build the project
mkdir build && cd build
cmake ..
make

# Run tests (if available)
cd build
ctest

# Build specific target
cmake --build build --target QmlCppTemplate

# Clean build
rm -rf build
```

## Architecture Overview

This is a Qt QML C++ template project with modular CMake structure following separation of concerns:

### Core Architecture Pattern
- **MVVM Pattern**: Model-View-ViewModel with QML UI and C++ backend
- **Modular Build System**: Each major component has its own CMakeLists.txt
- **Plugin-Based System**: Core application handles system-level plugin management

### Key Module Responsibilities

**Core Module** (`src/core/`):
- Application lifecycle and global state management
- Core services initialization and coordination
- System-wide configuration management
- Central event handling and application coordination

**Data Module** (`src/data/`):
- Data models and persistence layer
- Database operations and data access objects
- Model definitions and data structures
- Data validation and business rules

**Business Module** (`src/business/`):
- Business logic and view models for QML integration
- MVVM pattern implementation as UI backend
- Application workflows and use cases
- UI services: theme management, notifications, URL handling
- Data transformation and business rules processing

**Plugin System** (`src/plugin/`):
- Modular plugin architecture with IPlugin interface
- Plugin loading, initialization, and lifecycle management
- SmartLogPlugin for intelligent logging with zero-overhead filtering
- SamplePlugin demonstrating plugin development patterns
- Plugin configuration and metadata management

**Utils Module** (`src/utils/`):
- Utility functions and common helpers
- Cross-platform abstractions
- Shared algorithms and data structures
- System integration helpers

### Plugin Architecture

The plugin system uses a two-layer approach:
1. **Core/Application**: Handles system-level plugin management
2. **Plugin Modules**: Self-contained functionality with IPlugin interface

**Plugin Management Flow**:
- `Application` class provides plugin management APIs
- `PluginManager` handles plugin loading and lifecycle
- `PluginLoader` manages plugin system initialization
- Plugins inherit from `BasePlugin` which implements `IPlugin`

### Build System Structure

The project uses hierarchical CMake organization:
- Root CMakeLists.txt configures Qt and main executable
- `src/CMakeLists.txt` orchestrates module libraries with clean architecture
- Each module (`core/`, `data/`, `business/`, `plugin/`, `utils/`) has its own CMakeLists.txt
- Plugin modules built as shared libraries for dynamic loading in `plugins/` directory
- Third-party dependencies managed in `dependencies/` directory with auto-discovery

### Important Dependencies

- **Qt 6.2+**: Core, Quick, QuickControls2, Widgets, Test
- **CMake 3.21+**: Modern CMake with Qt integration and hierarchical builds
- **C++17**: Modern C++ features and Qt integration
- **Third-party libraries**: Auto-discovered in `dependencies/` directory (CMake/QMake projects)

### Testing Framework

Uses Qt Test framework with tests in `tests/` directory:
- Unit tests for individual components
- Integration tests for module interactions
- CTest for test execution

### Development Notes

- QML components should delegate business logic to C++ backend through ViewModels
- Use Qt's signal-slot mechanism for cross-layer communication between modules
- Logging uses SmartLogPlugin with automatic categorization and zero-overhead filtering
- Theme switching and UI state managed through business layer ViewModels
- Plugin metadata stored in JSON files alongside plugin libraries
- Third-party libraries can be added to `dependencies/` directory with automatic CMake/QMake detection
- Follow MVVM pattern: ViewModels in business layer, Models in data layer, Views in QML
- Use dependency injection and interfaces for loose coupling between modules

## Coding Standards and Preferences

### Documentation
- **README Synchronization**: When major modular functionality is added, removed, or modified, README files in first level directories must be updated accordingly
- **README Index**: Main project README provides overview and navigation to module-specific documentation
  - [README.md](README.md)
  - [dependencies/README.md](dependencies/README.md)
  - [src/README.md](src/README.md)
  - [qml/README.md](qml/README.md)

### Logging
- **Disable `Q_DECLARE_LOGGING_CATEGORY()`**: Do not use Qt logging categories
- **Use Qt's native output functions**: Use Qt's native output functions directly:
