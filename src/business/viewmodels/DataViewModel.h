#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QLoggingCategory>
#include "../../data/models/DataModel.h"


class DataViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(DataModel* model READ model CONSTANT)

public:
    explicit DataViewModel(QObject *parent = nullptr);
    ~DataViewModel();

    DataModel* model() const;

    Q_INVOKABLE void addItem(const QString &name, const QString &description, int value);
    Q_INVOKABLE void removeItem(const QString &id);
    Q_INVOKABLE void updateItemValue(const QString &id, int value);
    Q_INVOKABLE void setItemEnabled(const QString &id, bool enabled);
    Q_INVOKABLE void clear();

    Q_INVOKABLE QVariant getItem(const QString &id) const;
    Q_INVOKABLE int getCount() const;

signals:
    void itemAdded(const QString &id);
    void itemRemoved(const QString &id);
    void itemUpdated(const QString &id);

private:
    DataModel* m_model;
};