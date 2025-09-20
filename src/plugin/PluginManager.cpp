#include "PluginManager.h"
#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(pluginManager)
Q_LOGGING_CATEGORY(pluginManager, "plugin.manager")

PluginManager* PluginManager::s_instance = nullptr;

PluginManager* PluginManager::instance()
{
    if (!s_instance) {
        s_instance = new PluginManager(QCoreApplication::instance());
    }
    return s_instance;
}

PluginManager::PluginManager(QObject *parent)
    : QObject(parent)
{
    m_pluginDir = QCoreApplication::applicationDirPath() + "/plugins";
}

PluginManager::~PluginManager()
{
    cleanup();
    s_instance = nullptr;
}

bool PluginManager::loadPlugins(const QString &pluginDir)
{
    if (!pluginDir.isEmpty()) {
        m_pluginDir = pluginDir;
    }

    QDir dir(m_pluginDir);
    if (!dir.exists()) {
        qCDebug(pluginManager) << "Plugin directory does not exist:" << m_pluginDir;
        return false;
    }

    QStringList filters;
    filters << "*.dll" << "*.so" << "*.dylib";
    dir.setNameFilters(filters);

    QStringList pluginFiles = dir.entryList(QDir::Files);
    bool success = true;

    for (const QString &pluginFile : pluginFiles) {
        QString fullPath = dir.absoluteFilePath(pluginFile);
        if (!loadPlugin(fullPath)) {
            success = false;
        }
    }

    return success;
}

bool PluginManager::loadPlugin(const QString &pluginPath)
{
    QPluginLoader *loader = new QPluginLoader(pluginPath, this);
    if (!loader->load()) {
        qCWarning(pluginManager) << "Failed to load plugin:" << pluginPath
                                << "Error:" << loader->errorString();
        emit pluginError(QFileInfo(pluginPath).baseName(), loader->errorString());
        delete loader;
        return false;
    }

    QObject *pluginObj = loader->instance();
    if (!pluginObj) {
        qCWarning(pluginManager) << "Failed to get plugin instance:" << pluginPath
                                << "Error:" << loader->errorString();
        emit pluginError(QFileInfo(pluginPath).baseName(), loader->errorString());
        loader->unload();
        delete loader;
        return false;
    }

    IPlugin *plugin = qobject_cast<IPlugin*>(pluginObj);
    if (!plugin) {
        qCWarning(pluginManager) << "Plugin does not implement IPlugin interface:" << pluginPath;
        emit pluginError(QFileInfo(pluginPath).baseName(), "Invalid plugin interface");
        loader->unload();
        delete loader;
        return false;
    }

    QString pluginName = plugin->name();
    if (m_plugins.contains(pluginName)) {
        qCWarning(pluginManager) << "Plugin already loaded:" << pluginName;
        emit pluginError(pluginName, "Plugin already loaded");
        loader->unload();
        delete loader;
        return false;
    }

    m_plugins[pluginName] = plugin;
    m_loaders[pluginName] = loader;

    qCDebug(pluginManager) << "Plugin loaded successfully:" << pluginName;
    emit pluginLoaded(pluginName);

    return true;
}

bool PluginManager::unloadPlugin(const QString &pluginName)
{
    if (!m_plugins.contains(pluginName)) {
        qCWarning(pluginManager) << "Plugin not loaded:" << pluginName;
        return false;
    }

    IPlugin *plugin = m_plugins[pluginName];
    QPluginLoader *loader = m_loaders[pluginName];

    plugin->shutdown();

    if (!loader->unload()) {
        qCWarning(pluginManager) << "Failed to unload plugin:" << pluginName
                                << "Error:" << loader->errorString();
        return false;
    }

    m_plugins.remove(pluginName);
    m_loaders.remove(pluginName);

    qCDebug(pluginManager) << "Plugin unloaded successfully:" << pluginName;
    emit pluginUnloaded(pluginName);

    return true;
}

bool PluginManager::unloadAllPlugins()
{
    QStringList pluginNames = m_plugins.keys();
    bool success = true;

    for (const QString &pluginName : pluginNames) {
        if (!unloadPlugin(pluginName)) {
            success = false;
        }
    }

    return success;
}

QList<IPlugin*> PluginManager::loadedPlugins() const
{
    return m_plugins.values();
}

IPlugin* PluginManager::getPlugin(const QString &name) const
{
    return m_plugins.value(name);
}

bool PluginManager::initializeAllPlugins(const QVariantMap &config)
{
    bool success = true;

    for (auto it = m_plugins.begin(); it != m_plugins.end(); ++it) {
        if (!initializePlugin(it.key(), config)) {
            success = false;
        }
    }

    return success;
}

bool PluginManager::initializePlugin(const QString &name, const QVariantMap &config)
{
    IPlugin *plugin = m_plugins.value(name);
    if (!plugin) {
        qCWarning(pluginManager) << "Plugin not found:" << name;
        return false;
    }

    if (plugin->initialize(config)) {
        qCDebug(pluginManager) << "Plugin initialized:" << name;
        emit pluginInitialized(name);
        return true;
    } else {
        qCWarning(pluginManager) << "Failed to initialize plugin:" << name;
        return false;
    }
}

QStringList PluginManager::availablePlugins() const
{
    return m_plugins.keys();
}

bool PluginManager::isPluginLoaded(const QString &name) const
{
    return m_plugins.contains(name);
}

QVariantMap PluginManager::getPluginInfo(const QString &name) const
{
    QVariantMap info;

    IPlugin *plugin = m_plugins.value(name);
    if (!plugin) {
        return info;
    }

    info["name"] = plugin->name();
    info["version"] = plugin->version();
    info["description"] = plugin->description();
    info["author"] = plugin->author();
    info["enabled"] = plugin->isEnabled();
    info["hasSettings"] = plugin->hasSettings();

    return info;
}

void PluginManager::cleanup()
{
    unloadAllPlugins();
}