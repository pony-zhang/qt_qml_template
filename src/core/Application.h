#pragma once

#include <QObject>
#include <QSettings>
#include <QLoggingCategory>
#include <QVariantMap>

Q_DECLARE_LOGGING_CATEGORY(appCore)

class Application : public QObject
{
    Q_OBJECT

public:
    explicit Application(QObject *parent = nullptr);
    ~Application();

    Q_INVOKABLE QString version() const;
    Q_INVOKABLE QString organizationName() const;
    Q_INVOKABLE QString applicationName() const;

    QSettings* settings() const;

    // Plugin management methods
    Q_INVOKABLE QStringList getAvailablePlugins() const;
    Q_INVOKABLE QVariantMap getPluginInfo(const QString &pluginName) const;
    Q_INVOKABLE bool loadPlugins(const QString &pluginDir = QString());
    Q_INVOKABLE bool initializePlugin(const QString &pluginName, const QVariantMap &config = QVariantMap());
    Q_INVOKABLE bool unloadPlugin(const QString &pluginName);

signals:
    void pluginLoaded(const QString &pluginName);
    void pluginUnloaded(const QString &pluginName);
    void pluginError(const QString &pluginName, const QString &error);

private:
    QSettings* m_settings;
};