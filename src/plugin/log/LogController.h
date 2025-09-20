#pragma once

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QtQml/qqmlregistration.h>

class LogController : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit LogController(QObject *parent = nullptr);
    ~LogController() = default;

    Q_INVOKABLE void setLogLevel(const QString &category, const QString &level);
    Q_INVOKABLE void enableCategory(const QString &category, bool enabled = true);
    Q_INVOKABLE void disableCategory(const QString &category);
    Q_INVOKABLE void setGlobalRules(const QString &rules);
    Q_INVOKABLE QString getCurrentRules() const;

    Q_INVOKABLE void enableFileLogging(const QString &filePath);
    Q_INVOKABLE void disableFileLogging();
    Q_INVOKABLE void enableConsoleLogging(bool enabled);
    Q_INVOKABLE void setJsonFormat(bool enabled);

    Q_INVOKABLE void setCustomFormat(const QString &pattern);

    Q_INVOKABLE QVariantList getAvailableCategories() const;
    Q_INVOKABLE QVariantMap getCategoryConfig(const QString &category) const;

    Q_INVOKABLE void testLog(const QString &category, const QString &level, const QString &message);

signals:
    void rulesChanged(const QString &rules);
    void fileLoggingChanged(bool enabled);
    void consoleLoggingChanged(bool enabled);
    void formatChanged(bool jsonFormat);

private:
    void updateLogRules();
    void refreshCategoryList();
};