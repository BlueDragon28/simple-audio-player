#include "BaseCollectionModel.h"

BaseCollectionModel::BaseCollectionModel(const QString& tableName) :
    m_musicCollection(MusicCollectionList::instance()),
    m_numberOfItems(0),
    m_sqlTableName(tableName)
{
    // When the music collection is updated, update this collection.
    connect(m_musicCollection, &MusicCollectionList::listUpdated,
            this, &BaseCollectionModel::updateItemsList);
}

BaseCollectionModel::~BaseCollectionModel()
{}

int BaseCollectionModel::rowCount(const QModelIndex &parent) const
{
    return m_numberOfItems;
}

QVariant BaseCollectionModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

QHash<int, QByteArray> BaseCollectionModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    return roles;
}

void BaseCollectionModel::updateItemsList()
{
    // If the list have albums in it, remove everything.
    if (m_numberOfItems > 0)
    {
        beginRemoveRows(QModelIndex(), 0, m_numberOfItems-1);
        endRemoveRows();
        m_numberOfItems = 0;
    }

    // Retrieve the names in the sql table.
    m_itemsList = m_musicCollection->retrieveNamesFromTable(m_sqlTableName);

    // Notify the view of the change.
    if (!m_itemsList.isEmpty())
    {
        m_numberOfItems = m_itemsList.size();
        beginInsertRows(QModelIndex(), 0, m_numberOfItems-1);
        endInsertColumns();
    }
}
