#ifndef SIMPLEAUDIOPLAYER_PLAYINGLIST_H_
#define SIMPLEAUDIOPLAYER_PLAYINGLIST_H_

#include <QObject>
#include <QList>
#include <QString>
#include <QtQml/qqmlregistration.h>
#include <QtQml/QQmlEngine>
#include <QtQml/QJSEngine>

class PlayingList : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(QList<QString> list READ list WRITE setList NOTIFY listChanged)
    Q_PROPERTY(int index READ index NOTIFY indexChanged)

public:
    PlayingList();
    
    /*
    The list of the played musics.
    */
    void setList(const QList<QString>& list);
    const QList<QString>& list() const;

    /*
    The index of the currently played music in the list.
    */
    int index() const;

    /*
    Is there a next music.
    */
    Q_INVOKABLE bool hasNext() const;

    /*
    Is there a previous music.
    */
    Q_INVOKABLE bool hasPrevious() const;

    /*
    Clear the playing list.
    */
    Q_INVOKABLE void clear();

signals:
    void listChanged();
    void indexChanged();

public slots:
    /*
    Move to the next music.
    */
    void next();

    /*
    Move to the previous music.
    */
    void previous();

private:
    void setIndex(int index);

    QList<QString> m_list;
    int m_index;
};

#endif // SIMPLEAUDIOPLAYER_PLAYINGLIST_H_