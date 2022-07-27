#ifndef SIMPLEAUDIOPLAYER_COVERIMAGEPROVIDER_H_
#define SIMPLEAUDIOPLAYER_COVERIMAGEPROVIDER_H_

#include <QQuickImageProvider>
#include <QPixmap>

/*
This provider class is used to get access of the artist image from QML.
*/
class CoverImageProvider : public QQuickImageProvider
{
public:
    CoverImageProvider();

    virtual QPixmap requestPixmap(const QString& id, QSize* size, const QSize& requestedSize) override;
};

#endif // SIMPLEAUDIOPLAYER_COVERIMAGEPROVIDER_H_