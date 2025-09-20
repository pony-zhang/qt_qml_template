#pragma once

#include <QLoggingCategory>
#include <QMessageLogContext>
#include <QMutex>
#include <QSet>

class SmartLogHandler
{
public:
    static SmartLogHandler* instance();

    void initialize();
    void shutdown();

    void setLoggingRules(const QString &rules);
    void enableCategory(const QString &category, bool enabled = true);
    void setCategoryLevel(const QString &category, QtMsgType minLevel);

    bool isCategoryEnabled(const QString &category) const;
    QtMsgType getCategoryLevel(const QString &category) const;

    void setOutputFormat(bool jsonFormat);
    void setFileOutput(const QString &filePath);
    void setConsoleOutput(bool enabled);

private:
    static SmartLogHandler* s_instance;
    static QMutex s_mutex;

    struct CategoryConfig {
        bool enabled = true;
        QtMsgType minLevel = QtDebugMsg;
    };

    QMap<QString, CategoryConfig> m_categoryConfigs;
    QtMessageHandler m_originalHandler = nullptr;
    bool m_jsonFormat = false;
    bool m_consoleEnabled = true;
    QString m_logFilePath;

    SmartLogHandler();
    ~SmartLogHandler();

    void setupMessageHandler();
    void restoreMessageHandler();
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    void processMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    QString formatMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg) const;
    QString formatJsonMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg) const;
    void writeToLog(const QString &message);
    QString detectCategory(const QString &filePath) const;
    QString levelToString(QtMsgType type) const;
    void parseLoggingRules(const QString &rules);
};