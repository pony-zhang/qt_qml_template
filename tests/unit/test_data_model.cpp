#include <QtTest>
#include <QSignalSpy>
#include "models/DataModel.h"

class TestDataModel : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testAddItem();
    void testRemoveItem();
    void testUpdateItem();
    void testGetItem();
    void testClear();
    void testRowCount();
    void testData();

private:
    DataModel* m_model;
};

void TestDataModel::initTestCase()
{
    m_model = new DataModel();
}

void TestDataModel::cleanupTestCase()
{
    delete m_model;
}

void TestDataModel::testAddItem()
{
    QSignalSpy rowsInsertedSpy(m_model, &DataModel::rowsInserted);

    m_model->addItem("Test Item", "Test Description", 42);

    QCOMPARE(rowsInsertedSpy.count(), 1);
    QCOMPARE(m_model->getCount(), 1);

    QJsonObject item = m_model->getItem(rowsInsertedSpy[0][1].toInt());
    QCOMPARE(item["name"].toString(), QString("Test Item"));
    QCOMPARE(item["value"].toInt(), 42);
}

void TestDataModel::testRemoveItem()
{
    m_model->addItem("To Remove", "Will be removed", 100);
    QString id = m_model->getItem(0)["id"].toString();

    QSignalSpy rowsRemovedSpy(m_model, &DataModel::rowsRemoved);

    m_model->removeItem(id);

    QCOMPARE(rowsRemovedSpy.count(), 1);
    QCOMPARE(m_model->getCount(), 0);
}

void TestDataModel::testUpdateItem()
{
    m_model->addItem("To Update", "Original", 50);
    QString id = m_model->getItem(0)["id"].toString();

    QSignalSpy dataChangedSpy(m_model, &DataModel::dataChanged);

    m_model->updateItemValue(id, 99);

    QCOMPARE(dataChangedSpy.count(), 1);
    QCOMPARE(m_model->getItem(0)["value"].toInt(), 99);
}

void TestDataModel::testGetItem()
{
    m_model->addItem("Get Test", "Description", 25);
    QString id = m_model->getItem(0)["id"].toString();

    QJsonObject item = m_model->getItem(id);

    QCOMPARE(item["name"].toString(), QString("Get Test"));
    QCOMPARE(item["value"].toInt(), 25);
    QCOMPARE(item["enabled"].toBool(), true);
}

void TestDataModel::testClear()
{
    m_model->addItem("Item 1", "Desc 1", 1);
    m_model->addItem("Item 2", "Desc 2", 2);

    QSignalSpy modelResetSpy(m_model, &DataModel::modelReset);

    m_model->clear();

    QCOMPARE(modelResetSpy.count(), 1);
    QCOMPARE(m_model->getCount(), 0);
}

void TestDataModel::testRowCount()
{
    m_model->clear();

    QCOMPARE(m_model->rowCount(), 0);

    m_model->addItem("Test", "Test", 1);
    QCOMPARE(m_model->rowCount(), 1);

    m_model->addItem("Test 2", "Test 2", 2);
    QCOMPARE(m_model->rowCount(), 2);
}

void TestDataModel::testData()
{
    m_model->addItem("Data Test", "Data Description", 75);

    QModelIndex index = m_model->index(0, 0);

    QCOMPARE(m_model->data(index, DataModel::NameRole).toString(), QString("Data Test"));
    QCOMPARE(m_model->data(index, DataModel::ValueRole).toInt(), 75);
    QCOMPARE(m_model->data(index, DataModel::EnabledRole).toBool(), true);
}

QTEST_APPLESS_MAIN(TestDataModel)
#include "test_data_model.moc"