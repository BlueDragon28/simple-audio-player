#ifndef SIMPLEAUDIOLIBRARY_FILESYSTEMMODEL_H_
#define SIMPLEAUDIOLIBRARY_FILESYSTEMMODEL_H_

#include "SelectionModel.h"
#include <QAbstractListModel>
#include <QDir>
#include <QtQml/qqmlcomponent.h>

/*
This class display the content of a folder to a list view.
*/
class FileSystemModel : public SelectionModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)

public:
    FileSystemModel(QObject* parent = nullptr);

    enum FileSystemRole
    {
        NAME = MAX_ROLE,
        FILE_PATH,
        ABSOLUTE_FILE_PATH,
        IS_DIR,
        LIST_PATH, // Return the list of file from index to the end in the directory.
        COMPLETE_LIST_PATH, // Return the list of all the file in the directory.
        SIZE, // Return the size of the file in human readable form.
        LAST_MODIFIED, // Return the last time the file was modified.
    };

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    /*
    Set the path of the folder to display.
    Do nothing if the path is invalid.
    */
    void setPath(const QString& path);

    /*
    Get the path of the folder displayed.
    */
    QString path() const;

    /*
    Return the path of the file index (if valid) and all the files next to index inside the directory (m_dir).
    index:
        - if valid: return the files path of index and all the files next to it.
        - if equal to -1: return the files path of all the files.
    */
    Q_INVOKABLE QStringList fileList(int index = -1) const;

    /*
    Returning the selected file in the current directory.
    */
    Q_INVOKABLE QStringList selectedFilesList() const;

signals:
    /*
    This signal is emitted when the displayed path is displayed.
    */
    void pathChanged();

public slots:
    QVariantMap get(int index) const;

    /*
    Move to the children directory of the current directory.
    */
    void cd(const QString& dir);

    /*
    Move to the parent directory of the current directory.
    */
    void cdUp();

    /*
    Move to the previously opened directory (if any).
    */
    void cdDown();

private:
    /*
    When the directory path of the file system change, updating the list.
    */
    void updateList();

    /*
    Struct holding the file info and if the element is selected.
    */
    struct FileInfo
    {
        QFileInfo info;
    };

    // The Qt directory object used to retrive the file and folders list from a directory.
    QDir m_dir;

    // Display the drives.
#ifdef WIN32
    bool m_displayDrives;
#endif

    // This list store the list of all the last opened directories.
    QStringList m_lastDirsList;
};

#endif // SIMPLEAUDIOLIBRARY_FILESYSTEMMODEL_H_
