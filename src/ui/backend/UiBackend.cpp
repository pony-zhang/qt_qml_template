#include "UiBackend.h"
#include <QDesktopServices>
#include <QUrl>
#include <QTimer>

Q_LOGGING_CATEGORY(appUiBackend, "app.ui.backend")

UiBackend::UiBackend(QQmlEngine *engine, QObject *parent)
    : QObject(parent)
    , m_engine(engine)
    , m_currentTheme("light")
{
    qCDebug(appUiBackend) << "UiBackend initialized";
}

UiBackend::~UiBackend()
{
    qCDebug(appUiBackend) << "UiBackend destroyed";
}

void UiBackend::showToast(const QString &message, int duration)
{
    qCDebug(appUiBackend) << "Showing toast:" << message;
    emit showToast(message, duration);
}

void UiBackend::showNotification(const QString &title, const QString &message)
{
    qCDebug(appUiBackend) << "Showing notification:" << title << message;
    emit notificationRequested(title, message);
}

void UiBackend::openUrl(const QString &url)
{
    qCDebug(appUiBackend) << "Opening URL:" << url;
    QDesktopServices::openUrl(QUrl(url));
}

void UiBackend::setTheme(const QString &theme)
{
    if (m_currentTheme != theme) {
        m_currentTheme = theme;
        qCDebug(appUiBackend) << "Theme changed to:" << theme;
        emit themeChanged(theme);
    }
}

QString UiBackend::getTheme() const
{
    return m_currentTheme;
}