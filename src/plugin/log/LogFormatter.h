#pragma once

#include <QString>
#include <QMessageLogContext>
#include <QJsonObject>
#include <QJsonDocument>

class LogFormatter
{
public:
    enum class Format {
        Text,
        Json,
        Custom
    };

    struct LogEntry {
        QDateTime timestamp;
        QtMsgType level;
        QString category;
        QString message;
        QString file;
        int line;
        QString function;
        QString threadId;
    };

    static QString formatText(const LogEntry &entry);
    static QString formatJson(const LogEntry &entry);
    static QString formatCustom(const LogEntry &entry, const QString &pattern);

    static QString formatCompact(const LogEntry &entry);
    static QString formatDetailed(const LogEntry &entry);
    static QString formatColored(const LogEntry &entry);

    static QString levelToString(QtMsgType level);
    static QString getAnsiColor(QtMsgType level);

    static LogEntry parseMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    static QString escapeJson(const QString &value);
    static QString formatTimestamp(const QDateTime &timestamp, const QString &format = "yyyy-MM-dd hh:mm:ss.zzz");
};