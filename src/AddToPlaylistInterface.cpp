#include "AddToPlaylistInterface.h"
#include <qdebug.h>

AddToPlaylistInterface::AddToPlaylistInterface() :
    QObject()
{}

AddToPlaylistInterface::~AddToPlaylistInterface()
{}

void AddToPlaylistInterface::addTracks(const QStringList& tracksPath)
{
    if (tracksPath.isEmpty())
    {
        return;
    }

    emit tracksAdded(tracksPath);
}
