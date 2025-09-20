# CMake Configuration

This directory contains CMake configuration files for the QML C++ Template project, supporting cross-platform builds, packaging, and installation.

## 📁 File Structure

```
cmake/
├── BuildOptions.cmake          # Build type configurations and compiler flags
├── PlatformConfig.cmake        # Platform-specific settings
├── Dependencies.cmake          # Qt and third-party dependencies
├── BuildPresets.cmake          # Utility functions and macros
├── Packaging.cmake              # Installation and packaging
├── QmlCppTemplateConfig.cmake  # Package configuration
├── Info.plist.in               # macOS app bundle template
├── templates/
│   └── BuildInfo.h.in         # Build information template
└── README.md                   # This documentation
```

## 🚀 Quick Start

### Basic Build
```bash
mkdir build && cd build
cmake ..
make
```

### Different Build Types
```bash
# Debug build (default)
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build
cmake -DCMAKE_BUILD_TYPE=Release ..

# Release with debug info
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..

# Minimum size release
cmake -DCMAKE_BUILD_TYPE=MinSizeRel ..
```

### Build with Additional Features
```bash
# Build with tests
cmake -DBUILD_TESTING=ON ..

# Build with examples
cmake -DBUILD_EXAMPLES=ON ..

# Disable installation
cmake -DENABLE_INSTALL=OFF ..
```

## 🖥️ Platform-Specific Configuration

### Windows
```bash
# Use Visual Studio 2022
cmake -G "Visual Studio 17 2022" -A x64 ..

# Set Windows SDK version
cmake -DCMAKE_SYSTEM_VERSION=10.0.19041.0 ..

# Generate NSIS installer
cmake -DCPACK_GENERATOR=NSIS ..
```

### macOS
```bash
# Set minimum macOS version
cmake -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0 ..

# Build as app bundle (development mode disabled)
cmake -DMACOSX_BUNDLE=ON ..

# Generate Drag & Drop package
cmake -DCPACK_GENERATOR=DragNDrop ..
```

### Linux
```bash
# Custom install prefix
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..

# Generate Debian package
cmake -DCPACK_GENERATOR=DEB ..

# Generate RPM package
cmake -DCPACK_GENERATOR=RPM ..
```

## 📦 Packaging and Installation

### Local Installation
```bash
# Build and install to system
sudo make install

# Install to custom prefix
cmake -DCMAKE_INSTALL_PREFIX=/opt/myapp ..
make install
```

### Package Creation
```bash
# Create platform-specific packages
cpack -G NSIS          # Windows installer
cpack -G DragNDrop     # macOS DMG
cpack -G DEB           # Debian package
cpack -G RPM           # RPM package
cpack -G ZIP           # ZIP archive
```

### Component Installation
```bash
# Install only runtime components
cpack -G NSIS -C Runtime ..

# Install only development components
cpack -G DEB -C Development ..
```

## ⚙️ Configuration Options

| Option | Description | Default | Values |
|--------|-------------|---------|--------|
| `CMAKE_BUILD_TYPE` | Build configuration | `Debug` | `Debug`, `Release`, `RelWithDebInfo`, `MinSizeRel` |
| `BUILD_TESTING` | Build test suite | `OFF` | `ON`, `OFF` |
| `BUILD_EXAMPLES` | Build example applications | `OFF` | `ON`, `OFF` |
| `ENABLE_INSTALL` | Enable installation targets | `ON` | `ON`, `OFF` |
| `CMAKE_INSTALL_PREFIX` | Installation directory | System default | Path |
| `CMAKE_OSX_DEPLOYMENT_TARGET` | Minimum macOS version | `11.0` | Version string |

## 🔧 Available Functions and Macros

### Project Configuration
```cmake
set_project_warnings(target)           # Set compiler warnings
set_project_features(target)           # Set C++17 and project features
setup_qt_resources(target)            # Configure Qt resources
configure_build_info(target)           # Generate build information
```

### Test Creation
```cmake
add_qt_test(test_name
    SOURCES source1.cpp source2.cpp
    LIBRARIES Qt6::Test Qt6::Core
)
```

### Plugin Creation
```cmake
create_plugin(plugin_name
    SOURCES plugin.cpp plugin.h
    LIBRARIES Qt6::Core Qt6::Quick
)
```

### Example Creation
```cmake
add_example(example_name
    SOURCES main.cpp
    LIBRARIES QmlCppTemplate::core QmlCppTemplate::data
)
```

## 📋 Build Output

### Directory Structure
```
build/
├── bin/                      # Executables
│   ├── QmlCppTemplate        # Main application
│   ├── tests/               # Test executables (if BUILD_TESTING=ON)
│   └── examples/            # Example applications (if BUILD_EXAMPLES=ON)
├── lib/                      # Static libraries
│   ├── libcore.a
│   ├── libdata.a
│   ├── libbusiness.a
│   ├── libutils.a
│   └── libplugin.a
├── plugins/                  # Plugin libraries
│   └── libSmartLogPlugin.dylib
├── include/                  # Generated headers
│   └── BuildInfo.h
└── packages/                 # Generated packages (after cpack)
```

