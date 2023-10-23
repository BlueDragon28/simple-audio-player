#include "CoverCache.h"
#include "NetworkManager.h"
#include <optional>
#include <qassert.h>
#include <qdir.h>
#include <qimage.h>
#include <qjsengine.h>
#include <qnetworkreply.h>
#include <qstandardpaths.h>

CoverCache* CoverCache::_instance = nullptr;
QJSEngine* CoverCache::_jsEngine = nullptr;
QString CoverCache::_imagesDirectory = "coverImages";
QString CoverCache::_imagesJsonPath = "coversPath.json";

CoverCache::CoverCache() :
    m_appDataPath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation))
{}
CoverCache::~CoverCache() {}

void CoverCache::createInstance() {
    if (_instance) return;
    _instance = new CoverCache();
}

void CoverCache::destroyInstance() {
    if (!_instance) return;
    delete _instance;
    _instance = nullptr;
}

CoverCache* CoverCache::create(QQmlEngine* qmlEngine, QJSEngine* jsEngine) {
    Q_ASSERT(_instance);
    Q_ASSERT(jsEngine->thread() == _instance->thread());

    if (_jsEngine) {
        Q_ASSERT(jsEngine == _jsEngine);
    } else {
        _jsEngine = jsEngine;
    }

    QJSEngine::setObjectOwnership(_instance, QJSEngine::CppOwnership);
    return _instance;
}

CoverCache* CoverCache::instance() {
    return _instance;
}

QUrl CoverCache::getImage(const QString& id, const QUrl& imageURL) {
    const auto foundCover = getImagePathFromCache(id);

    if (foundCover) {
        return QUrl::fromLocalFile(foundCover.value());
    }
    retrieveImageFromEndpoint(id, imageURL);

    return imageURL;
}

std::optional<QString> CoverCache::getImagePathFromCache(const QString& id) {
    Q_ASSERT(m_appDataPath.size() > 0);
    makeSureAppLocationIsCreated(m_appDataPath);

    for (const auto& cover : m_allCoversInfo) {
        if (id != cover.id) continue;

        return m_appDataPath + "/" + _imagesDirectory + "/" + cover.name;
    }

    return std::nullopt;
}

void CoverCache::retrieveImageFromEndpoint(const QString& id, const QUrl& imageURL) {
    QNetworkReply* reply = NetworkManager::accessManager()->get(QNetworkRequest(
        imageURL
    ));
    connect(reply, &QNetworkReply::finished, [this, reply, id]() {
        this->handleImageResponse(reply, id);
    });
}

void CoverCache::handleImageResponse(QNetworkReply* reply, const QString& id) {
    QByteArray data = reply->readAll();
    reply->deleteLater();

    makeSureAppLocationIsCreated(m_appDataPath + "/" + _imagesDirectory);

    auto image = QImage::fromData(data);
    if (!image.isNull()) {
        image.save(m_appDataPath + "/" + _imagesDirectory + "/" + id + ".jpg", "jpg");
        CoverStruct coverStruct = {
            .id = id,
            .name = id + ".jpg",
        };
        m_allCoversInfo.append(coverStruct);
    }
}

void CoverCache::makeSureAppLocationIsCreated(const QString path) {
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}
