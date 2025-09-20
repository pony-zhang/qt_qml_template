#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(appUiBackend)

class UiBackend : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit UiBackend(QQmlEngine *engine, QObject *parent = nullptr);
    ~UiBackend();

    Q_INVOKABLE void showToast(const QString &message, int duration = 3000);
    Q_INVOKABLE void showNotification(const QString &title, const QString &message);
    Q_INVOKABLE void openUrl(const QString &url);
    Q_INVOKABLE void setTheme(const QString &theme);
    Q_INVOKABLE QString getTheme() const;

signals:
    void themeChanged(const QString &theme);
    void notificationRequested(const QString &title, const QString &message);

private:
    QQmlEngine *m_engine;
    QString m_currentTheme;
};