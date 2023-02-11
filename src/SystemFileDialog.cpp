#include "SystemFileDialog.h"

SystemFileDialog::SystemFileDialog() :
    m_dialog(new QFileDialog())
{}

QString SystemFileDialog::title() const
{
    return m_title;
}

void SystemFileDialog::setTitle(const QString& title)
{
    if (title.compare(m_title) == 0) 
    {
        return;
    }

    m_title = title;
    emit titleChanged();
}

SystemFileDialog::FileMode SystemFileDialog::fileMode() const
{
    return m_fileMode;
}

void SystemFileDialog::setFileMode(FileMode mode)
{
    if (mode == m_fileMode)
    {
        return;
    }

    m_fileMode = mode;
    emit fileModeChanged();
}

QString SystemFileDialog::selectedFile() const
{
    const QStringList selectedFiles = m_dialog->selectedFiles();

    if (!selectedFiles.size())
    {
        return "";
    }

    return selectedFiles.at(0);
}

QStringList SystemFileDialog::selectedFiles() const
{
    return m_dialog->selectedFiles();
}

QString SystemFileDialog::directory() const
{
    return m_directory;
}

void SystemFileDialog::setDirectory(const QString &directory)
{
    const QString tmpDirectory = QUrl::fromUserInput(directory).toLocalFile();

    if (tmpDirectory.compare(m_directory) == 0)
    {
        return;
    }

    m_directory = tmpDirectory;
    emit directoryChanged();
}

QString SystemFileDialog::defaultSuffix() const
{
    return m_defaultSuffix;
}

void SystemFileDialog::setDefaultSuffix(const QString &suffix)
{
    QString tmpStr(suffix.trimmed());

    if (tmpStr.size())
    {
        if(tmpStr.at(0) == '.')
        {
            tmpStr = tmpStr.size() > 1 ? tmpStr.sliced(1) : "";
        }
    }

    if (tmpStr.compare(m_defaultSuffix) == 0)
    {
        return;
    }

    m_defaultSuffix = tmpStr;
    emit defaultSuffixChanged();
}

QStringList SystemFileDialog::nameFilters() const
{
    return m_nameFilters;
}

bool SystemFileDialog::compareStringList(const QStringList& a, const QStringList& b)
{
    if (a.size() != b.size()) 
    {
        return false;
    }

    for (const QString& aStr : a)
    {
        for (const QString& bStr : b)
        {
            if (aStr.compare(bStr) != 0) return false;
        }
    }

    return true;
}

void SystemFileDialog::setNameFilters(const QStringList &filters)
{
    if (compareStringList(m_nameFilters, filters))
    {
        return;
    }

    m_nameFilters = filters;
    emit nameFiltersChanged();
}

void SystemFileDialog::open()
{
    applyTitle();
    applyDirectory();
    applyFileMode();
    applyDefaultSuffix();
    applyNameFilters();
    execDialog();
}

void SystemFileDialog::applyTitle()
{
    m_dialog->setWindowTitle(m_title);
}

void SystemFileDialog::applyDirectory()
{
    if (!QDir(m_directory).exists())
    {
        return;
    }

    m_dialog->setDirectory(m_directory);
}

void SystemFileDialog::applyFileMode()
{
    switch (m_fileMode) 
    {
    case FileMode::OPEN_FILE:
    {
        m_dialog->setFileMode(QFileDialog::ExistingFile);
    } break;

    case FileMode::OPEN_FILES:
    {
        m_dialog->setFileMode(QFileDialog::ExistingFiles);
    } break;

    case FileMode::SELECT_DIRECTORY:
    {
        m_dialog->setFileMode(QFileDialog::Directory);
    } break;

    case FileMode::SAVE_FILE:
    default:
    {
        m_dialog->setFileMode(QFileDialog::AnyFile);
    } break;
    }
}

void SystemFileDialog::applyDefaultSuffix()
{
    m_dialog->setDefaultSuffix(m_defaultSuffix);
}

void SystemFileDialog::applyNameFilters()
{
    m_dialog->setNameFilters(m_nameFilters);
}

void SystemFileDialog::execDialog()
{
    int result = m_dialog->exec();

    if (!result)
    {
        emit rejected();
        return;
    }

    notifyUser();
    emit accepted();
}

void SystemFileDialog::notifyUser()
{
    emit selectedFileChanged();
    emit selectedFilesChanged();
}
