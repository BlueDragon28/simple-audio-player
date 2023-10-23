#include "CoverCache.h"
#include "NetworkManager.h"
#include <optional>
#include <qassert.h>
#include <qdir.h>
#include <qimage.h>
#include <qjsengine.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qnetworkreply.h>
#include <qstandardpaths.h>
#include <qsavefile.h>

CoverCache* CoverCache::_instance = nullptr;
QJSEngine* CoverCache::_jsEngine = nullptr;
QString CoverCache::_imagesDirectory = "coverImages";
QString CoverCache::_imagesJsonPath = "coversPath.json";

CoverCache::CoverCache() :
    m_appDataPath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation))
{
    makeSureAppLocationIsCreated(m_appDataPath);
    makeSureAppLocationIsCreated(m_appDataPath + "/" + _imagesDirectory);
    loadCoversConfigFile();
}
CoverCache::~CoverCache() {}

void CoverCache::createInstance() {
    if (_instance) return;
    _instance = new CoverCache();
}

void CoverCache::destroyInstance() {
    if (!_instance) return;
    _instance->saveCoversConfigFile();
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

    return QUrl();
}

std::optional<QString> CoverCache::getImagePathFromCache(const QString& id) {
    Q_ASSERT(m_appDataPath.size() > 0);

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

    auto image = QImage::fromData(data);
    if (!image.isNull()) {
        const QString imagePath = m_appDataPath + "/" + _imagesDirectory + "/" + id + ".jpg";
        bool result = image.save(imagePath, "jpg");
        if (!result) return;

        CoverStruct coverStruct = {
            .id = id,
            .name = id + ".jpg",
        };
        m_allCoversInfo.append(coverStruct);
        emit imageReceived(id, QUrl::fromLocalFile(imagePath));
    }
}

void CoverCache::makeSureAppLocationIsCreated(const QString path) {
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

void CoverCache::loadCoversConfigFile() {
    const QString configPath = m_appDataPath + "/" + _imagesJsonPath;

    QFile file(configPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    const QByteArray data = file.readAll();
    QJsonParseError jsonError;
    const QJsonDocument jsonDocument = QJsonDocument::fromJson(data, &jsonError);

    if (jsonError.error != QJsonParseError::NoError || !jsonDocument.isArray()) {
        return;
    }

    QList<CoverStruct> covers;

    for (const QJsonValue value : jsonDocument.array()) {
        if (!value.isObject()) return;

        const QJsonObject coverObject = value.toObject();

        if (
            !coverObject.contains("id") ||
            !coverObject.contains("name")
        ) {
            return;
        }

        covers.append(CoverStruct{
            .id = coverObject.value("id").toString(),
            .name = coverObject.value("name").toString()
        });
    }

    m_allCoversInfo = covers;
}

void CoverCache::saveCoversConfigFile() {
    const QString configPath = m_appDataPath + "/" + _imagesJsonPath;

    QJsonArray jsonArray;
    for (const auto& cover : m_allCoversInfo) {
        QJsonObject jsonObject = {
            {"id", cover.id},
            {"name", cover.name},
        };
        jsonArray.append(jsonObject);
    }


    const QByteArray data = 
        QJsonDocument(jsonArray).toJson(QJsonDocument::Indented);

    QSaveFile file(configPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    file.write(data);
    file.commit();
}
