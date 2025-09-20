#include "PluginLoader.h"
#include "PluginManager.h"
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

PluginLoader* PluginLoader::s_instance = nullptr;

PluginLoader* PluginLoader::instance()
{
    if (!s_instance) {
        s_instance = new PluginLoader(QCoreApplication::instance());
    }
    return s_instance;
}

PluginLoader::PluginLoader(QObject *parent)
    : QObject(parent)
{
}

PluginLoader::~PluginLoader()
{
    shutdownPluginSystem();
    s_instance = nullptr;
}

bool PluginLoader::loadPluginSystem(const QVariantMap &config)
{
    if (m_systemLoaded) {
        qDebug() << "Plugin system already loaded";
        return true;
    }

    PluginManager *manager = PluginManager::instance();

    QString pluginDir = config.value("pluginDir").toString();
    if (pluginDir.isEmpty()) {
        pluginDir = QCoreApplication::applicationDirPath() + "/plugins";
    }

    QDir dir(pluginDir);
    if (!dir.exists()) {
        dir.mkpath(pluginDir);
        qDebug() << "Created plugin directory:" << pluginDir;
    }

    if (!manager->loadPlugins(pluginDir)) {
        qWarning() << "Failed to load plugins from:" << pluginDir;
    }

    if (!manager->initializeAllPlugins(config)) {
        qWarning() << "Failed to initialize all plugins";
    }

    m_systemLoaded = true;
    emit pluginSystemLoaded();

    qDebug() << "Plugin system loaded successfully";
    return true;
}

void PluginLoader::shutdownPluginSystem()
{
    if (!m_systemLoaded) {
        return;
    }

    PluginManager *manager = PluginManager::instance();
    manager->unloadAllPlugins();

    m_systemLoaded = false;
    emit pluginSystemShutdown();

    qDebug() << "Plugin system shutdown complete";
}

bool PluginLoader::isPluginSystemLoaded() const
{
    return m_systemLoaded;
}