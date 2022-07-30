#include "PlayingList.h"

PlayingList::PlayingList() :
    QObject(nullptr),
    m_index(0)
{}

/*
The list of the played musics.
*/
void PlayingList::setList(const QList<QString>& list)
{
    if (list != m_list)
    {
        m_list = list;
        m_index = 0;
        emit listChanged();
        emit indexChanged();
        emit currentChanged();
    }
}

const QList<QString>& PlayingList::list() const
{
    return m_list;
}

/*
The index of the currently played music in the list.
*/
int PlayingList::index() const
{
    return m_index;
}

void PlayingList::setIndex(int index)
{
    if (index != m_index && index >= 0 && (index < m_list.size() || index == 0))
    {
        m_index = index;
        emit indexChanged();
        emit currentChanged();
    }
}

/*
Is there a next music.
*/
bool PlayingList::hasNext() const
{
    return m_index+1 < m_list.size();
}

/*
Is there a previous music.
*/
bool PlayingList::hasPrevious() const
{
    return m_index-1 >= 0;
}

/*
Clear the playing list.
*/
void PlayingList::clear()
{
    setList(QList<QString>());
}

/*
Move to the next music.
If filePath is empty, the index move to the next item in the list.
If filePath is not empty, the index move to the item in the list with the same
file path.
*/
void PlayingList::next(const QString& filePath)
{
    // Move index to the position of filePath in the list.
    if (!filePath.isEmpty())
    {
        int index = m_list.indexOf(filePath);
        if (index >= 0)
        {
            setIndex(index);
        }
    }
    // Move index to one item.
    else 
    {
        if (hasNext())
        {
            setIndex(m_index+1);
        }
    }
}

/*
Move to the previous music.
*/
void PlayingList::previous()
{
    if (hasPrevious())
    {
        setIndex(m_index-1);
    }
}

/*
Get playing list from index to the end of the list.
*/
QList<QString> PlayingList::listFromIndex() const
{
    if (m_index >= 0 && m_index < m_list.size())
    {
        return m_list.mid(m_index);
    }
    else
    {
        return QList<QString>();
    }
}

QString PlayingList::current() const
{
    if (m_index >= 0 && m_index < m_list.size())
    {
        return m_list.at(m_index);
    }
    return QString();
}
