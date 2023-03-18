#include "options/SAPOptions.h"
#include "LicenseDialog.h"
#include "Player.h"
#include "options/AboutDialog.h"
#include "AppConfig.h"
#include "simple-audio-library/AudioPlayer.h"
#include "simple-audio-library/Common.h"
#include <QtWidgets/qdialog.h>
#include <QtWidgets/QFrame>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QFileDialog>
#include <QStandardPaths>
#include <qboxlayout.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlistwidget.h>
#include <qpushbutton.h>

/*
========================================================
===================== OptionsDialog ====================
========================================================
*/

OptionsDialog::OptionsDialog() :
    QDialog(nullptr),
    m_folderList(nullptr),
    m_backendAudio(nullptr)
{
    buildInterface();
    connect(this, &OptionsDialog::finished, this, [this](){this->deleteLater();});

    setWindowTitle(tr("Options"));
}

OptionsDialog::~OptionsDialog()
{}

void OptionsDialog::buildInterface()
{
    QVBoxLayout* vLayout = new QVBoxLayout(this);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addLayout(mainLayout, 1);

    /*
    Main content of the dialog.
    */
    createMainView(mainLayout);
    
    // Line seperation.
    QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    vLayout->addWidget(line, 0);

    // Button at the button of the layout.
    QHBoxLayout* hLayout = new QHBoxLayout(this);

    // Cancel button.
    QPushButton* cancel = new QPushButton(tr("Cancel"), this);
    connect(cancel, &QPushButton::clicked, this, &OptionsDialog::reject);

    // Accept button.
    QPushButton* apply = new QPushButton(tr("Apply"), this);
    connect(apply, &QPushButton::clicked, this, &OptionsDialog::applyChange);

    hLayout->addStretch(1);
    hLayout->addWidget(cancel);
    hLayout->addWidget(apply);

    vLayout->addLayout(hLayout, 0);
}

void OptionsDialog::createMainView(QVBoxLayout* layout)
{
    if (layout)
    {
        // Creating the QTabWidget
        QTabWidget* tab = new QTabWidget(this);
        tab->addTab(createMusicTab(), "Music Collection"); // Music tab
        tab->addTab(createStreamTab(), "Stream");
        
        layout->addWidget(tab);
    }
}

void OptionsDialog::applyChange()
{
    saveCollectionList();
    applyStreamOptions();

    accept();
}

QWidget* OptionsDialog::createMusicTab()
{
    QWidget* tab = new QWidget();

    // Creating the main layout of the tab.
    QVBoxLayout* vLayout = new QVBoxLayout(tab);

    // Creating the collection path list.
    createMusicCollectionList(vLayout, tab);

    return tab;
}

void OptionsDialog::createMusicCollectionList(QBoxLayout* mainLayout, QWidget* parent)
{
    // The Group layout for the music folders collection.
    QGroupBox* collectionGroup = new QGroupBox(tr("Collections Folders:"), parent);
    mainLayout->addWidget(collectionGroup);

    // Layout of the collectionGroup widget.
    QVBoxLayout* collectionLayout = new QVBoxLayout();
    collectionLayout->setContentsMargins(0, 0, 0, 0);
    collectionGroup->setLayout(collectionLayout);

    // A list with the folders list.
    m_folderList = new QListWidget(parent);
    initCollectionListWidget(); // Initialize the foldersList widget with the collection music path list to listen to.
    collectionLayout->addWidget(m_folderList);

    // The layout for the collectionGroup.
    QHBoxLayout* collectionBtnLayout = new QHBoxLayout();
    collectionLayout->addLayout(collectionBtnLayout);
    
    // Btn to remove selected item from the list.
    QPushButton* delFolderBtn = new QPushButton(tr("Remove Folder"), collectionGroup);
    collectionBtnLayout->addWidget(delFolderBtn);

    // Btn to add a new folder into the list.
    QPushButton* addFolderBtn = new QPushButton(tr("Add Folder"), collectionGroup);
    collectionBtnLayout->addWidget(addFolderBtn);

    // Action to remove a path from the list.
    connect(delFolderBtn, &QPushButton::clicked, [this]() {
        QList<QListWidgetItem*> selectedItems = m_folderList->selectedItems();

        for (QListWidgetItem* item : selectedItems)
        {
            if (m_folderList->takeItem(
                m_folderList->row(item)))
            {
                delete item;
            }
        }
    });

    // Action to add a new path to the list.
    connect(addFolderBtn, &QPushButton::clicked, [this]() {
        const QString directory = QFileDialog::getExistingDirectory(this, tr("Select a folder"));
        if (!directory.isEmpty())
        {
            bool isPresent = false;

            // Check if the folder has not been already added.
            for (int i = 0; i < m_folderList->count(); i++)
            {
                const QListWidgetItem* item = m_folderList->item(i);
                if (item->text().compare(directory) == 0)
                {
                    isPresent = true;
                    break;
                }
            }

            if (!isPresent)
            {
                m_folderList->addItem(directory);
            }
        }
    });
}

void OptionsDialog::initCollectionListWidget()
{
    if (m_folderList)
    {
        bool isExists = false; // If the music collection path list is set.
        QStringList pathList = AppConfig::getMusicCollectionPathList(&isExists);

        // If the list exists, put the list into the listWidget.
        if (isExists)
        {
            for (const QString& path : pathList)
            {
                m_folderList->addItem(path);
            }
        }
        // If the list config do not exists, use default value.
        else 
        {
            m_folderList->addItem(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).at(0));
        }
    }
}

