#pragma once

#include "IPlugin.h"

class BasePlugin : public IPlugin
{
    Q_OBJECT

public:
    explicit BasePlugin(QObject *parent = nullptr);
    virtual ~BasePlugin() = default;

    bool initialize(const QVariantMap &config = QVariantMap()) override;
    void shutdown() override;

    bool hasSettings() const override;
    QVariantMap getSettings() const override;
    bool setSettings(const QVariantMap &settings) override;

protected:
    virtual bool onInitialize(const QVariantMap &config) = 0;
    virtual void onShutdown() = 0;
    virtual bool onSetSettings(const QVariantMap &settings) = 0;
    virtual QVariantMap onGetSettings() const = 0;

    QVariantMap m_settings;
    bool m_initialized = false;
};