#pragma once

#include <QObject>
#include <QLoggingCategory>
#include <QDateTime>
#include <QMetaType>

Q_DECLARE_LOGGING_CATEGORY(appDataService)

class DataService : public QObject
{
    Q_OBJECT

public:
    explicit DataService(QObject *parent = nullptr);
    ~DataService();

    // Business logic for data operations
    bool validateItemName(const QString &name) const;
    bool validateItemValue(int value) const;
    QString generateItemId() const;
    bool isItemIdUnique(const QString &id) const;

    // Data processing logic
    QVariantMap processItemData(const QVariantMap &inputData) const;
    QVariantList filterItems(const QVariantList &items, const QString &filter) const;
    QVariantList sortItems(const QVariantList &items, const QString &sortBy, bool ascending = true) const;

signals:
    void dataValidated(const QString &itemId, bool isValid);
    void dataProcessed(const QVariantMap &result);

private:
    QStringList m_existingIds;
};