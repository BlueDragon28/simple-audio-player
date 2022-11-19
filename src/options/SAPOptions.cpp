#include "options/SAPOptions.h"
#include <QtWidgets/qdialog.h>
#include <QtWidgets/QFrame>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QFileDialog>
#include <qboxlayout.h>
#include <qlistwidget.h>
#include <qpushbutton.h>

/*
========================================================
===================== OptionsDialog ====================
========================================================
*/

OptionsDialog::OptionsDialog() :
    QDialog(nullptr)
{
    buildInterface();
    connect(this, &OptionsDialog::finished, this, [this](){this->deleteLater();});
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
    connect(apply, &QPushButton::clicked, this, &OptionsDialog::accept);

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
        
        layout->addWidget(tab);
    }
}

QWidget* OptionsDialog::createMusicTab()
{
    QWidget* tab = new QWidget();

    // Creating the main layout of the tab.
    QVBoxLayout* vLayout = new QVBoxLayout(tab);

    // The Group layout for the music folders collection.
    QGroupBox* collectionGroup = new QGroupBox(tr("Collections Folders:"), tab);
    vLayout->addWidget(collectionGroup);

    // Layout of the collectionGroup widget.
    QVBoxLayout* collectionLayout = new QVBoxLayout();
    collectionLayout->setContentsMargins(0, 0, 0, 0);
    collectionGroup->setLayout(collectionLayout);

    // A list with the folders list.
    QListWidget* foldersList = new QListWidget(tab);
    collectionLayout->addWidget(foldersList);

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
    connect(delFolderBtn, &QPushButton::clicked, [foldersList]() {
        QList<QListWidgetItem*> selectedItems = foldersList->selectedItems();

        for (QListWidgetItem* item : selectedItems)
        {
            if (foldersList->takeItem(
                foldersList->row(item)))
            {
                delete item;
            }
        }
    });

    // Action to add a new path to the list.
    connect(addFolderBtn, &QPushButton::clicked, [this, foldersList]() {
        const QString directory = QFileDialog::getExistingDirectory(this, tr("Select a folder"));
        if (!directory.isEmpty())
        {
            bool isPresent = false;

            // Check if the folder has not been already added.
            for (int i = 0; i < foldersList->count(); i++)
            {
                const QListWidgetItem* item = foldersList->item(i);
                if (item->text().compare(directory) == 0)
                {
                    isPresent = true;
                    break;
                }
            }

            if (!isPresent)
            {
                foldersList->addItem(directory);
            }
        }
    });

    return tab;
}

/*
========================================================
====================== SAPOptions ======================
========================================================
*/
SAPOptions::SAPOptions() :
    m_dialog(nullptr)
{}

SAPOptions::~SAPOptions()
{
    // Delete m_dialog if exists.
    if (m_dialog)
    {
        delete m_dialog;
    }
}

void SAPOptions::openOptions()
{
    // Check if there is not a dialog already open.
    if (m_dialog) 
    {
        return;
    }

    // Creating a new instance of OptionsDialog and run it.
    OptionsDialog* dialog = new OptionsDialog();
    m_dialog = dialog;

    // When the dialog is destroyed, set m_dialog to nullptr.
    connect(dialog, &OptionsDialog::destroyed, [this, dialog]() {
        if (this->m_dialog == dialog)
        {
            this->m_dialog = nullptr;
        }
    });

    dialog->show();
    dialog->raise();
    dialog->activateWindow();
}