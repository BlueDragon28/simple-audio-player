#ifndef SIMPLEAUDIOPLAYER_PLAYLISTMODEL_H_
#define SIMPLEAUDIOPLAYER_PLAYLISTMODEL_H_

#include "SelectionModel.h"
#include <qhash.h>
#include <qnamespace.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class PlaylistModel : public SelectionModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    PlaylistModel(QObject* parent = nullptr);
    virtual ~PlaylistModel();

    virtual QVariant data(const QModelIndex& parent, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

private:

};

#endif // SIMPLEAUDIOPLAYER_PLAYLISTMODEL_H_