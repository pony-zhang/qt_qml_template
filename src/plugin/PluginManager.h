#pragma once

#include <QObject>
#include <QStringList>
#include <QVariantMap>
#include <QMap>
#include <QPluginLoader>
#include <QDir>
#include "IPlugin.h"

class PluginManager : public QObject
{
    Q_OBJECT

public:
    static PluginManager* instance();

    explicit PluginManager(QObject *parent = nullptr);
    ~PluginManager();

    bool loadPlugins(const QString &pluginDir = QString());
    bool unloadAllPlugins();

    bool loadPlugin(const QString &pluginPath);
    bool unloadPlugin(const QString &pluginName);

    QList<IPlugin*> loadedPlugins() const;
    IPlugin* getPlugin(const QString &name) const;

    bool initializeAllPlugins(const QVariantMap &config = QVariantMap());
    bool initializePlugin(const QString &name, const QVariantMap &config = QVariantMap());

    QStringList availablePlugins() const;
    bool isPluginLoaded(const QString &name) const;

    QVariantMap getPluginInfo(const QString &name) const;

signals:
    void pluginLoaded(const QString &name);
    void pluginUnloaded(const QString &name);
    void pluginInitialized(const QString &name);
    void pluginError(const QString &name, const QString &error);

private:
    static PluginManager* s_instance;

    QMap<QString, IPlugin*> m_plugins;
    QMap<QString, QPluginLoader*> m_loaders;
    QString m_pluginDir;

    void cleanup();
};