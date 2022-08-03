#ifndef SIMPLEAUDIOPLAYER_BASECOLLECTIONMODEL_H
#define SIMPLEAUDIOPLAYER_BASECOLLECTIONMODEL_H

#include <QAbstractListModel>
#include "MusicCollectionList.h"

class BaseCollectionModel : public QAbstractListModel
{
    Q_OBJECT

public:
    BaseCollectionModel(const QString& tableName);
    virtual ~BaseCollectionModel();

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

private:
    // Update the list of the albums list.
    void updateItemsList();

    MusicCollectionList* m_musicCollection;
    int m_numberOfItems;

    QString m_sqlTableName;
    QStringList m_itemsList;
};

#endif // SIMPLEAUDIOPLAYER_BASECOLLECTIONMODEL_H
