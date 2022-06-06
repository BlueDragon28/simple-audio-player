#include "FileSystemModel.h"
#include <iostream>

FileSystemModel::FileSystemModel(QObject* parent) :
    QAbstractListModel(parent)
{
    connect(this, &FileSystemModel::pathChanged, this, &FileSystemModel::updateList);
}

// Return the roles name to be used in QML.
QHash<int, QByteArray> FileSystemModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NAME] = "name";
    roles[FILE_PATH] = "filePath";
    return roles;
}

// Allow the to access one element from the QML.
QVariantMap FileSystemModel::get(int i) const
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

int FileSystemModel::rowCount(const QModelIndex& parent) const
{
    return m_fileList.count();
}

QVariant FileSystemModel::data(const QModelIndex& index, int role) const
{
    if (index.column() == 0 &&
        index.row() >= 0 && index.row() < rowCount() &&
        (role == NAME || role == FILE_PATH))
    {
        if (role == NAME)
        {
            //return fileName(this->index("/home/", 0));
            //return fileName(index);
            //return "test";
            return m_fileList.at(index.row()).fileName();
        }
        else if (role == FILE_PATH)
        {
            //return filePath(index);
        }
    }

    return QVariant();
}

/*
When the directory path of the file system change, updating the list.
*/
void FileSystemModel::updateList() 
{
    if (!m_dir.path().isEmpty() && m_dir.isReadable())
    {
        beginRemoveRows(QModelIndex(), 0, rowCount());
        endRemoveRows();

        /*
        Get the list of folders and files inside the directory width the directory displayed first and
        without the . and .. directories.
        */
        m_fileList = m_dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::DirsFirst);

        beginInsertRows(QModelIndex(), 0, m_fileList.size());
        endInsertRows();
    }
}

/*
Get the path of the folder displayed.
*/
QString FileSystemModel::path() const
{
    return m_dir.path();
}

/*
Set the path of the folder to display.
Do nothing if the path is invalid.
*/
void FileSystemModel::setPath(const QString& path)
{
    if (!path.isEmpty())
    {
        QDir dir = QDir(path);
        if (dir.isReadable())
        {
            m_dir = dir;
            emit pathChanged();
        }
    }
}