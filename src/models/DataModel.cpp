#include "DataModel.h"
#include <QUuid>

Q_LOGGING_CATEGORY(appModels, "app.models")

DataModel::DataModel(QObject *parent)
    : QAbstractListModel(parent)
{
    qCDebug(appModels) << "DataModel initialized";
}

DataModel::~DataModel()
{
    qCDebug(appModels) << "DataModel destroyed";
}

int DataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_items.size();
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.size()) {
        return QVariant();
    }

    const DataItem &item = m_items[index.row()];

    switch (role) {
    case IdRole:
        return item.id;
    case NameRole:
        return item.name;
    case DescriptionRole:
        return item.description;
    case ValueRole:
        return item.value;
    case EnabledRole:
        return item.enabled;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> DataModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[DescriptionRole] = "description";
    roles[ValueRole] = "value";
    roles[EnabledRole] = "enabled";
    return roles;
}

void DataModel::addItem(const QString &name, const QString &description, int value)
{
    QString id = QUuid::createUuid().toString(QUuid::WithoutBraces);

    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items.append({id, name, description, value, true});
    endInsertRows();

    qCDebug(appModels) << "Added item:" << id << name;
}

void DataModel::removeItem(const QString &id)
{
    int index = findItemIndex(id);
    if (index >= 0) {
        beginRemoveRows(QModelIndex(), index, index);
        m_items.removeAt(index);
        endRemoveRows();
        qCDebug(appModels) << "Removed item:" << id;
    }
}

void DataModel::updateItemValue(const QString &id, int value)
{
    int index = findItemIndex(id);
    if (index >= 0) {
        m_items[index].value = value;
        emit dataChanged(createIndex(index, 0), createIndex(index, 0), {ValueRole});
        qCDebug(appModels) << "Updated item value:" << id << value;
    }
}

void DataModel::setItemEnabled(const QString &id, bool enabled)
{
    int index = findItemIndex(id);
    if (index >= 0) {
        m_items[index].enabled = enabled;
        emit dataChanged(createIndex(index, 0), createIndex(index, 0), {EnabledRole});
        qCDebug(appModels) << "Set item enabled:" << id << enabled;
    }
}

void DataModel::clear()
{
    beginResetModel();
    m_items.clear();
    endResetModel();
    qCDebug(appModels) << "Cleared all items";
}

QJsonObject DataModel::getItem(const QString &id) const
{
    int index = findItemIndex(id);
    if (index >= 0) {
        const DataItem &item = m_items[index];
        QJsonObject obj;
        obj["id"] = item.id;
        obj["name"] = item.name;
        obj["description"] = item.description;
        obj["value"] = item.value;
        obj["enabled"] = item.enabled;
        return obj;
    }
    return QJsonObject();
}

int DataModel::getCount() const
{
    return m_items.size();
}

int DataModel::findItemIndex(const QString &id) const
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i].id == id) {
            return i;
        }
    }
    return -1;
}