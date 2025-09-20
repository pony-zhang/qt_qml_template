#include "AppViewModel.h"
#include <QDesktopServices>
#include <QUrl>

Q_LOGGING_CATEGORY(appViewModel, "app.viewmodel")

AppViewModel::AppViewModel(QObject *parent)
    : QObject(parent)
    , m_currentTheme("light")
{
    qDebug(appViewModel) << "AppViewModel initialized";
}

AppViewModel::~AppViewModel()
{
    qDebug(appViewModel) << "AppViewModel destroyed";
}

void AppViewModel::showToast(const QString &message, int duration)
{
    qDebug(appViewModel) << "Showing toast:" << message;
    emit showToast(message, duration);
}

void AppViewModel::showNotification(const QString &title, const QString &message)
{
    qDebug(appViewModel) << "Showing notification:" << title << message;
    emit notificationRequested(title, message);
}

void AppViewModel::openUrl(const QString &url)
{
    qDebug(appViewModel) << "Opening URL:" << url;
    QDesktopServices::openUrl(QUrl(url));
}

void AppViewModel::setTheme(const QString &theme)
{
    if (m_currentTheme != theme) {
        m_currentTheme = theme;
        qDebug(appViewModel) << "Theme changed to:" << theme;
        emit themeChanged(theme);
    }
}

QString AppViewModel::getTheme() const
{
    return m_currentTheme;
}