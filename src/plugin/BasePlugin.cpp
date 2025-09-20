#include "BasePlugin.h"
#include <QDebug>

BasePlugin::BasePlugin(QObject *parent)
    : IPlugin(parent)
{
}

bool BasePlugin::initialize(const QVariantMap &config)
{
    if (m_initialized) {
        qWarning() << "Plugin already initialized:" << name();
        return false;
    }

    if (!onInitialize(config)) {
        qWarning() << "Plugin initialization failed:" << name();
        return false;
    }

    m_initialized = true;
    emit initialized();
    return true;
}

void BasePlugin::shutdown()
{
    if (!m_initialized) {
        return;
    }

    onShutdown();
    m_initialized = false;
    emit shutdownComplete();
}

bool BasePlugin::hasSettings() const
{
    return !onGetSettings().isEmpty();
}

QVariantMap BasePlugin::getSettings() const
{
    if (!m_initialized) {
        return QVariantMap();
    }
    return onGetSettings();
}

bool BasePlugin::setSettings(const QVariantMap &settings)
{
    if (!m_initialized) {
        return false;
    }

    if (onSetSettings(settings)) {
        m_settings = settings;
        emit settingsChanged(settings);
        return true;
    }

    return false;
}