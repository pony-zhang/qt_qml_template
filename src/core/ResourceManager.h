#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QVariant>
#include <QDir>
#include <QUrl>
#include <QResource>

class ResourceManager : public QObject
{
    Q_OBJECT

public:
    static ResourceManager* instance();

    explicit ResourceManager(QObject *parent = nullptr);
    ~ResourceManager();

    // Resource management
    bool initialize(const QString &baseResourcePath = QString());
    bool registerResourcePath(const QString &resourceType, const QString &path);
    bool unregisterResourcePath(const QString &resourceType);
    QStringList getRegisteredResourceTypes() const;
    QString getResourcePath(const QString &resourceType, const QString &resourceName) const;

    // File operations
    Q_INVOKABLE bool resourceExists(const QString &resourcePath) const;
    Q_INVOKABLE QString readTextResource(const QString &resourcePath) const;
    Q_INVOKABLE QByteArray readBinaryResource(const QString &resourcePath) const;
    Q_INVOKABLE bool saveResource(const QString &resourcePath, const QByteArray &data) const;

    // QML resources
    Q_INVOKABLE QUrl getQmlResource(const QString &qmlName) const;
    Q_INVOKABLE QStringList getAvailableQmlResources() const;

    // Image resources
    Q_INVOKABLE QUrl getImageResource(const QString &imageName) const;
    Q_INVOKABLE QStringList getAvailableImages() const;

    // Font resources - removed
    // Q_INVOKABLE bool loadFont(const QString &fontPath);
    // Q_INVOKABLE QStringList getAvailableFonts() const;

    // Translation resources
    Q_INVOKABLE bool loadTranslation(const QString &translationFile, const QString &locale = QString());
    Q_INVOKABLE QStringList getAvailableTranslations() const;

    // Configuration resources
    Q_INVOKABLE QVariantMap loadConfig(const QString &configName) const;
    Q_INVOKABLE bool saveConfig(const QString &configName, const QVariantMap &config) const;

    // Plugin resources
    Q_INVOKABLE bool registerPluginResources(const QString &pluginName, const QString &resourcePath);
    Q_INVOKABLE bool unregisterPluginResources(const QString &pluginName);
    Q_INVOKABLE QUrl getPluginResource(const QString &pluginName, const QString &resourceName) const;

    // Resource caching
    void enableCache(bool enabled);
    void clearCache();
    Q_INVOKABLE void preloadResources(const QStringList &resourcePaths);

signals:
    void resourceLoaded(const QString &resourcePath);
    void resourceError(const QString &resourcePath, const QString &error);
    void pluginResourcesRegistered(const QString &pluginName);
    void pluginResourcesUnregistered(const QString &pluginName);

private:
    static ResourceManager* s_instance;

    QString m_baseResourcePath;
    QMap<QString, QStringList> m_resourcePaths;
    QMap<QString, QByteArray> m_resourceCache;
    QMap<QString, QString> m_pluginResourcePaths;
    bool m_cacheEnabled;

    bool isValidResourcePath(const QString &path) const;
    QString resolveResourcePath(const QString &resourceType, const QString &resourceName) const;
    QByteArray getResourceData(const QString &resourcePath) const;
    void cacheResource(const QString &resourcePath, const QByteArray &data);
};