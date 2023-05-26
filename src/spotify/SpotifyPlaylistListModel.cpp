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
    return SelectionModel::data(index, role);
}

QHash<int, QByteArray> SpotifyPlaylistListModel::roleNames() const
{
    QHash<int, QByteArray> roles = SelectionModel::roleNames();
    roles[NAME] = "name";
    roles[ARTISTS] = "artists";
    roles[ALBUM] = "albums";
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
    
    getAuthorsAndDuration();
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
