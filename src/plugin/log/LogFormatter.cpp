#include "LogFormatter.h"
#include <QDateTime>
#include <QCoreApplication>
#include <QThread>
#include <QFileInfo>

QString LogFormatter::formatText(const LogEntry &entry)
{
    return QString("[%1] [%2] [%3] %4:%5:%6 - %7")
           .arg(formatTimestamp(entry.timestamp),
                levelToString(entry.level),
                entry.category,
                entry.file,
                QString::number(entry.line),
                entry.function,
                entry.message);
}

QString LogFormatter::formatJson(const LogEntry &entry)
{
    QJsonObject json;
    json["timestamp"] = entry.timestamp.toString(Qt::ISODate);
    json["level"] = levelToString(entry.level);
    json["category"] = entry.category;
    json["message"] = entry.message;

    if (!entry.file.isEmpty()) {
        json["file"] = entry.file;
        json["line"] = entry.line;
    }

    if (!entry.function.isEmpty()) {
        json["function"] = entry.function;
    }

    if (!entry.threadId.isEmpty()) {
        json["thread_id"] = entry.threadId;
    }

    QJsonDocument doc(json);
    return doc.toJson(QJsonDocument::Compact);
}

QString LogFormatter::formatCustom(const LogEntry &entry, const QString &pattern)
{
    QString result = pattern;

    result.replace("{timestamp}", formatTimestamp(entry.timestamp));
    result.replace("{level}", levelToString(entry.level));
    result.replace("{category}", entry.category);
    result.replace("{message}", entry.message);
    result.replace("{file}", entry.file);
    result.replace("{line}", QString::number(entry.line));
    result.replace("{function}", entry.function);
    result.replace("{thread_id}", entry.threadId);

    return result;
}

QString LogFormatter::formatCompact(const LogEntry &entry)
{
    return QString("[%1] [%2] %3")
           .arg(formatTimestamp(entry.timestamp, "hh:mm:ss.zzz"),
                levelToString(entry.level),
                entry.message);
}

QString LogFormatter::formatDetailed(const LogEntry &entry)
{
    return QString("┌─ [%1] %2 (%3)\n"
                   "│  Level:    %4\n"
                   "│  Category: %5\n"
                   "│  File:     %6:%7\n"
                   "│  Function: %8\n"
                   "│  Thread:   %9\n"
                   "└─ Message:  %10")
           .arg(formatTimestamp(entry.timestamp),
                entry.file,
                QString::number(entry.line),
                levelToString(entry.level),
                entry.category,
                entry.file,
                QString::number(entry.line),
                entry.function,
                entry.threadId,
                entry.message);
}

QString LogFormatter::formatColored(const LogEntry &entry)
{
    QString color = getAnsiColor(entry.level);
    QString reset = "\033[0m";

    return QString("%1[%2] [%3] [%4]%5 %6:%7:%8 - %9")
           .arg(color,
                formatTimestamp(entry.timestamp),
                levelToString(entry.level),
                entry.category,
                reset,
                entry.file,
                QString::number(entry.line),
                entry.function,
                entry.message);
}

QString LogFormatter::levelToString(QtMsgType level)
{
    switch (level) {
    case QtDebugMsg:    return "DEBUG";
    case QtInfoMsg:     return "INFO";
    case QtWarningMsg:  return "WARNING";
    case QtCriticalMsg: return "CRITICAL";
    case QtFatalMsg:    return "FATAL";
    default:            return "UNKNOWN";
    }
}

QString LogFormatter::getAnsiColor(QtMsgType level)
{
    switch (level) {
    case QtDebugMsg:    return "\033[36m";    // Cyan
    case QtInfoMsg:     return "\033[32m";    // Green
    case QtWarningMsg:  return "\033[33m";    // Yellow
    case QtCriticalMsg: return "\033[31m";    // Red
    case QtFatalMsg:    return "\033[35m";    // Magenta
    default:            return "\033[0m";     // Reset
    }
}

LogFormatter::LogEntry LogFormatter::parseMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    LogEntry entry;
    entry.timestamp = QDateTime::currentDateTime();
    entry.level = type;
    entry.message = msg;
    entry.category = context.category ? QString(context.category) : "default";
    entry.file = context.file ? QFileInfo(context.file).fileName() : "unknown";
    entry.line = context.line ? context.line : 0;
    entry.function = context.function ? QString(context.function) : "";
    entry.threadId = QString::number(reinterpret_cast<quintptr>(QThread::currentThreadId()));

    return entry;
}

QString LogFormatter::escapeJson(const QString &value)
{
    QString result = value;
    result.replace("\\", "\\\\");
    result.replace("\"", "\\\"");
    result.replace("\b", "\\b");
    result.replace("\f", "\\f");
    result.replace("\n", "\\n");
    result.replace("\r", "\\r");
    result.replace("\t", "\\t");
    return result;
}

QString LogFormatter::formatTimestamp(const QDateTime &timestamp, const QString &format)
{
    return timestamp.toString(format);
}