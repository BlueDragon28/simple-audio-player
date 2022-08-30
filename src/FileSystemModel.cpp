#include "FileSystemModel.h"
#include "FileSize.h"

FileSystemModel::FileSystemModel(QObject* parent) :
    SelectionModel(parent),
#ifdef WIN32
    m_displayDrives(true)
#endif
{
    connect(this, &FileSystemModel::pathChanged, this, &FileSystemModel::updateList);

    // Set how the root should be based.
#ifdef WIN32
    setPath("");
#else
    setPath("/");
#endif
}

// Return the roles name to be used in QML.
QHash<int, QByteArray> FileSystemModel::roleNames() const
{
    // Initialize roles with the roles of the parent class SelectionModel.
    QHash<int, QByteArray> roles(SelectionModel::roleNames());
    roles[NAME] = "name";
    roles[FILE_PATH] = "filePath";
    roles[ABSOLUTE_FILE_PATH] = "absoluteFilePath";
    roles[IS_DIR] = "isDir";
    roles[LIST_PATH] = "listPath";
    roles[COMPLETE_LIST_PATH] = "completeListPath";
    roles[SIZE] = "size";
    roles[LAST_MODIFIED] = "lastModified";
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

QVariant FileSystemModel::data(const QModelIndex& index, int role) const
{
    if (index.column() == 0 &&
        index.row() >= 0 && index.row() < rowCount())
    {
        // Retrieve the item from the parent class.
        QVariant variant = SelectionModel::item(index.row());

        // Convert it to FileInfo (if possible).
        FileInfo fileInfo;
        if (variant.canConvert<FileInfo>())
        {
            fileInfo = qvariant_cast<FileInfo>(variant);
        }
        else
        {
            return QVariant();
        }


        if (role == NAME)
        {
            return fileInfo.info.fileName();
        }
        else if (role == FILE_PATH)
        {
            return fileInfo.info.filePath();
        }
        else if (role == ABSOLUTE_FILE_PATH)
        {
            return fileInfo.info.absoluteFilePath();
        }
        else if (role == SIZE)
        {
            if (!fileInfo.info.isDir())
                return FileSize(fileInfo.info.size()).toString();
            else
                return "";
        }
        else if (role == LAST_MODIFIED)
        {
            return fileInfo.info.lastModified().toString();
        }
        /*
        Is the item is a folder or a file.
        This is necerrary to be able to known what to do
        when the user select the item.
        */
        else if (role == IS_DIR)
        {
            return fileInfo.info.isDir();
        }
        else if (role == LIST_PATH)
        {
            return fileList(index.row());
        }
        else if (role == COMPLETE_LIST_PATH)
        {
            return fileList();
        }
        // Otherwise, call the data parent class.
        else
        {
            return SelectionModel::data(index, role);
        }
    }

    return QVariant();
}

/*
When the directory path of the file system change, updating the list.
*/
void FileSystemModel::updateList() 
{
// On Windows, allow empty path to display the drives.
#ifdef WIN32
    if (m_displayDrives || (!m_dir.path().isEmpty() && m_dir.isReadable()))
#else
    if (!m_dir.path().isEmpty() && m_dir.isReadable())
#endif
    {
        if (rowCount() > 0)
        {
            beginRemoveRows(QModelIndex(), 0, rowCount()-1);
            endRemoveRows();
        }

        /*
        Get the list of folders and files inside the directory width the directory displayed first and
        without the . and .. directories.
        */
        QFileInfoList fileList;
#ifdef WIN32
        // On Windows: if the path is not empty, retrieve the files and folders list, otherwise, retrieve the drives list.
        if (!m_displayDrives && !m_dir.path().isEmpty())
        {
#endif
            fileList = m_dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::DirsFirst);
#ifdef WIN32
        }
        else
        {
            fileList = QDir::drives();
        }
#endif

        // Put every file info into the variant list.
        if (!fileList.isEmpty())
        {
            QVariantList variantList(fileList.size());
            for (int i = 0; i < fileList.size(); i++)
            {
                variantList[i] = QVariant::fromValue(FileInfo { fileList.at(i) });
            }

            // Send the variant list to the parent class.
            setItemList(variantList);

            // Notify the view.
            beginInsertRows(QModelIndex(), 0, rowCount()-1);
            endInsertRows();
        }
        else
        {
            // If no folders/files found in the current directory, send an empty list to the upper class.
            setItemList(QVariantList());
        }
    }
}

/*
Get the path of the folder displayed.
*/
QString FileSystemModel::path() const
{
    if (!m_displayDrives)
    {
        return m_dir.absolutePath();
    }
    else
    {
        return "";
    }
}

/*
Set the path of the folder to display.
Do nothing if the path is invalid.
*/
void FileSystemModel::setPath(const QString& path)
{
    // On Windows, check if the path is not null or m_displayDrives is true
#ifdef WIN32
    if (path != m_dir.path() || m_displayDrives)
#else
    // On Linux, check if the path if not valid and different from m_dir.path().
    if (path != m_dir.path() && !path.isEmpty())
#endif
    {
        // Check if not empty only on Windows.
#ifdef WIN32
        if (!path.isEmpty())
        {
#endif
            QDir dir = QDir(path);
#ifdef WIN32
            m_displayDrives = false;
#endif
            if (dir.isReadable())
            {
                m_dir = dir;
                m_lastDirsList.clear();
                emit pathChanged();
            }
#ifdef WIN32
        }
        else
        {
            m_displayDrives = true;
            emit pathChanged();
        }
#endif
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

    // Getting the list from the parent class.
    QVariantList variantList = itemList();

    // Check if index is valid
    if (index >= 0 && index < variantList.size())
    {
        // Create a string list with the size of variantList minous index position.
        QStringList stringList(variantList.size()-index);
        int invalidVariant = 0;

        // Copy every absoluteFilePath from variantList into stringList.
        int i = 0;
        foreach (const QVariant& variant, variantList)
        {
            // Convert variant to FileInfo struct.
            if (variant.canConvert<FileInfo>())
            {
                stringList[i] = qvariant_cast<FileInfo>(variant).info.absoluteFilePath();
                i++;
            }
            // If convertion fail, increment invalidVariant variable.
            else
            {
                invalidVariant++;
            }
        }

        // If some convertion failed, remove the extra space from the list.
        if (invalidVariant > 0)
        {
            stringList.resize(stringList.size()-invalidVariant);
        }

        return stringList;
    }

    return QStringList();
}

QStringList FileSystemModel::selectedFilesList() const
{
    // Retrieving the selected items from the upper class.
    QVariantList variantList = selectedItemList();

    QStringList filePathList;

    // Iterate thourgh the variant list and retrieve the file path.
    foreach (const QVariant& variant, variantList)
    {
        // Convert variant to FileInfo struct
        if (variant.canConvert<FileInfo>())
        {
            FileInfo fileInfo = qvariant_cast<FileInfo>(variant);

            // Append the file path from fileInfo into the filePathList;
            filePathList.append(fileInfo.info.absoluteFilePath());
        }
    }

    // Retrieving the file path list.
    return filePathList;
}
