#include "PlaylistModel.h"
#include "SelectionModel.h"
#include <qabstractitemmodel.h>
#include <qhash.h>

PlaylistModel::PlaylistModel(QObject* parent) :
    SelectionModel(parent)
{}

PlaylistModel::~PlaylistModel()
{}

QVariant PlaylistModel::data(const QModelIndex& parent, int role) const
{
    return QVariant();
}

QHash<int, QByteArray> PlaylistModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    return roles;
}