#pragma once

#include "../BasePlugin.h"
#include <QLoggingCategory>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>
#include <QMap>
#include <QSet>
#include <QJsonObject>

class SmartLogPlugin : public BasePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.template.smartlog" FILE "smartlog.json")

public:
    static SmartLogPlugin* instance();

    explicit SmartLogPlugin(QObject *parent = nullptr);
    ~SmartLogPlugin() = default;

    QString name() const override { return "SmartLogPlugin"; }
    QString version() const override { return "1.0.0"; }
    QString description() const override { return "Intelligent logging system with automatic categorization"; }
    QString author() const override { return "Logging System Team"; }

    Q_INVOKABLE void setLogRules(const QString &rules);
    Q_INVOKABLE QString getLogRules() const;
    Q_INVOKABLE void setLogLevel(const QString &category, const QString &level);
    Q_INVOKABLE void enableFileLogging(const QString &filePath);
    Q_INVOKABLE void disableFileLogging();
    Q_INVOKABLE void enableConsoleLogging(bool enable);
    Q_INVOKABLE void setJsonFormat(bool enable);

    static QString autoDetectCategory(const QString &filePath);
    static void logToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static void logToConsole(QtMsgType type, const QMessageLogContext &context, const QString &msg);

protected:
    bool onInitialize(const QVariantMap &config) override;
    void onShutdown() override;
    bool onSetSettings(const QVariantMap &settings) override;
    QVariantMap onGetSettings() const override;

private:
    struct CategoryConfig {
        bool enabled = true;
        QtMsgType minLevel = QtDebugMsg;
    };

    static SmartLogPlugin* s_instance;
    static QMutex s_mutex;
    static QtMessageHandler s_originalHandler;
    static QFile s_logFile;
    static QTextStream s_logStream;
    static QMap<QString, CategoryConfig> s_categoryConfigs;
    static bool s_fileLoggingEnabled;
    static bool s_consoleLoggingEnabled;
    static bool s_jsonFormat;
    static QMap<QString, QString> s_categoryMapping;

    void initializeCategoryMapping();
    void setupMessageHandler();
    void cleanupMessageHandler();

    static QString formatMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static QString formatJsonMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static QString levelToString(QtMsgType type);
    static void ensureLogDirectory(const QString &filePath);
    static void processLogRules(const QString &rules);
};