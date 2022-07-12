#include "FileSystemModel.h"
#include "FileSize.h"

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
    roles[SIZE] = "size";
    roles[LAST_MODIFIED] = "lastModified";
    roles[SELECTED] = "isSelected";
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
        (role == NAME || 
         role == FILE_PATH || 
         role == ABSOLUTE_FILE_PATH || 
         role == IS_DIR || 
         role == LIST_PATH || 
         role == COMPLETE_LIST_PATH || 
         role == SIZE ||
         role == LAST_MODIFIED ||
         role == SELECTED))
    {
        if (role == NAME)
        {
            return m_fileList.at(index.row()).info.fileName();
        }
        else if (role == FILE_PATH)
        {
            return m_fileList.at(index.row()).info.filePath();
        }
        else if (role == ABSOLUTE_FILE_PATH)
        {
            return m_fileList.at(index.row()).info.absoluteFilePath();
        }
        else if (role == SIZE)
        {
            if (!m_fileList.at(index.row()).info.isDir())
                return FileSize(m_fileList.at(index.row()).info.size()).toString();
            else
                return "";
        }
        else if (role == LAST_MODIFIED)
        {
            return m_fileList.at(index.row()).info.lastModified().toString();
        }
        else if (role == SELECTED)
        {
            return m_fileList.at(index.row()).isSelected;
        }
        /*
        Is the item is a folder or a file.
        This is necerrary to be able to known what to do
        when the user select the item.
        */
        else if (role == IS_DIR)
        {
            return m_fileList.at(index.row()).info.isDir();
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
        m_fileList.clear();
        m_listOrder.clear();
        QFileInfoList fileList = m_dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::DirsFirst);
        m_fileList.resize(fileList.size());
        for (size_t i = 0; i < fileList.size(); i++)
        {
            m_fileList[i] = { fileList.at(i), false };
        }

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
            if (m_fileList.at(i).info.isFile())
            {
                strList.append(m_fileList.at(i).info.absoluteFilePath());
            }
        }

        return strList;
    }

    return QStringList();
}

/*
Change the selection state of an item.
*/
void FileSystemModel::setIsSelected(int index, bool isSelected)
{
    if (index >= 0 && index < m_fileList.size())
    {
        if (isSelected != m_fileList.at(index).isSelected)
        {
            // Update if it's selected.
            m_fileList[index].isSelected = isSelected;

            /*
            Check if the list order list contain this index. 
            Otherwise, adding the index to the list.
            */
            int i = m_listOrder.indexOf(index);
            bool isModifies = false;

            if (isSelected)
            {
                if (i == -1)
                {
                    m_listOrder.append(index);
                    isModifies = true;
                }
            }
            else
            {
                if (i != -1)
                {
                    m_listOrder.remove(index);
                    isModifies = false;
                }
            }

            // Notify the view.
            emit dataChanged(this->index(index, 0), this->index(index, 0), {SELECTED});
        }
    }
}

/*
Clear the selection.
*/
void FileSystemModel::clearSelection()
{
    m_listOrder.clear();
    for (FileInfo& file : m_fileList)
    {
        file.isSelected = false;
    }

    // Notify the view of the modification.
    emit dataChanged(index(0), index(m_fileList.size()-1));
}

/*
Return the list of selected file.
*/
QStringList FileSystemModel::selectedFilesList()
{
    QStringList fileList;
    if (!m_listOrder.isEmpty())
    {
        for (int i = 0; i < m_listOrder.size(); i++)
        {
            if (m_listOrder.at(i) >= 0 && m_listOrder.at(i) < m_fileList.size())
            {
                fileList.append(m_fileList.at(m_listOrder.at(i)).info.absoluteFilePath());
            }
        }
    }

    return fileList;
}