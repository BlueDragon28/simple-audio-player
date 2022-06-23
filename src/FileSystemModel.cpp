#include "FileSystemModel.h"

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
    roles[ABSOLUTE_FILE_PATH] = "absoluteFilePath";
    roles[IS_DIR] = "isDir";
    roles[LIST_PATH] = "listPath";
    roles[COMPLETE_LIST_PATH] = "completeListPath";
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
        (role == NAME || role == FILE_PATH || role == ABSOLUTE_FILE_PATH || role == IS_DIR || role == LIST_PATH || role == COMPLETE_LIST_PATH))
    {
        if (role == NAME)
        {
            return m_fileList.at(index.row()).fileName();
        }
        else if (role == FILE_PATH)
        {
            return m_fileList.at(index.row()).filePath();
        }
        else if (role == ABSOLUTE_FILE_PATH)
        {
            return m_fileList.at(index.row()).absoluteFilePath();
        }
        /*
        Is the item is a folder or a file.
        This is necerrary to be able to known what to do
        when the user select the item.
        */
        else if (role == IS_DIR)
        {
            return m_fileList.at(index.row()).isDir();
        }
        else if (role == LIST_PATH)
        {
            return fileList(index.row());
        }
        else if (role == COMPLETE_LIST_PATH)
        {
            return fileList();
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
        beginRemoveRows(QModelIndex(), 0, rowCount()-1);
        endRemoveRows();

        /*
        Get the list of folders and files inside the directory width the directory displayed first and
        without the . and .. directories.
        */
        m_fileList = m_dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::DirsFirst);

        beginInsertRows(QModelIndex(), 0, m_fileList.size()-1);
        endInsertRows();
    }
}

/*
Get the path of the folder displayed.
*/
QString FileSystemModel::path() const
{
    return m_dir.absolutePath();
}

/*
Set the path of the folder to display.
Do nothing if the path is invalid.
*/
void FileSystemModel::setPath(const QString& path)
{
    if (!path.isEmpty() && path != m_dir.path())
    {
        QDir dir = QDir(path);
        if (dir.isReadable())
        {
            m_dir = dir;
            m_lastDirsList.clear();
            emit pathChanged();
        }
    }
}

/*
Move to the children directory of the current directory.
*/
void FileSystemModel::cd(const QString& dir)
{
    QString curDirName = m_dir.dirName();
    if (m_dir.cd(dir))
    {
        // If whe move to the parent directory, store the name of the current dir inside the m_lastDirsList list.
        if (dir == "..")
        {
            m_lastDirsList.append(curDirName);
        }
        // Otherwise, clear the m_lastDirsList list.
        else
        {
            m_lastDirsList.clear();
        }

        emit pathChanged();
    }
}

/*
Move to the parent directory of the current directory.
*/
void FileSystemModel::cdUp()
{
    cd("..");
}

/*
Move to the previously opened directory (if any).
*/
void FileSystemModel::cdDown()
{
    if (!m_lastDirsList.isEmpty())
    {
        if (m_dir.cd(*m_lastDirsList.crbegin()))
        {
            m_lastDirsList.removeLast();
            emit pathChanged();
        }
    }
}

/*
Return the path of the file index (if valid) and all the files next to index inside the directory (m_dir).
index:
    - if valid: return the files path of index and all the files next to it.
    - if equal to -1: return the files path of all the files.
*/
QStringList FileSystemModel::fileList(int index) const
{
    if (index == -1)
    {
        index = 0;
    }

    if (!m_fileList.isEmpty() && index >= 0 && index < m_fileList.size())
    {
        QStringList strList;
        for (int i = index; i < m_fileList.size(); i++)
        {
            if (m_fileList.at(i).isFile())
            {
                strList.append(m_fileList.at(i).absoluteFilePath());
            }
        }

        return strList;
    }

    return QStringList();
}