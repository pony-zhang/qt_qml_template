#pragma once

#include <QObject>
#include <QString>
#include <QVariantMap>

class PluginLoader : public QObject
{
    Q_OBJECT

public:
    static PluginLoader* instance();

    explicit PluginLoader(QObject *parent = nullptr);
    ~PluginLoader();

    bool loadPluginSystem(const QVariantMap &config = QVariantMap());
    void shutdownPluginSystem();

    bool isPluginSystemLoaded() const;

signals:
    void pluginSystemLoaded();
    void pluginSystemShutdown();

private:
    static PluginLoader* s_instance;
    bool m_systemLoaded = false;
};