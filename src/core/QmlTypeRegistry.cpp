#include "QmlTypeRegistry.h"
#include <QDebug>
#include "../business/viewmodels/AppViewModel.h"
#include "../business/viewmodels/DataViewModel.h"


QmlTypeRegistry* QmlTypeRegistry::s_instance = nullptr;

QmlTypeRegistry* QmlTypeRegistry::instance()
{
    if (!s_instance) {
        s_instance = new QmlTypeRegistry();
    }
    return s_instance;
}

QmlTypeRegistry::QmlTypeRegistry(QObject *parent)
    : QObject(parent)
{
    qDebug() << "QmlTypeRegistry initialized";
}

QmlTypeRegistry::~QmlTypeRegistry()
{
    qDebug() << "QmlTypeRegistry destroyed";
}

void QmlTypeRegistry::registerApplicationTypes()
{
    qDebug() << "Registering application types";

    // Register application types here if needed
}

void QmlTypeRegistry::registerBusinessTypes()
{
    qDebug() << "Registering business types";

    // Register business viewmodels
    qmlRegisterType<AppViewModel>("com.example.app", 1, 0, "AppViewModel");
    qmlRegisterType<DataViewModel>("com.example.app", 1, 0, "DataViewModel");
}

void QmlTypeRegistry::registerDataTypes()
{
    qDebug() << "Registering data types";

    // Register data models here if needed
}

void QmlTypeRegistry::registerUtilityTypes()
{
    qDebug() << "Registering utility types";

    // Register utility types here if needed
}

void QmlTypeRegistry::registerAllTypes()
{
    qDebug() << "Registering all QML types";

    registerApplicationTypes();
    registerBusinessTypes();
    registerDataTypes();
    registerUtilityTypes();
}