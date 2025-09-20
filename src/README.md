# Source Code Architecture

This directory contains the C++ source code organized into clean architecture layers with clear separation of concerns.

## Directory Structure

```
├── src/                   # C++ source code
│   ├── core/              # Core application layer
│   │   ├── Application lifecycle and plugin management
│   │   ├── QML type registration system
│   │   ├── Resource management system
│   │   └── Core module build configuration
│   ├── business/          # Business logic layer
│   │   ├── Pure business logic services
│   │   ├── MVVM ViewModels for QML integration
│   │   └── Business module build configuration
│   ├── data/              # Data layer
│   │   ├── Pure data models and entities
│   │   ├── Data access and repositories
│   │   └── Data module build configuration
│   ├── plugin/            # Plugin system
│   │   ├── Plugin interfaces and base implementations
│   │   ├── Plugin management and loading
│   │   ├── Smart logging plugin system with zero-overhead filtering
│   │   └── Plugin module build configuration
│   ├── utils/             # Utility classes
│   ├── Main source build configuration
│   └── Application entry point
```

## Architecture Layers

### Core Layer (`core/`)
The foundation layer providing application-level services:
- **Application**: Manages application lifecycle, settings, and global state
- **QmlTypeRegistry**: Handles QML type registration for C++ components
- **ResourceManager**: Manages application resources and assets

### Business Layer (`business/`)
Contains business logic and MVVM pattern implementation:
- **ViewModels**: Bridge between QML UI and business logic
  - `AppViewModel`: Theme management, notifications, UI coordination
  - `DataViewModel`: Data presentation logic and user interactions
- **Services**: Pure business logic implementation
  - `DataService`: Data validation, processing, and business rules

### Data Layer (`data/`)
Handles data models and persistence:
- **Models**: Pure data structures and domain entities
- Data access and repository pattern implementation

### Plugin System (`plugin/`)
Modular architecture for extensible functionality:
- **PluginLoader**: Plugin discovery, loading, and initialization
- **PluginManager**: Plugin lifecycle and coordination
- **BasePlugin**: Base implementation for plugin development
- **Interfaces**: Plugin contract definitions

### Utils (`utils/`)
Shared utility functions and helpers:
- Cross-platform abstractions
- Common algorithms and data structures
- System integration helpers

## Build System

The source code uses a hierarchical CMake structure:

### Root Level (`src/CMakeLists.txt`)
- Orchestrates all module libraries
- Defines module dependencies following clean architecture
- Links modules to main executable

### Module Level (each subdirectory)
- Builds individual module libraries
- Defines module-specific dependencies
- Configures compilation settings

### Main Entry Point (`main.cpp`)
- Application initialization
- Core services setup
- QML engine configuration
- Main event loop execution

## Key Features

- **Clean Architecture**: Dependency flows from presentation � business � data � core
- **MVVM Pattern**: ViewModels bridge QML views with business services
- **Plugin System**: Modular architecture with dynamic loading
- **Modern CMake**: Hierarchical build with clear module separation
- **Qt Integration**: Seamless QML/C++ integration with proper type registration

## Development Guidelines

- Follow dependency direction: business depends on core, not vice versa
- Use Qt's signal-slot mechanism for cross-layer communication
- Implement proper error handling and logging
- Keep business logic pure and testable
- Use dependency injection for service management
- Maintain clear separation between UI logic and business rules

## Integration with QML

ViewModels from the business layer are automatically registered as QML types and made available to the QML frontend through the QmlTypeRegistry system, enabling reactive UI updates and clean separation between presentation and business logic.