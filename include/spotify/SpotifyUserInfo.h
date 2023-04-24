#ifndef SIMPLEAUDIOPLAYER_SPOTIFYUSERINFO_H_
#define SIMPLEAUDIOPLAYER_SPOTIFYUSERINFO_H_

#include <qobject.h>
#include <qtmetamacros.h>
#include <qqmlintegration.h>

class SpotifyUserInfo : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString country READ country WRITE setCountry NOTIFY countryChanged)
    Q_PROPERTY(int followers READ followers WRITE setFollowers NOTIFY followersChanged)

public:
    SpotifyUserInfo(QObject* parent = nullptr);
    virtual ~SpotifyUserInfo();

    QString username() const;
    QString country() const;
    int followers() const;

public slots:
    void setUsername(const QString& username);
    void setCountry(const QString& country);
    void setFollowers(int followers);

signals:
    void usernameChanged();
    void countryChanged();
    void followersChanged();

private:
    QString m_username;
    QString m_country;
    int m_followers;
};

#endif // SIMPLEAUDIOPLAYER_SPOTIFYUSERINFO_H_

