#include "SmartLogPlugin.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QDebug>
#include <QDateTime>

SmartLogPlugin* SmartLogPlugin::s_instance = nullptr;
QMutex SmartLogPlugin::s_mutex;
QtMessageHandler SmartLogPlugin::s_originalHandler = nullptr;
QFile SmartLogPlugin::s_logFile;
QTextStream SmartLogPlugin::s_logStream;
QMap<QString, SmartLogPlugin::CategoryConfig> SmartLogPlugin::s_categoryConfigs;
bool SmartLogPlugin::s_fileLoggingEnabled = false;
bool SmartLogPlugin::s_consoleLoggingEnabled = true;
bool SmartLogPlugin::s_jsonFormat = false;
QMap<QString, QString> SmartLogPlugin::s_categoryMapping;

SmartLogPlugin* SmartLogPlugin::instance()
{
    return s_instance;
}

SmartLogPlugin::SmartLogPlugin(QObject *parent)
    : BasePlugin(parent)
{
    s_instance = this;
}

bool SmartLogPlugin::onInitialize(const QVariantMap &config)
{
    initializeCategoryMapping();

    if (config.contains("logRules")) {
        processLogRules(config["logRules"].toString());
    }

    if (config.contains("logFile")) {
        enableFileLogging(config["logFile"].toString());
    }

    if (config.contains("consoleLogging")) {
        enableConsoleLogging(config["consoleLogging"].toBool());
    }

    if (config.contains("jsonFormat")) {
        setJsonFormat(config["jsonFormat"].toBool());
    }

    setupMessageHandler();

    return true;
}

void SmartLogPlugin::onShutdown()
{
    cleanupMessageHandler();
    disableFileLogging();
}

bool SmartLogPlugin::onSetSettings(const QVariantMap &settings)
{
    if (settings.contains("logRules")) {
        processLogRules(settings["logRules"].toString());
    }

    if (settings.contains("logFile")) {
        QString logFile = settings["logFile"].toString();
        if (logFile.isEmpty()) {
            disableFileLogging();
        } else {
            enableFileLogging(logFile);
        }
    }

    if (settings.contains("consoleLogging")) {
        enableConsoleLogging(settings["consoleLogging"].toBool());
    }

    if (settings.contains("jsonFormat")) {
        setJsonFormat(settings["jsonFormat"].toBool());
    }

    return true;
}

QVariantMap SmartLogPlugin::onGetSettings() const
{
    QVariantMap settings;
    settings["logRules"] = getLogRules();
    settings["consoleLogging"] = s_consoleLoggingEnabled;
    settings["jsonFormat"] = s_jsonFormat;

    if (s_fileLoggingEnabled && s_logFile.isOpen()) {
        settings["logFile"] = s_logFile.fileName();
    } else {
        settings["logFile"] = "";
    }

    return settings;
}

void SmartLogPlugin::initializeCategoryMapping()
{
    s_categoryMapping["/ui/"] = "app.ui";
    s_categoryMapping["/network/"] = "app.network";
    s_categoryMapping["/database/"] = "app.database";
    s_categoryMapping["/plugin/"] = "app.plugin";
    s_categoryMapping["/core/"] = "app.core";
    s_categoryMapping["/models/"] = "app.models";
    s_categoryMapping["/backend/"] = "app.backend";
    s_categoryMapping["/services/"] = "app.services";
    s_categoryMapping["/utils/"] = "app.utils";
    s_categoryMapping["/tests/"] = "app.tests";

    s_categoryMapping["default"] = "app.default";
}

QString SmartLogPlugin::autoDetectCategory(const QString &filePath)
{
    QString normalizedPath = QDir::fromNativeSeparators(filePath).toLower();

    for (auto it = s_categoryMapping.constBegin(); it != s_categoryMapping.constEnd(); ++it) {
        if (it.key() != "default" && normalizedPath.contains(it.key())) {
            return it.value();
        }
    }

    return s_categoryMapping["default"];
}

