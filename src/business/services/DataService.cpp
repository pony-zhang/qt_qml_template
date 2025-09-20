#include "DataService.h"
#include <QUuid>
#include <QRegularExpression>
#include <QVariant>

Q_LOGGING_CATEGORY(appDataService, "app.dataservice")

DataService::DataService(QObject *parent)
    : QObject(parent)
{
    qDebug(appDataService) << "DataService initialized";
}

DataService::~DataService()
{
    qDebug(appDataService) << "DataService destroyed";
}

bool DataService::validateItemName(const QString &name) const
{
    if (name.isEmpty()) return false;
    if (name.length() < 2 || name.length() > 50) return false;

    // Check for valid characters (letters, numbers, spaces, basic punctuation)
    QRegularExpression regex("^[a-zA-Z0-9\\s\\-_.]+$");
    return regex.match(name).hasMatch();
}

bool DataService::validateItemValue(int value) const
{
    return value >= 0 && value <= 1000; // Example validation rules
}

QString DataService::generateItemId() const
{
    QString id;
    do {
        id = QUuid::createUuid().toString(QUuid::WithoutBraces).left(8);
    } while (!isItemIdUnique(id));

    return id;
}

bool DataService::isItemIdUnique(const QString &id) const
{
    return !m_existingIds.contains(id);
}

QVariantMap DataService::processItemData(const QVariantMap &inputData) const
{
    QVariantMap result = inputData;

    // Add processing timestamp
    result["processedAt"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    // Add validation status
    bool nameValid = validateItemName(inputData["name"].toString());
    bool valueValid = validateItemValue(inputData["value"].toInt());
    result["isValid"] = nameValid && valueValid;

    return result;
}

QVariantList DataService::filterItems(const QVariantList &items, const QString &filter) const
{
    if (filter.isEmpty()) return items;

    QVariantList filtered;
    QString lowerFilter = filter.toLower();

    for (const QVariant &item : items) {
        QVariantMap itemMap = item.toMap();
        QString name = itemMap["name"].toString().toLower();
        QString description = itemMap["description"].toString().toLower();

        if (name.contains(lowerFilter) || description.contains(lowerFilter)) {
            filtered.append(item);
        }
    }

    return filtered;
}

QVariantList DataService::sortItems(const QVariantList &items, const QString &sortBy, bool ascending) const
{
    QVariantList sorted = items;

    std::sort(sorted.begin(), sorted.end(),
        [sortBy, ascending](const QVariant &a, const QVariant &b) {
            QVariantMap aMap = a.toMap();
            QVariantMap bMap = b.toMap();

            QVariant aValue = aMap[sortBy];
            QVariant bValue = bMap[sortBy];

            if (aValue.userType() == QMetaType::Int && bValue.userType() == QMetaType::Int) {
                return ascending ? aValue.toInt() < bValue.toInt() : aValue.toInt() > bValue.toInt();
            } else {
                return ascending ? aValue.toString() < bValue.toString() : aValue.toString() > bValue.toString();
            }
        });

    return sorted;
}