void OptionsDialog::saveCollectionList()
{
    // Extract the folder path from the QListWidgetItem objects of m_folderList QListWidget.
    QStringList collectionsList;
    for (int i = 0; i < m_folderList->count(); i++)
    {
        collectionsList.append(m_folderList->item(i)->text());
    }
    
    // Send the list to AppConfig.
    AppConfig::setMusicCollectionPathList(collectionsList);
}

QWidget* OptionsDialog::createStreamTab()
{
    QWidget* tab = new QWidget();
    QVBoxLayout* vLayout = new QVBoxLayout(tab);

    createBackendAudioOption(vLayout, tab);
    getDefaultStreamOptions();

    return tab;
}

void OptionsDialog::createBackendAudioOption(QBoxLayout* layout, QWidget* parent)
{
    QHBoxLayout* hLayout = new QHBoxLayout();
    QLabel* textLabel = new QLabel("Backend Audio", parent);
    
    QComboBox* backendSelection = new QComboBox(parent);
    setAvailableBackend(backendSelection);
    
    hLayout->addWidget(textLabel, 1);
    hLayout->addWidget(backendSelection, 0);
    layout->addLayout(hLayout);

    m_backendAudio = backendSelection;
}

void OptionsDialog::setAvailableBackend(QComboBox* backendSelection)
{
    const Player* player = Player::getInstance();

    if (!player) return;

    const SAL::AudioPlayer* salPlayer = player->getPlayer();
    std::vector<SAL::BackendAudio> availableBackendsAudio = salPlayer->availableBackendAudio();

    backendSelection->addItem(u8"System Default");

    for (const SAL::BackendAudio backend : availableBackendsAudio)
    {
        const std::string backendName = SAL::AudioPlayer::getAudioBackendName(backend);
        backendSelection->addItem(QString::fromStdString(backendName));
    }
}

void OptionsDialog::getDefaultStreamOptions()
{
    SAL::BackendAudio selectedBackend = AppConfig::getBackendAudioSetting();

    if (selectedBackend == SAL::BackendAudio::SYSTEM_DEFAULT) 
    {
        m_backendAudio->setCurrentIndex(0);
        return;
    }

    QString backendName = getBackendStringNameFromEnum(selectedBackend);

    const int selectionCount = m_backendAudio->count();
    
    for (int i = 0; i < selectionCount; i++)
    {
        const QString name = m_backendAudio->itemText(i);

        if (name.compare(backendName) == 0) 
        {
            m_backendAudio->setCurrentIndex(i);
            break;
        }
    }
}

void OptionsDialog::applyStreamOptions()
{
    const QString backendName = m_backendAudio->currentText();
    const SAL::BackendAudio backend = getBackendEnumFromString(backendName);
    AppConfig::setBackendAudioSetting(backend);
}

QString OptionsDialog::getBackendStringNameFromEnum(SAL::BackendAudio backend) const 
{
    const std::string backendName = SAL::AudioPlayer::getAudioBackendName(backend);
    return QString::fromStdString(backendName);
}

SAL::BackendAudio OptionsDialog::getBackendEnumFromString(const QString& name)
{
    if (name.compare(u8"Direct Sound") == 0)
    {
        return SAL::BackendAudio::DIRECT_SOUND;
    } 
    else if (name.compare(u8"MME") == 0)
    {
        return SAL::BackendAudio::MME;
    }
    else if (name.compare(u8"ASIO") == 0)
    {
        return SAL::BackendAudio::ASIO;
    }
    else if (name.compare(u8"WASAPI") == 0)
    {
        return SAL::BackendAudio::WASAPI;
    }
    else if (name.compare(u8"WDMKS") == 0)
    {
        return SAL::BackendAudio::WDMKS;
    }
    else if (name.compare(u8"OSS") == 0)
    {
        return SAL::BackendAudio::OSS;
    }
    else if (name.compare(u8"ALSA") == 0)
    {
        return SAL::BackendAudio::ALSA;
    }
    else if (name.compare(u8"JACK") == 0)
    {
        return SAL::BackendAudio::JACK;
    }
    else
    {
        return SAL::BackendAudio::SYSTEM_DEFAULT;
    }
}

/*
========================================================
====================== SAPOptions ======================
========================================================
*/
SAPOptions::SAPOptions() :
    m_optionsDialog(nullptr),
    m_aboutDialog(nullptr),
    m_licenseDialog(nullptr)
{}

SAPOptions::~SAPOptions()
{
    // Delete m_dialog if exists.
    if (m_optionsDialog)
    {
        delete m_optionsDialog;
    }

    if (m_aboutDialog)
    {
        delete m_aboutDialog;
    }

    if (m_licenseDialog)
    {
        delete m_licenseDialog;
    }
}

template<typename T>
void SAPOptions::openDialog(T** memberVar)
{
    // memberVar is the pointer the member variable of the SAPOptions class storing a pointer to the dailog object.
    if (*memberVar)
    {
        return;
    }

    // Creating a new instance of T and run it.
    T* dialog = new T();
    *memberVar = dialog;

    // When the dialog is destroyed, set *memberVar to nullptr.
    connect(dialog, &T::destroyed, [memberVar, dialog]() {
        if (*memberVar == dialog)
        {
            *memberVar = nullptr;
        }
    });

    dialog->show();
    dialog->raise();
    dialog->activateWindow();
}

void SAPOptions::openOptions()
{
    openDialog(&m_optionsDialog);
}

void SAPOptions::openAbout()
{
    openDialog(&m_aboutDialog);
}

void SAPOptions::openLicense()
{
    openDialog(&m_licenseDialog);
}