void SmartLogPlugin::setupMessageHandler()
{
    QMutexLocker locker(&s_mutex);
    s_originalHandler = qInstallMessageHandler(logToFile);
}

void SmartLogPlugin::cleanupMessageHandler()
{
    QMutexLocker locker(&s_mutex);
    if (s_originalHandler) {
        qInstallMessageHandler(s_originalHandler);
        s_originalHandler = nullptr;
    }
}

void SmartLogPlugin::logToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QMutexLocker locker(&s_mutex);

    QString category = context.category ? QString(context.category) : autoDetectCategory(context.file);

    auto it = s_categoryConfigs.find(category);
    if (it != s_categoryConfigs.end()) {
        if (!it->enabled || type < it->minLevel) {
            return;
        }
    }

    QString formattedMessage;
    if (s_jsonFormat) {
        formattedMessage = formatJsonMessage(type, context, msg);
    } else {
        formattedMessage = formatMessage(type, context, msg);
    }

    if (s_fileLoggingEnabled && s_logFile.isOpen()) {
        s_logStream << formattedMessage << Qt::endl;
        s_logStream.flush();
    }

    if (s_consoleLoggingEnabled) {
        if (s_originalHandler) {
            s_originalHandler(type, context, msg);
        } else {
            fprintf(stderr, "%s\n", formattedMessage.toUtf8().constData());
            fflush(stderr);
        }
    }
}

void SmartLogPlugin::logToConsole(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QMutexLocker locker(&s_mutex);

    QString category = context.category ? QString(context.category) : autoDetectCategory(context.file);

    auto it = s_categoryConfigs.find(category);
    if (it != s_categoryConfigs.end()) {
        if (!it->enabled || type < it->minLevel) {
            return;
        }
    }

    if (s_originalHandler) {
        s_originalHandler(type, context, msg);
    } else {
        QString formattedMessage = formatMessage(type, context, msg);
        fprintf(stderr, "%s\n", formattedMessage.toUtf8().constData());
        fflush(stderr);
    }
}

QString SmartLogPlugin::formatMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString level = levelToString(type);
    QString category = context.category ? QString(context.category) : autoDetectCategory(context.file);
    QString file = context.file ? QFileInfo(context.file).fileName() : "unknown";
    QString line = context.line ? QString::number(context.line) : "0";
    QString function = context.function ? QString(context.function) : "";

    return QString("[%1] [%2] [%3] %4:%5:%6 - %7")
           .arg(timestamp, level, category, file, line, function, msg);
}

QString SmartLogPlugin::formatJsonMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QJsonObject json;
    json["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    json["level"] = levelToString(type);
    json["category"] = context.category ? QString(context.category) : autoDetectCategory(context.file);

    if (context.file) {
        json["file"] = QFileInfo(context.file).fileName();
        json["line"] = context.line;
    }

    if (context.function) {
        json["function"] = QString(context.function);
    }

    json["message"] = msg;

    QJsonDocument doc(json);
    return doc.toJson(QJsonDocument::Compact);
}

QString SmartLogPlugin::levelToString(QtMsgType type)
{
    switch (type) {
    case QtDebugMsg: return "DEBUG";
    case QtInfoMsg: return "INFO";
    case QtWarningMsg: return "WARNING";
    case QtCriticalMsg: return "CRITICAL";
    case QtFatalMsg: return "FATAL";
    default: return "UNKNOWN";
    }
}

