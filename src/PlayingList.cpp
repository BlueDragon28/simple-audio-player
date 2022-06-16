#include "PlayingList.h"

PlayingList::PlayingList(QObject* parent) :
    QObject(parent),
    m_index(0)
{}

/*
The list of the played musics.
*/
void PlayingList::setList(const QList<QString>& list)
{
    m_list = list;
    emit listChanged();
    setIndex(0);
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
    if (index >= 0 && (index < m_list.size() || index == 0))
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
    return m_index-1 > 0;
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
    if (hasNext())
    {
        setIndex(m_index+1);
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