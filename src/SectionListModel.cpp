#include "SectionListModel.h"
#include "MusicCollectionList.h"

SectionListModel::SectionListType::SectionListType() :
    m_type(SectionListModel::FILE_SYSTEM)
{}

SectionListModel::SectionListType::SectionListType(const QString& name, int type) :
    m_name(name),
    m_type(type)
{}

const QString& SectionListModel::SectionListType::name() const
{
    return m_name;
}

void SectionListModel::SectionListType::setName(const QString& name)
{
    m_name = name;
}

int SectionListModel::SectionListType::type() const
{
    return m_type;
}

void SectionListModel::SectionListType::setType(int type)
{
    m_type = type;
}

SectionListModel::SectionListModel(QObject* parent) :
    QAbstractListModel(parent),
    m_data({
        SectionListType("File system", FILE_SYSTEM),
        SectionListType("Albums", ALBUMS),
        SectionListType("Playlist", PLAYLIST),
        SectionListType("Spotify", SPOTIFY)
    })
{}

SectionListModel::~SectionListModel()
{}

int SectionListModel::rowCount(const QModelIndex& parent) const
{
    return m_data.count();
}

QVariant SectionListModel::data(const QModelIndex& index, int role) const
{
    if (index.column() == 0 && (index.row() >= 0 && index.row() < rowCount()))
    {
        // Return the data base on the role.
        if (role == NAME)
            return m_data.at(index.row()).name();
        else if (role == TYPE)
            return m_data.at(index.row()).type();
    }

    return QVariant();
}

QHash<int, QByteArray> SectionListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NAME] = "name";
    roles[TYPE] = "type";
    return roles;
}

QVariantMap SectionListModel::get(int i) const
{
    QModelIndex index = this->index(i, 0);
    QHash<int, QByteArray> roles = roleNames();
    QVariantMap map;
    for (QHash<int, QByteArray>::const_iterator cit = roles.cbegin();
         cit != roles.cend();
         cit++)
    {
        map[cit.value()] = index.data(cit.key());
    }
    return map;
}