void SmartLogPlugin::ensureLogDirectory(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    QDir dir = fileInfo.absoluteDir();

    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

void SmartLogPlugin::processLogRules(const QString &rules)
{
    QMutexLocker locker(&s_mutex);

    QStringList ruleList = rules.split(";", Qt::SkipEmptyParts);

    for (const QString &rule : ruleList) {
        QString trimmedRule = rule.trimmed();

        if (trimmedRule.startsWith("*")) {
            CategoryConfig config;
            if (trimmedRule.contains("=true")) {
                config.enabled = true;
            } else if (trimmedRule.contains("=false")) {
                config.enabled = false;
            }

            QString levelStr = trimmedRule.section('=', 1).section('.', 1);
            if (levelStr == "debug") config.minLevel = QtDebugMsg;
            else if (levelStr == "info") config.minLevel = QtInfoMsg;
            else if (levelStr == "warning") config.minLevel = QtWarningMsg;
            else if (levelStr == "critical") config.minLevel = QtCriticalMsg;
            else if (levelStr == "fatal") config.minLevel = QtFatalMsg;

            s_categoryConfigs["*"] = config;
        } else {
            QString category = trimmedRule.section('=', 0).trimmed();
            CategoryConfig config = s_categoryConfigs[category];

            if (trimmedRule.contains("=true")) {
                config.enabled = true;
            } else if (trimmedRule.contains("=false")) {
                config.enabled = false;
            }

            QString levelStr = trimmedRule.section('=', 1);
            if (levelStr == "debug") config.minLevel = QtDebugMsg;
            else if (levelStr == "info") config.minLevel = QtInfoMsg;
            else if (levelStr == "warning") config.minLevel = QtWarningMsg;
            else if (levelStr == "critical") config.minLevel = QtCriticalMsg;
            else if (levelStr == "fatal") config.minLevel = QtFatalMsg;

            s_categoryConfigs[category] = config;
        }
    }
}

void SmartLogPlugin::setLogRules(const QString &rules)
{
    processLogRules(rules);
}

QString SmartLogPlugin::getLogRules() const
{
    QStringList rules;

    for (auto it = s_categoryConfigs.constBegin(); it != s_categoryConfigs.constEnd(); ++it) {
        QString rule = it.key();
        if (!it.value().enabled) {
            rule += "=false";
        } else {
            QString level;
            switch (it.value().minLevel) {
            case QtDebugMsg: level = "debug"; break;
            case QtInfoMsg: level = "info"; break;
            case QtWarningMsg: level = "warning"; break;
            case QtCriticalMsg: level = "critical"; break;
            case QtFatalMsg: level = "fatal"; break;
            default: break;
            }
            if (!level.isEmpty()) {
                rule += "=" + level;
            }
        }
        rules.append(rule);
    }

    return rules.join("; ");
}

void SmartLogPlugin::setLogLevel(const QString &category, const QString &level)
{
    QMutexLocker locker(&s_mutex);

    CategoryConfig config = s_categoryConfigs[category];

    if (level == "debug") config.minLevel = QtDebugMsg;
    else if (level == "info") config.minLevel = QtInfoMsg;
    else if (level == "warning") config.minLevel = QtWarningMsg;
    else if (level == "critical") config.minLevel = QtCriticalMsg;
    else if (level == "fatal") config.minLevel = QtFatalMsg;

    s_categoryConfigs[category] = config;
}

void SmartLogPlugin::enableFileLogging(const QString &filePath)
{
    QMutexLocker locker(&s_mutex);

    if (s_logFile.isOpen()) {
        s_logFile.close();
    }

    ensureLogDirectory(filePath);
    s_logFile.setFileName(filePath);

    if (s_logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        s_logStream.setDevice(&s_logFile);
        s_fileLoggingEnabled = true;
    } else {
        s_fileLoggingEnabled = false;
    }
}

void SmartLogPlugin::disableFileLogging()
{
    QMutexLocker locker(&s_mutex);

    if (s_logFile.isOpen()) {
        s_logFile.close();
    }
    s_fileLoggingEnabled = false;
}

void SmartLogPlugin::enableConsoleLogging(bool enable)
{
    QMutexLocker locker(&s_mutex);
    s_consoleLoggingEnabled = enable;
}

void SmartLogPlugin::setJsonFormat(bool enable)
{
    QMutexLocker locker(&s_mutex);
    s_jsonFormat = enable;
}

