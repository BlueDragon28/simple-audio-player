#ifndef SIMPLEAUDIOPLAYER_SPOTIFYPLAYLISTITEM_H_
#define SIMPLEAUDIOPLAYER_SPOTIFYPLAYLISTITEM_H_

#include <qobject.h>
#include <qmetaobject.h>
#include <qtmetamacros.h>
#include <qstring.h>
#include <qurl.h>

class SpotifyPlaylistItem : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QUrl imageUrl READ imageUrl WRITE setImageUrl NOTIFY imageUrlChanged)

public:
    SpotifyPlaylistItem(const QString& title, const QUrl& url, QObject* parent = nullptr);
    virtual ~SpotifyPlaylistItem();

    QString title() const;
    QUrl imageUrl() const;

    bool operator==(const SpotifyPlaylistItem& other) const;
    operator QString() const;

public slots:
    void setTitle(const QString& title);
    void setImageUrl(const QUrl& imageUrl);

signals:
    void titleChanged();
    void imageUrlChanged();

private:
    QString m_title;
    QUrl m_imageUrl;
};

#endif // SIMPLEAUDIOPLAYER_SPOTIFYPLAYLISTITEM_H_