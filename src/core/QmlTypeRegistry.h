#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>

class QmlTypeRegistry : public QObject
{
    Q_OBJECT

public:
    static QmlTypeRegistry* instance();
    ~QmlTypeRegistry();

    void registerApplicationTypes();
    void registerBusinessTypes();
    void registerDataTypes();
    void registerUtilityTypes();
    void registerAllTypes();

private:
    explicit QmlTypeRegistry(QObject *parent = nullptr);
    static QmlTypeRegistry* s_instance;
    Q_DISABLE_COPY(QmlTypeRegistry)
};