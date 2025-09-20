#include "Application.h"
#include "../plugin/PluginManager.h"
#include <QCoreApplication>
#include <QDir>


Application::Application(QObject *parent)
    : QObject(parent)
    , m_settings(new QSettings(this))
{
    qDebug() << "Application initialized";
}

Application::~Application()
{
    qDebug() << "Application destroyed";
}

QString Application::version() const
{
    return QCoreApplication::applicationVersion();
}

QString Application::organizationName() const
{
    return QCoreApplication::organizationName();
}

QString Application::applicationName() const
{
    return QCoreApplication::applicationName();
}

QSettings* Application::settings() const
{
    return m_settings;
}

QStringList Application::getAvailablePlugins() const
{
    auto *manager = PluginManager::instance();
    return manager->availablePlugins();
}

QVariantMap Application::getPluginInfo(const QString &pluginName) const
{
    auto *manager = PluginManager::instance();
    return manager->getPluginInfo(pluginName);
}

bool Application::loadPlugins(const QString &pluginDir)
{
    auto *manager = PluginManager::instance();
    return manager->loadPlugins(pluginDir);
}

bool Application::initializePlugin(const QString &pluginName, const QVariantMap &config)
{
    auto *manager = PluginManager::instance();
    return manager->initializePlugin(pluginName, config);
}

bool Application::unloadPlugin(const QString &pluginName)
{
    auto *manager = PluginManager::instance();
    return manager->unloadPlugin(pluginName);
}