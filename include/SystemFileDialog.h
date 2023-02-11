#ifndef SIMPLEAUDIOPLAYER_SYSTEMFILEDIALOG_H_
#define SIMPLEAUDIOPLAYER_SYSTEMFILEDIALOG_H_

#include <qobject.h>
#include <qfiledialog.h>
#include <qtmetamacros.h>
#include <qqmlintegration.h>

class SystemFileDialog : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(FileMode fileMode READ fileMode WRITE setFileMode NOTIFY fileModeChanged)
    Q_PROPERTY(QString selectedFile READ selectedFile NOTIFY selectedFileChanged)
    Q_PROPERTY(QStringList selectedFiles READ selectedFiles NOTIFY selectedFilesChanged)
    Q_PROPERTY(QString directory READ directory WRITE setDirectory NOTIFY directoryChanged)
    Q_PROPERTY(QString defaultSuffix READ defaultSuffix WRITE setDefaultSuffix NOTIFY defaultSuffixChanged)
    Q_PROPERTY(QStringList nameFilters READ nameFilters WRITE setNameFilters NOTIFY nameFiltersChanged)

public:
    enum FileMode 
    {
        OPEN_FILE,
        OPEN_FILES,
        SELECT_DIRECTORY,
        SAVE_FILE
    };
    Q_ENUM(FileMode);

    SystemFileDialog();

    QString title() const;
    void setTitle(const QString& title);

    FileMode fileMode() const;
    void setFileMode(FileMode mode);

    QString selectedFile() const;
    QStringList selectedFiles() const;

    QString directory() const;
    void setDirectory(const QString& directory);

    QString defaultSuffix() const;
    void setDefaultSuffix(const QString& suffix);

    QStringList nameFilters() const;
    void setNameFilters(const QStringList& filters);

signals:
    void titleChanged();
    void fileModeChanged();
    void selectedFileChanged();
    void selectedFilesChanged();
    void directoryChanged();
    void defaultSuffixChanged();
    void nameFiltersChanged();

    void accepted();
    void rejected();

public slots:
    void open();

private:
    void applyFileMode();
    void applyTitle();
    void applyDirectory();
    void applyDefaultSuffix();
    void applyNameFilters();
    void execDialog();
    void notifyUser();

    static bool compareStringList(const QStringList& a, const QStringList& b);

    QString m_title;
    QString m_directory;
    FileMode m_fileMode;
    QString m_defaultSuffix;
    QStringList m_nameFilters;

    QFileDialog* m_dialog;
};

#endif // SIMPLEAUDIOPLAYER_SYSTEMFILEDIALOG_H_
