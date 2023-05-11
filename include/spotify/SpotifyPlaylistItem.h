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
    Q_PROPERTY(QUrl href READ href WRITE setHRef NOTIFY hrefChanged)

public:
    SpotifyPlaylistItem(const QString& title, const QUrl& url, const QUrl& href, QObject* parent = nullptr);
    virtual ~SpotifyPlaylistItem();

    QString title() const;
    QUrl imageUrl() const;
    QUrl href() const;

    bool operator==(const SpotifyPlaylistItem& other) const;
    operator QString() const;

public slots:
    void setTitle(const QString& title);
    void setImageUrl(const QUrl& imageUrl);
    void setHRef(const QUrl& href);

signals:
    void titleChanged();
    void imageUrlChanged();
    void hrefChanged();

private:
    QString m_title;
    QUrl m_imageUrl;
    QUrl m_href;
};

#endif // SIMPLEAUDIOPLAYER_SPOTIFYPLAYLISTITEM_H_