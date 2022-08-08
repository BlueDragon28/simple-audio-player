#ifndef ALBUMTRACKSLISTMODEL_H
#define ALBUMTRACKSLISTMODEL_H

#include <QAbstractListModel>
#include <QFileInfo>
#include <QList>
#include <QtQml/qqmlcomponent.h>
#include "MusicCollectionList.h"

/*
This model store the tracks inside an album.
*/
class AlbumTracksListModel : public QAbstractItemModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum Roles
    {
        TRACK_NAME,
        ARTISTS,
    };

    AlbumTracksListModel();
    virtual ~AlbumTracksListModel();

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

private:
    /*
    Retrieve the tracks inside an album.
    */
    void retrieveTracksFromAlbumName();

    QString m_albumName;
    QList<MusicCollectionList::TrackInfo> m_trackList;
};

#endif // ALBUMTRACKSLISTMODEL_H
