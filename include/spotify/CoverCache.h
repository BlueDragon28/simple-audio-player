#ifndef SIMPLE_AUDIO_PLAYER_SPOTIFY_COVER_CACHE_H_
#define SIMPLE_AUDIO_PLAYER_SPOTIFY_COVER_CACHE_H_

#include <optional>
#include <qjsengine.h>
#include <qnetworkreply.h>
#include <qobject.h>
#include <qqmlengine.h>
#include <qqmlintegration.h>

class CoverCache : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    CoverCache();
    ~CoverCache();

    struct CoverStruct {
        QString id;
        QString name;
    };

    static void createInstance();
    static void destroyInstance();
    static CoverCache* create(QQmlEngine* qmlEngine, QJSEngine* jsEngine);
    static CoverCache* instance();

public slots:
    QUrl getImage(const QString& id, const QUrl& imageURL);

private:
    std::optional<QString> getImagePathFromCache(const QString& id);
    void retrieveImageFromEndpoint(const QString& id, const QUrl& imageURL);
    void handleImageResponse(QNetworkReply* reply, const QString& id);

    QList<CoverStruct> m_allCoversInfo;
    QString m_appDataPath;

    static void makeSureAppLocationIsCreated(const QString path);

    static CoverCache* _instance;
    static QJSEngine* _jsEngine;
    static QString _imagesDirectory;
    static QString _imagesJsonPath;
};

#endif // SIMPLE_AUDIO_PLAYER_SPOTIFY_COVER_CACHE_H_
