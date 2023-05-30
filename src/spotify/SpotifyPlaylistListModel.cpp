#include "spotify/SpotifyPlaylistListModel.h"
#include "SelectionModel.h"
#include "SpotifyPlaylistListModel.h"
#include "SpotifyReceivedPlaylistElement.h"

SpotifyPlaylistListModel::SpotifyPlaylistListModel(QObject* parent) :
    SelectionModel(parent),
    m_playlist(nullptr)
{}

SpotifyPlaylistListModel::~SpotifyPlaylistListModel()
{}

QVariant SpotifyPlaylistListModel::data(const QModelIndex& index, int role) const
{
    if ((role != TRACK_NUMBER && role != NAME &&
        role != ARTISTS && role != ALBUM) ||
        !item(index.row()).canConvert<SpotifyReceivedPlaylistElement::Track>() ||
        !item(index.row()).convert(
            QMetaType::fromType<SpotifyReceivedPlaylistElement::Track>()))
    {
        return SelectionModel::data(index, role);
    }

    const SpotifyReceivedPlaylistElement::Track track = 
        qvariant_cast<SpotifyReceivedPlaylistElement::Track>(item(index.row()));

    if (role == TRACK_NUMBER)
    {
        return QString::number(index.row()+1);
    }
    else if (role == NAME)
    {
        return track.name;
    }
    else if (role == ARTISTS)
    {
        return track.artists;
    }
    else if (role == ALBUM)
    {
        return track.album;
    }

    return SelectionModel::data(index, role);
}

QHash<int, QByteArray> SpotifyPlaylistListModel::roleNames() const
{
    QHash<int, QByteArray> roles = SelectionModel::roleNames();
    roles[TRACK_NUMBER] = "trackIndex";
    roles[NAME] = "name";
    roles[ARTISTS] = "artists";
    roles[ALBUM] = "album";
    return roles;
}

void SpotifyPlaylistListModel::setPlaylist(SpotifyReceivedPlaylistElement* playlist)
{
    if (!playlist) return;

    if (m_playlist)
    {
        m_playlist->deleteLater();
        m_playlist = nullptr;
    }

    m_playlist = playlist;
    m_playlist->setParent(this);

    if (m_playlist->failed()) return;

    emit playlistLoaded();
    emit nameChanged();
    emit imageHrefChanged();
    emit idChanged();
    emit uriChanged();
    
    getAuthorsAndDuration();
    parsePlaylistData();
}

QString SpotifyPlaylistListModel::name() const
{
    if (!m_playlist) return QString();

    return m_playlist->name();
}

QUrl SpotifyPlaylistListModel::imageHref() const
{
    if (!m_playlist) return QUrl();

    return m_playlist->imageHref();
}

QString SpotifyPlaylistListModel::id() const
{
    if (!m_playlist) return QString();

    return m_playlist->id();
}

QString SpotifyPlaylistListModel::uri() const
{
    if (!m_playlist) return QString();

    return m_playlist->uri();
}

QString SpotifyPlaylistListModel::authors() const
{
    return m_authors;
}

QString SpotifyPlaylistListModel::duration() const
{
    return m_duration;
}

void SpotifyPlaylistListModel::setAuthors(const QString& authors)
{
    if (authors == m_authors) return;

    m_authors = authors;
    emit authorsChanged();
}

void SpotifyPlaylistListModel::setDuration(const QString& duration)
{
    if (duration == m_duration) return;

    m_duration = duration;
    emit durationChanged();
}

void SpotifyPlaylistListModel::getAuthorsAndDuration()
{
    if (!m_playlist || !m_playlist->tracksCount())
    {
        setAuthors("");
        setDuration("");
        return;
    }

    setAuthors(m_playlist->track(0).artists);

    uint64_t totalDuration = 0;
    for (int i = 0; i < m_playlist->tracksCount(); i++)
    {
        totalDuration+= m_playlist->track(i).durationMS;
    }

    setDuration(parseDuration(totalDuration));
}

QString SpotifyPlaylistListModel::parseDuration(uint64_t duration) const
{
    QString totalDuration;
    int count = 0;

    static const uint64_t dayInMS = 86400000;
    static const uint64_t hourInMS = 3600000;
    static const uint64_t minuteInMS = 60000;
    static const uint64_t secondInMS = 1000;

    if (duration >= dayInMS)
    {
        totalDuration += QString::number(duration/dayInMS) + "day";
        duration %= dayInMS;
        count++;
    }

    if (duration >= hourInMS && count < 2)
    {
        totalDuration += 
            (count > 0 ? " " : "") +
            QString::number(duration/hourInMS) + "h";
        duration %= hourInMS;
        count++;
    }

    if (duration >= minuteInMS && count < 2)
    {
        totalDuration += 
            (count > 0 ? " " : "") +
            QString::number(duration/minuteInMS) + "m";
        duration %= minuteInMS;
        count++;
    }

    if (duration >= secondInMS && count < 2)
    {
        totalDuration +=
            (count > 0 ? " " : "") +
            QString::number(duration/secondInMS) + "s";
        duration %= secondInMS;
        count++;
    }

    if (count < 2)
    {
        totalDuration +=
            (count > 0 ? " " : "") +
            QString::number(duration) + "ms";
        count++;
    }

    return totalDuration;
}

void SpotifyPlaylistListModel::parsePlaylistData() 
{
    if (rowCount() > 0) 
    {
        beginRemoveRows(QModelIndex(), 0, rowCount()-1);
        clear();
        endRemoveRows();
    }

    if (!m_playlist) return;

    beginInsertRows(QModelIndex(), 0, m_playlist->tracksCount()-1);

    QVariantList tracksData;

    for (int i = 0; i < m_playlist->tracksCount(); i++)
    {
        tracksData.append(QVariant::fromValue(m_playlist->track(i)));
    }

    setItemList(tracksData);

    endInsertRows();
}
