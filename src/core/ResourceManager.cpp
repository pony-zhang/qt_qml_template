#include "ResourceManager.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QTranslator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QRegularExpression>

ResourceManager* ResourceManager::s_instance = nullptr;

ResourceManager* ResourceManager::instance()
{
    if (!s_instance) {
        s_instance = new ResourceManager();
    }
    return s_instance;
}

ResourceManager::ResourceManager(QObject *parent)
    : QObject(parent)
    , m_cacheEnabled(true)
{
    qDebug() << "ResourceManager initialized";
}

ResourceManager::~ResourceManager()
{
    clearCache();
    qDebug() << "ResourceManager destroyed";
}

bool ResourceManager::initialize(const QString &baseResourcePath)
{
    if (!baseResourcePath.isEmpty()) {
        m_baseResourcePath = baseResourcePath;
    } else {
        m_baseResourcePath = QCoreApplication::applicationDirPath() + "/resources";
    }

    QDir baseDir(m_baseResourcePath);
    if (!baseDir.exists()) {
        if (!baseDir.mkpath(m_baseResourcePath)) {
            qWarning() << "Failed to create base resource path:" << m_baseResourcePath;
            return false;
        }
    }

    registerResourcePath("qml", m_baseResourcePath + "/qml");
    registerResourcePath("images", m_baseResourcePath + "/images");
    // registerResourcePath("fonts", m_baseResourcePath + "/fonts"); // Font functionality removed
    registerResourcePath("translations", m_baseResourcePath + "/translations");
    registerResourcePath("config", m_baseResourcePath + "/config");

    qDebug() << "ResourceManager initialized with base path:" << m_baseResourcePath;
    return true;
}

bool ResourceManager::registerResourcePath(const QString &resourceType, const QString &path)
{
    if (!isValidResourcePath(path)) {
        qWarning() << "Invalid resource path:" << path << "for type:" << resourceType;
        return false;
    }

    QDir dir(path);
    if (!dir.exists()) {
        if (!dir.mkpath(path)) {
            qWarning() << "Failed to create resource path:" << path;
            return false;
        }
    }

    if (!m_resourcePaths[resourceType].contains(path)) {
        m_resourcePaths[resourceType].append(path);
        qDebug() << "Registered resource path:" << path << "for type:" << resourceType;
    }

    return true;
}

bool ResourceManager::unregisterResourcePath(const QString &resourceType)
{
    if (m_resourcePaths.contains(resourceType)) {
        m_resourcePaths.remove(resourceType);
        qDebug() << "Unregistered resource type:" << resourceType;
        return true;
    }
    return false;
}

QStringList ResourceManager::getRegisteredResourceTypes() const
{
    return m_resourcePaths.keys();
}

QString ResourceManager::getResourcePath(const QString &resourceType, const QString &resourceName) const
{
    return resolveResourcePath(resourceType, resourceName);
}

bool ResourceManager::resourceExists(const QString &resourcePath) const
{
    return QFile::exists(resourcePath);
}

QString ResourceManager::readTextResource(const QString &resourcePath) const
{
    QByteArray data = getResourceData(resourcePath);
    return QString::fromUtf8(data);
}

QByteArray ResourceManager::readBinaryResource(const QString &resourcePath) const
{
    return getResourceData(resourcePath);
}

bool ResourceManager::saveResource(const QString &resourcePath, const QByteArray &data) const
{
    QFile file(resourcePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open file for writing:" << resourcePath;
        return false;
    }

    if (file.write(data) == -1) {
        qWarning() << "Failed to write data to file:" << resourcePath;
        return false;
    }

    file.close();
    return true;
}

QUrl ResourceManager::getQmlResource(const QString &qmlName) const
{
    QString resourcePath = resolveResourcePath("qml", qmlName);
    if (resourcePath.isEmpty()) {
        return QUrl();
    }

    QUrl url = QUrl::fromLocalFile(resourcePath);
    if (!url.isValid()) {
        qWarning() << "Invalid QML resource URL:" << resourcePath;
        return QUrl();
    }

    return url;
}

QStringList ResourceManager::getAvailableQmlResources() const
{
    QStringList qmlFiles;
    const QStringList &paths = m_resourcePaths.value("qml");

    for (const QString &path : paths) {
        QDir dir(path);
        QStringList filters;
        filters << "*.qml" << "*.js";
        dir.setNameFilters(filters);

        QFileInfoList files = dir.entryInfoList(QDir::Files);
        for (const QFileInfo &file : files) {
            qmlFiles.append(file.fileName());
        }
    }

    return qmlFiles;
}

QUrl ResourceManager::getImageResource(const QString &imageName) const
{
    QString resourcePath = resolveResourcePath("images", imageName);
    if (resourcePath.isEmpty()) {
        return QUrl();
    }

    QUrl url = QUrl::fromLocalFile(resourcePath);
    return url;
}

QStringList ResourceManager::getAvailableImages() const
{
    QStringList imageFiles;
    const QStringList &paths = m_resourcePaths.value("images");

    for (const QString &path : paths) {
        QDir dir(path);
        QStringList filters;
        filters << "*.png" << "*.jpg" << "*.jpeg" << "*.gif" << "*.svg" << "*.bmp";
        dir.setNameFilters(filters);

        QFileInfoList files = dir.entryInfoList(QDir::Files);
        for (const QFileInfo &file : files) {
            imageFiles.append(file.fileName());
        }
    }

    return imageFiles;
}


