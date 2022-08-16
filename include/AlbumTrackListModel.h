#ifndef ALBUMTRACKSLISTMODEL_H
#define ALBUMTRACKSLISTMODEL_H

#include "SelectionModel.h"
#include <QFileInfo>
#include <QList>
#include <QtQml/qqmlcomponent.h>
#include "MusicCollectionList.h"

/*
This model store the tracks inside an album.
*/
class AlbumTracksListModel : public SelectionModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString album READ album WRITE setAlbum NOTIFY albumUpdated)

public:
    enum Roles
    {
        TRACK_NAME = MAX_ROLE,
        ARTISTS,
        FILE_PATH,
    };

    AlbumTracksListModel(QObject* parent = nullptr);
    virtual ~AlbumTracksListModel();

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

signals:
    void albumUpdated();

private:
    /*
    Retrieve the tracks inside an album.
    */
    void retrieveTracksFromAlbumName();

    /*
    Update album name.
    */
    QString album() const;
    void setAlbum(const QString& album);

    QString m_albumName;
};

#endif // ALBUMTRACKSLISTMODEL_H
