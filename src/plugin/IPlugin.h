#pragma once

#include <QObject>
#include <QString>
#include <QVariantMap>

class IPlugin : public QObject
{
    Q_OBJECT

public:
    explicit IPlugin(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~IPlugin() = default;

    virtual QString name() const = 0;
    virtual QString version() const = 0;
    virtual QString description() const = 0;
    virtual QString author() const = 0;

    virtual bool initialize(const QVariantMap &config = QVariantMap()) = 0;
    virtual void shutdown() = 0;

    virtual bool isEnabled() const { return m_enabled; }
    virtual void setEnabled(bool enabled) { m_enabled = enabled; }

    virtual bool hasSettings() const { return false; }
    virtual QVariantMap getSettings() const { return QVariantMap(); }
    virtual bool setSettings(const QVariantMap &settings) { return false; }

signals:
    void initialized();
    void shutdownComplete();
    void settingsChanged(const QVariantMap &settings);

protected:
    bool m_enabled = true;
};