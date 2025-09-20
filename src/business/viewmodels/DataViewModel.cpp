#include "DataViewModel.h"


DataViewModel::DataViewModel(QObject *parent)
    : QObject(parent)
    , m_model(new DataModel(this))
{
    qDebug() << "DataViewModel initialized";
}

DataViewModel::~DataViewModel()
{
    qDebug() << "DataViewModel destroyed";
}

DataModel* DataViewModel::model() const
{
    return m_model;
}

void DataViewModel::addItem(const QString &name, const QString &description, int value)
{
    m_model->addItem(name, description, value);
    emit itemAdded(m_model->index(m_model->rowCount() - 1).data(DataModel::IdRole).toString());
}

void DataViewModel::removeItem(const QString &id)
{
    m_model->removeItem(id);
    emit itemRemoved(id);
}

void DataViewModel::updateItemValue(const QString &id, int value)
{
    m_model->updateItemValue(id, value);
    emit itemUpdated(id);
}

void DataViewModel::setItemEnabled(const QString &id, bool enabled)
{
    m_model->setItemEnabled(id, enabled);
    emit itemUpdated(id);
}

void DataViewModel::clear()
{
    m_model->clear();
}

QVariant DataViewModel::getItem(const QString &id) const
{
    return m_model->getItem(id);
}

int DataViewModel::getCount() const
{
    return m_model->getCount();
}