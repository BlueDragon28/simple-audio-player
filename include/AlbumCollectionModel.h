#ifndef SAMPLEMODELPLAYER_ALBUMCOLLECTIONMODEL_H
#define SAMPLEMODELPLAYER_ALBUMCOLLECTIONMODEL_H

#include "BaseCollectionModel.h"
#include <QtQml/qqmlcomponent.h>

class AlbumCollectionModel : public BaseCollectionModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    AlbumCollectionModel();
    ~AlbumCollectionModel();
};

#endif // SAMPLEMODELPLAYER_ALBUMCOLLECTIONMODEL_H