### Build Information
The build system automatically generates `BuildInfo.h` with:
- Project name and version
- Build type and timestamp
- Compiler and system information
- Git commit and branch (if available)

## 🏗️ Architecture

### Core Modules

```
cmake/
├── BuildOptions.cmake      # Build configuration and compiler flags
├── PlatformConfig.cmake    # Platform-specific settings
├── Dependencies.cmake      # Qt and third-party dependency management
├── BuildPresets.cmake      # Utility functions and macros
├── Packaging.cmake         # Installation and packaging
└── QmlCppTemplateConfig.cmake  # Package export configuration
```

### Module Dependencies
```
CMakeLists.txt → BuildOptions.cmake → PlatformConfig.cmake
                ↓ Dependencies.cmake → BuildPresets.cmake
                ↓ Packaging.cmake
```

### Build Flow
1. **Configuration**: Set build type, platform settings, dependencies
2. **Generation**: Process Qt files, generate build info, configure installation
3. **Build**: Compile sources, link libraries, create executables
4. **Package** (optional): Create installers and distribution packages

## 🎯 Development Features

### Code Quality
- Comprehensive compiler warnings for all platforms
- Static analysis integration
- Position independent code
- Link Time Optimization (release builds)

### Qt Integration
- Automatic MOC, RCC, UIC processing
- Qt 6.2+ compatibility
- Platform-specific Qt configurations

### Cross-Platform Support
- Windows: MSVC, MinGW support
- macOS: Xcode, command-line tools
- Linux: GCC, Clang support

## 🔍 Troubleshooting

### Common Issues

**Include Directory Errors**
```bash
# CMake policy error
cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 17)
```

**Qt Not Found**
```bash
# Set Qt6_DIR manually
cmake -DQt6_DIR=/path/to/qt/lib/cmake/Qt6 ..
```

**Platform-Specific Errors**
```bash
# Windows: Set correct generator
cmake -G "Visual Studio 17 2022" -A x64 ..

# macOS: Set deployment target
cmake -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0 ..

# Linux: Set install prefix
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
```

### Build Diagnostics
```bash
# Verbose build output
make VERBOSE=1

# CMake debug output
cmake --debug-output ..

# Show cache variables
cmake -LAH ..
```

## 📚 Integration with IDEs

### Visual Studio Code
1. Install CMake extension
2. Open project folder
3. Select build variant (Debug/Release)
4. Configure CMake options in .vscode/settings.json

### CLion
1. Open project folder
2. Go to File → Settings → Build, Execution, Deployment → CMake
3. Configure build types and options
4. Reload CMake project

### Qt Creator
1. Open project folder
2. Select "Open Project"
3. Configure build and run settings
4. Select kit (compiler, Qt version)

## 🎨 Customization

### Adding New Build Options
```cmake
# In CMakeLists.txt
option(ENABLE_CUSTOM_FEATURE "Enable custom feature" OFF)

if(ENABLE_CUSTOM_FEATURE)
    # Add custom configuration
endif()
```

### Extending Packaging
```cmake
# In cmake/Packaging.cmake
set(CPACK_COMPONENTS_ALL Runtime Development Custom)
set(CPACK_COMPONENT_CUSTOM_DESCRIPTION "Custom component")
```

### Adding Platform-Specific Code
```cmake
# In cmake/PlatformConfig.cmake
if(WIN32)
    # Windows-specific code
elseif(APPLE)
    # macOS-specific code
elseif(UNIX AND NOT APPLE)
    # Linux-specific code
endif()
```

## 🔄 Version History

### 1.0.0 (2024-09-20)
- Complete CMake configuration system
- Cross-platform support (Windows, macOS, Linux)
- Multiple build types and packaging
- Qt 6.2+ integration
- Comprehensive utility functions
- Component-based installation

### Known Issues
- Qt logging warnings on macOS (benign)
- Some unused parameter warnings in plugin code
- Variadic macro warnings on newer compilers

## 📄 Contributing

### Code Style
- Use 4 spaces for indentation (no tabs)
- Maximum line length: 120 characters
- Functions: `lower_case_with_underscores()`
- Variables: `UPPER_CASE_WITH_UNDERSCORES`
- Targets: `lower_case_with_underscores`

### Development Workflow
1. Create feature branch: `git checkout -b feature/your-feature`
2. Make changes following style guidelines
3. Test on all supported platforms and build types
4. Submit pull request with clear description

### Testing
- Test all build types: Debug, Release, RelWithDebInfo, MinSizeRel
- Test packaging on all platforms
- Verify installation works correctly

## 📄 License

This CMake configuration is part of the QML C++ Template project and follows the same license terms.