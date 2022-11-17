#include "options/SAPOptions.h"
#include <QtWidgets/qdialog.h>
#include <QtWidgets/QPushButton>
#include <qforeach.h>

/*
========================================================
===================== OptionsDialog ====================
========================================================
*/

OptionsDialog::OptionsDialog() :
    QDialog(nullptr)
{
    buildInterface();
}

OptionsDialog::~OptionsDialog()
{}

void OptionsDialog::buildInterface()
{
    QPushButton *btn = new QPushButton("Hello There", this);
    connect(btn, &QPushButton::clicked, this, &OptionsDialog::accept);
    connect(btn, &QPushButton::clicked, this, &OptionsDialog::deleteLater);
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