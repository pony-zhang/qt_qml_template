#pragma once

#include <QLoggingCategory>

#define LOG_CATEGORY(name) \
    Q_LOGGING_CATEGORY(name, name)

#define LOG_DEBUG() \
    qDebug(SmartLogPlugin::autoDetectCategory(__FILE__).toUtf8().constData())

#define LOG_INFO() \
    qCInfo(SmartLogPlugin::autoDetectCategory(__FILE__).toUtf8().constData())

#define LOG_WARNING() \
    qCWarning(SmartLogPlugin::autoDetectCategory(__FILE__).toUtf8().constData())

#define LOG_CRITICAL() \
    qCCritical(SmartLogPlugin::autoDetectCategory(__FILE__).toUtf8().constData())

#define LOG_ONCE_DEBUG() \
    LOG_DEBUG().noDebug() << ""

#define LOG_ONCE_INFO() \
    LOG_INFO().noDebug() << ""

#define LOG_ONCE_WARNING() \
    LOG_WARNING().noDebug() << ""

#define LOG_ONCE_CRITICAL() \
    LOG_CRITICAL().noDebug() << ""