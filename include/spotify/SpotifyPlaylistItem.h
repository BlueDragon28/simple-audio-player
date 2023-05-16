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
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)

public:
    SpotifyPlaylistItem(const QString& title, const QUrl& url, const QUrl& href, const QString& id, QObject* parent = nullptr);
    virtual ~SpotifyPlaylistItem();

    QString title() const;
    QUrl imageUrl() const;
    QUrl href() const;
    QString id() const;

    bool operator==(const SpotifyPlaylistItem& other) const;
    operator QString() const;

public slots:
    void setTitle(const QString& title);
    void setImageUrl(const QUrl& imageUrl);
    void setHRef(const QUrl& href);
    void setId(const QString& id);

signals:
    void titleChanged();
    void imageUrlChanged();
    void hrefChanged();
    void idChanged();

private:
    QString m_title;
    QUrl m_imageUrl;
    QUrl m_href;
    QString m_id;
};

#endif // SIMPLEAUDIOPLAYER_SPOTIFYPLAYLISTITEM_H_