#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(appViewModel)

class AppViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit AppViewModel(QObject *parent = nullptr);
    ~AppViewModel();

    Q_INVOKABLE void showToast(const QString &message, int duration = 3000);
    Q_INVOKABLE void showNotification(const QString &title, const QString &message);
    Q_INVOKABLE void openUrl(const QString &url);
    Q_INVOKABLE void setTheme(const QString &theme);
    Q_INVOKABLE QString getTheme() const;

signals:
    void themeChanged(const QString &theme);
    void notificationRequested(const QString &title, const QString &message);

private:
    QString m_currentTheme;
};