#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include <QLoggingCategory>
#include <QVariantMap>

#include "core/Application.h"
#include "core/QmlTypeRegistry.h"
#include "plugin/PluginLoader.h"


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

        // Register QML types
        auto* qmlRegistry = QmlTypeRegistry::instance();
        qmlRegistry->registerAllTypes();

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
            qCritical() << "Failed to load QML file";
            return -1;
        }

        return app.exec();
    }
    catch (const std::exception& e) {
        qCritical() << "Application error:" << e.what();
        return -1;
    }
    catch (...) {
        qCritical() << "Unknown application error";
        return -1;
    }
}
