#include "LogController.h"
#include "SmartLogPlugin.h"
#include <QDebug>

LogController::LogController(QObject *parent)
    : QObject(parent)
{
}

void LogController::setLogLevel(const QString &category, const QString &level)
{
    auto *plugin = SmartLogPlugin::instance();
    if (plugin) {
        plugin->setLogLevel(category, level);
        emit rulesChanged(getCurrentRules());
    }
}

void LogController::enableCategory(const QString &category, bool enabled)
{
    auto *plugin = SmartLogPlugin::instance();
    if (plugin) {
        QString currentRules = plugin->getLogRules();
        QString newRules;

        if (enabled) {
            newRules = category + "=true";
        } else {
            newRules = category + "=false";
        }

        if (!currentRules.isEmpty()) {
            newRules = currentRules + ";" + newRules;
        }

        plugin->setLogRules(newRules);
        emit rulesChanged(newRules);
    }
}

void LogController::disableCategory(const QString &category)
{
    enableCategory(category, false);
}

void LogController::setGlobalRules(const QString &rules)
{
    auto *plugin = SmartLogPlugin::instance();
    if (plugin) {
        plugin->setLogRules(rules);
        emit rulesChanged(rules);
    }
}

QString LogController::getCurrentRules() const
{
    auto *plugin = SmartLogPlugin::instance();
    return plugin ? plugin->getLogRules() : QString();
}

void LogController::enableFileLogging(const QString &filePath)
{
    auto *plugin = SmartLogPlugin::instance();
    if (plugin) {
        plugin->enableFileLogging(filePath);
        emit fileLoggingChanged(true);
    }
}

void LogController::disableFileLogging()
{
    auto *plugin = SmartLogPlugin::instance();
    if (plugin) {
        plugin->disableFileLogging();
        emit fileLoggingChanged(false);
    }
}

void LogController::enableConsoleLogging(bool enabled)
{
    auto *plugin = SmartLogPlugin::instance();
    if (plugin) {
        plugin->enableConsoleLogging(enabled);
        emit consoleLoggingChanged(enabled);
    }
}

void LogController::setJsonFormat(bool enabled)
{
    auto *plugin = SmartLogPlugin::instance();
    if (plugin) {
        plugin->setJsonFormat(enabled);
        emit formatChanged(enabled);
    }
}

void LogController::setCustomFormat(const QString &pattern)
{
    Q_UNUSED(pattern)
    qDebug() << "Custom format not yet implemented";
}

QVariantList LogController::getAvailableCategories() const
{
    QVariantList categories;

    categories.append("app.ui");
    categories.append("app.network");
    categories.append("app.database");
    categories.append("app.plugin");
    categories.append("app.core");
    categories.append("app.models");
    categories.append("app.backend");
    categories.append("app.services");
    categories.append("app.utils");
    categories.append("app.tests");
    categories.append("app.default");

    return categories;
}

QVariantMap LogController::getCategoryConfig(const QString &category) const
{
    QVariantMap config;
    auto *plugin = SmartLogPlugin::instance();

    if (plugin) {
        QString rules = plugin->getLogRules();
        config["category"] = category;
        config["enabled"] = !rules.contains(category + "=false");
        config["rules"] = rules;
    }

    return config;
}

void LogController::testLog(const QString &category, const QString &level, const QString &message)
{
    QString logCategory = category.isEmpty() ? "app.default" : category;
    QString logMessage = message.isEmpty() ? "Test log message" : message;

    if (level.toLower() == "debug") {
        qDebug() << "[" << logCategory << "]" << logMessage;
    } else if (level.toLower() == "info") {
        qInfo() << "[" << logCategory << "]" << logMessage;
    } else if (level.toLower() == "warning") {
        qWarning() << "[" << logCategory << "]" << logMessage;
    } else if (level.toLower() == "critical") {
        qCritical() << "[" << logCategory << "]" << logMessage;
    } else {
        qDebug() << "[" << logCategory << "]" << logMessage;
    }
}

void LogController::updateLogRules()
{
    emit rulesChanged(getCurrentRules());
}

void LogController::refreshCategoryList()
{
    getAvailableCategories();
}