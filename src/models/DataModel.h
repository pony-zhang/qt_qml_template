#pragma once

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QAbstractListModel>
#include <QList>
#include <QJsonObject>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(appModels)

struct DataItem {
    QString id;
    QString name;
    QString description;
    int value;
    bool enabled;

    bool operator==(const DataItem &other) const {
        return id == other.id;
    }
};

class DataModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum DataRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        DescriptionRole,
        ValueRole,
        EnabledRole
    };

    explicit DataModel(QObject *parent = nullptr);
    ~DataModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addItem(const QString &name, const QString &description, int value);
    Q_INVOKABLE void removeItem(const QString &id);
    Q_INVOKABLE void updateItemValue(const QString &id, int value);
    Q_INVOKABLE void setItemEnabled(const QString &id, bool enabled);
    Q_INVOKABLE void clear();

    Q_INVOKABLE QJsonObject getItem(const QString &id) const;
    Q_INVOKABLE int getCount() const;

private:
    QList<DataItem> m_items;

    int findItemIndex(const QString &id) const;
};