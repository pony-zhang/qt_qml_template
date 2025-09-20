#pragma once

#include <QLoggingCategory>
#include <QDebug>
#include <QMessageLogContext>

class ZeroOverheadLogger
{
public:
    template<typename T>
    ZeroOverheadLogger& operator<<(const T &value)
    {
        if (m_enabled) {
            m_stream << value;
        }
        return *this;
    }

    ZeroOverheadLogger& noDebug()
    {
        m_enabled = false;
        return *this;
    }

    bool isEnabled() const { return m_enabled; }

private:
    bool m_enabled = true;
    QString m_buffer;
    QDebug m_stream{&m_buffer};

    friend class ZeroOverheadLogFactory;
    ZeroOverheadLogger(bool enabled) : m_enabled(enabled) {}
};

class ZeroOverheadLogFactory
{
public:
    static ZeroOverheadLogger createDebug(const char *category)
    {
        return ZeroOverheadLogger(isDebugEnabled(category));
    }

    static ZeroOverheadLogger createInfo(const char *category)
    {
        return ZeroOverheadLogger(isInfoEnabled(category));
    }

    static ZeroOverheadLogger createWarning(const char *category)
    {
        return ZeroOverheadLogger(isWarningEnabled(category));
    }

    static ZeroOverheadLogger createCritical(const char *category)
    {
        return ZeroOverheadLogger(isCriticalEnabled(category));
    }

private:
    static bool isDebugEnabled(const char *category)
    {
        return QLoggingCategory::defaultCategory()->isDebugEnabled();
    }

    static bool isInfoEnabled(const char *category)
    {
        return QLoggingCategory::defaultCategory()->isInfoEnabled();
    }

    static bool isWarningEnabled(const char *category)
    {
        return QLoggingCategory::defaultCategory()->isWarningEnabled();
    }

    static bool isCriticalEnabled(const char *category)
    {
        return QLoggingCategory::defaultCategory()->isCriticalEnabled();
    }
};

#define ZLOG_DEBUG() \
    ZeroOverheadLogFactory::createDebug(SmartLogPlugin::autoDetectCategory(__FILE__).toUtf8().constData())

#define ZLOG_INFO() \
    ZeroOverheadLogFactory::createInfo(SmartLogPlugin::autoDetectCategory(__FILE__).toUtf8().constData())

#define ZLOG_WARNING() \
    ZeroOverheadLogFactory::createWarning(SmartLogPlugin::autoDetectCategory(__FILE__).toUtf8().constData())

#define ZLOG_CRITICAL() \
    ZeroOverheadLogFactory::createCritical(SmartLogPlugin::autoDetectCategory(__FILE__).toUtf8().constData())

#define ZLOG_ONCE_DEBUG() \
    ZLOG_DEBUG().noDebug() << ""

#define ZLOG_ONCE_INFO() \
    ZLOG_INFO().noDebug() << ""

#define ZLOG_ONCE_WARNING() \
    ZLOG_WARNING().noDebug() << ""

#define ZLOG_ONCE_CRITICAL() \
    ZLOG_CRITICAL().noDebug() << ""