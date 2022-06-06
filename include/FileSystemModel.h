#ifndef SIMPLEAUDIOLIBRARY_FILESYSTEMMODEL_H_
#define SIMPLEAUDIOLIBRARY_FILESYSTEMMODEL_H_

#include <QAbstractListModel>
#include <QDir>
#include <QtQml/qqmlcomponent.h>

/*
This class display the content of a folder to a list view.
*/
class FileSystemModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)

public:
    FileSystemModel(QObject* parent = nullptr);

    enum FileSystemRole
    {
        NAME = Qt::UserRole+1,
        FILE_PATH,
        ABSOLUTE_FILE_PATH,
        IS_DIR,
    };

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
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

private:
    /*
    When the directory path of the file system change, updating the list.
    */
    void updateList();

    QDir m_dir;
    QFileInfoList m_fileList;
};

#endif // SIMPLEAUDIOLIBRARY_FILESYSTEMMODEL_H_