#include "options/SAPOptions.h"
#include <QtWidgets/qdialog.h>
#include <QtWidgets/QFrame>
#include <QtWidgets/QTabWidget>
#include <qboxlayout.h>
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
        tab->addTab(new QWidget(), "Test");
        
        layout->addWidget(tab);
    }
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