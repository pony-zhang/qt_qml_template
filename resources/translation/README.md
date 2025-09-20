# Translation Files

This directory contains internationalization (i18n) files for the QML C++ Template application.

## Files

### Translation Source Files (.ts)
- `app_en_US.ts` - English (United States) translation source
- `app_zh_CN.ts` - Chinese (Simplified) translation source

### Compiled Translation Files (.qm)
- `app_en_US.qm` - Compiled English translations
- `app_zh_CN.qm` - Compiled Chinese translations

### Resource Configuration
- `translations.qrc` - Qt resource file for translations

## Usage

### Updating Translation Sources

To update translation sources with new translatable strings from QML files:

```bash
lupdate qml/main.qml qml/pages/MainPage.qml -ts resources/translation/app_en_US.ts
lupdate qml/main.qml qml/pages/MainPage.qml -ts resources/translation/app_zh_CN.ts
```

### Building Translation Files

To compile translation source files to binary format:

```bash
lrelease resources/translation/app_en_US.ts -qm resources/translation/app_en_US.qm
lrelease resources/translation/app_zh_CN.ts -qm resources/translation/app_zh_CN.qm
```

### Using Translations in QML

Load translations in your main application:

```cpp
// In main.cpp
QTranslator translator;
if (translator.load(":/i18n/app_en_US.qm")) {
    app.installTranslator(&translator);
}
```

In QML files, use `qsTr()` or `tr()` for translatable strings:

```qml
Text {
    text: qsTr("Hello World")
}
```

## Adding New Languages

1. Create new .ts file for the language
2. Add the .qm file to translations.qrc
3. Update CMake configuration if needed
4. Build and test the translation

## CMake Build Targets

- `translations`: Update translation source files
- `release_translations`: Build compiled translation files