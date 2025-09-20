#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include <QLoggingCategory>
#include <QVariantMap>

#include "core/Application.h"
#include "business/viewmodels/AppViewModel.h"
#include "business/viewmodels/DataViewModel.h"
#include "data/models/DataModel.h"
#include "plugin/PluginLoader.h"

Q_LOGGING_CATEGORY(appMain, "app.main")

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setOrganizationName("Example");
    app.setOrganizationDomain("example.com");
    app.setApplicationName("QML C++ Template");
    app.setApplicationVersion("1.0.0");

    QLoggingCategory::setFilterRules("app.*.debug=true");

    try {
        Application application;

        QVariantMap pluginConfig;
        pluginConfig["pluginDir"] = QCoreApplication::applicationDirPath() + "/plugins";

        auto *pluginLoader = PluginLoader::instance();
        pluginLoader->loadPluginSystem(pluginConfig);

        QQmlApplicationEngine engine;

        AppViewModel appViewModel;
        engine.rootContext()->setContextProperty("appViewModel", &appViewModel);

        DataViewModel dataViewModel;
        engine.rootContext()->setContextProperty("dataViewModel", &dataViewModel);

        engine.addImportPath(QStringLiteral("qrc:/"));

        const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
        QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                         &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) {
                QCoreApplication::exit(1);
            }
        }, Qt::QueuedConnection);

        engine.load(url);

        if (engine.rootObjects().isEmpty()) {
            qCritical(appMain) << "Failed to load QML file";
            return -1;
        }

        return app.exec();
    }
    catch (const std::exception& e) {
        qCritical(appMain) << "Application error:" << e.what();
        return -1;
    }
    catch (...) {
        qCritical(appMain) << "Unknown application error";
        return -1;
    }
}
