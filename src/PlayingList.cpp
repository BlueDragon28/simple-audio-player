#include "PlayingList.h"

PlayingList::PlayingList() :
    QObject(nullptr),
    m_index(0),
    m_doNotMove(false)
{}

/*
The list of the played musics.
*/
void PlayingList::setList(const QList<QString>& list)
{
    if (list != m_list)
    {
        m_list = list;
        emit listChanged();
        setIndex(0);
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
        m_index = index;
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
*/
void PlayingList::next()
{
    if (hasNext() && !m_doNotMove)
    {
        setIndex(m_index+1);
    }
}

/*
Move to the previous music.
*/
void PlayingList::previous()
{
    if (hasPrevious() && !m_doNotMove)
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

bool PlayingList::doNotMove() const
{
    return m_doNotMove;
}

void PlayingList::setDoNotMove(bool value)
{
    if (value != m_doNotMove)
    {
        m_doNotMove = value;
        emit doNotMoveChanged();
    }
}