bool ResourceManager::loadTranslation(const QString &translationFile, const QString &locale)
{
    QString translationPath = resolveResourcePath("translations", translationFile);
    if (translationPath.isEmpty()) {
        qWarning() << "Translation file not found:" << translationFile;
        return false;
    }

    QTranslator *translator = new QTranslator(this);
    if (!translator->load(translationPath)) {
        qWarning() << "Failed to load translation:" << translationPath;
        delete translator;
        return false;
    }

    QString effectiveLocale = locale.isEmpty() ? QLocale::system().name() : locale;
    if (!QCoreApplication::installTranslator(translator)) {
        qWarning() << "Failed to install translator for locale:" << effectiveLocale;
        delete translator;
        return false;
    }

    qDebug() << "Translation loaded for locale:" << effectiveLocale;
    return true;
}

QStringList ResourceManager::getAvailableTranslations() const
{
    QStringList translationFiles;
    const QStringList &paths = m_resourcePaths.value("translations");

    for (const QString &path : paths) {
        QDir dir(path);
        QStringList filters;
        filters << "*.qm" << "*.ts";
        dir.setNameFilters(filters);

        QFileInfoList files = dir.entryInfoList(QDir::Files);
        for (const QFileInfo &file : files) {
            translationFiles.append(file.fileName());
        }
    }

    return translationFiles;
}

QVariantMap ResourceManager::loadConfig(const QString &configName) const
{
    QString configPath = resolveResourcePath("config", configName + ".json");
    if (configPath.isEmpty()) {
        return QVariantMap();
    }

    QByteArray data = getResourceData(configPath);
    if (data.isEmpty()) {
        return QVariantMap();
    }

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "Invalid JSON config file:" << configPath;
        return QVariantMap();
    }

    return doc.object().toVariantMap();
}

bool ResourceManager::saveConfig(const QString &configName, const QVariantMap &config) const
{
    QString configPath = resolveResourcePath("config", configName + ".json");
    if (configPath.isEmpty()) {
        return false;
    }

    QJsonObject jsonObj = QJsonObject::fromVariantMap(config);
    QJsonDocument doc(jsonObj);
    QByteArray data = doc.toJson();

    return saveResource(configPath, data);
}

bool ResourceManager::registerPluginResources(const QString &pluginName, const QString &resourcePath)
{
    if (!isValidResourcePath(resourcePath)) {
        qWarning() << "Invalid plugin resource path:" << resourcePath;
        return false;
    }

    m_pluginResourcePaths[pluginName] = resourcePath;
    emit pluginResourcesRegistered(pluginName);
    qDebug() << "Registered plugin resources for:" << pluginName << "at:" << resourcePath;
    return true;
}

bool ResourceManager::unregisterPluginResources(const QString &pluginName)
{
    if (m_pluginResourcePaths.remove(pluginName)) {
        emit pluginResourcesUnregistered(pluginName);
        qDebug() << "Unregistered plugin resources for:" << pluginName;
        return true;
    }
    return false;
}

QUrl ResourceManager::getPluginResource(const QString &pluginName, const QString &resourceName) const
{
    if (!m_pluginResourcePaths.contains(pluginName)) {
        qWarning() << "Plugin resources not registered for:" << pluginName;
        return QUrl();
    }

    QString fullPath = m_pluginResourcePaths[pluginName] + "/" + resourceName;
    if (!QFile::exists(fullPath)) {
        qWarning() << "Plugin resource not found:" << fullPath;
        return QUrl();
    }

    return QUrl::fromLocalFile(fullPath);
}

void ResourceManager::enableCache(bool enabled)
{
    m_cacheEnabled = enabled;
    if (!enabled) {
        clearCache();
    }
    qDebug() << "Resource caching" << (enabled ? "enabled" : "disabled");
}

void ResourceManager::clearCache()
{
    m_resourceCache.clear();
    qDebug() << "Resource cache cleared";
}

void ResourceManager::preloadResources(const QStringList &resourcePaths)
{
    for (const QString &resourcePath : resourcePaths) {
        if (resourceExists(resourcePath)) {
            getResourceData(resourcePath);
            qDebug() << "Preloaded resource:" << resourcePath;
        }
    }
}

bool ResourceManager::isValidResourcePath(const QString &path) const
{
    if (path.isEmpty()) {
        return false;
    }

    QFileInfo fileInfo(path);
    return fileInfo.isAbsolute();
}

QString ResourceManager::resolveResourcePath(const QString &resourceType, const QString &resourceName) const
{
    if (!m_resourcePaths.contains(resourceType)) {
        qWarning() << "Resource type not registered:" << resourceType;
        return QString();
    }

    const QStringList &paths = m_resourcePaths[resourceType];
    for (const QString &path : paths) {
        QString fullPath = path + "/" + resourceName;
        if (QFile::exists(fullPath)) {
            return fullPath;
        }
    }

    qWarning() << "Resource not found:" << resourceName << "for type:" << resourceType;
    return QString();
}

QByteArray ResourceManager::getResourceData(const QString &resourcePath) const
{
    if (m_cacheEnabled && m_resourceCache.contains(resourcePath)) {
        return m_resourceCache[resourcePath];
    }

    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open resource:" << resourcePath;
        return QByteArray();
    }

    QByteArray data = file.readAll();
    file.close();

    if (m_cacheEnabled) {
        const_cast<ResourceManager*>(this)->cacheResource(resourcePath, data);
    }

    return data;
}

void ResourceManager::cacheResource(const QString &resourcePath, const QByteArray &data)
{
    if (m_cacheEnabled) {
        m_resourceCache[resourcePath] = data;
    }
}