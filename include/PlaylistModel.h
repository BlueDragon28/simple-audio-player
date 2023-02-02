#ifndef SIMPLEAUDIOPLAYER_PLAYLISTMODEL_H_
#define SIMPLEAUDIOPLAYER_PLAYLISTMODEL_H_

#include "SelectionModel.h"
#include <qhash.h>
#include <qlist.h>
#include <qnamespace.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class PlaylistModel : public SelectionModel
{
    Q_OBJECT
    QML_ELEMENT

    enum TracksRole {
        FILEPATH = MAX_ROLE,
        NAME,
        ARTISTS
    };

public slots:
    /*
    Add files to the list.
    It retrieve the tags from the file.
    */
    void add(const QVariantList& vFilePath);

public:
    PlaylistModel(QObject* parent = nullptr);
    virtual ~PlaylistModel();

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

private:
    struct Track {
        QString filepath;
        QString name;
        QString artists;
    };
};

#endif // SIMPLEAUDIOPLAYER_PLAYLISTMODEL_H_
