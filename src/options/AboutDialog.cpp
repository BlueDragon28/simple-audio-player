#include "AboutDialog.h"
#include "config.h"
#include <qboxlayout.h>
#include <qframe.h>
#include <qlabel.h>
#include <qnamespace.h>
#include <qpushbutton.h>

AboutDialog::AboutDialog() :
    QDialog()
{
    buildInterface();

    connect(this, &QDialog::finished, this, &QDialog::deleteLater);

    setWindowTitle(tr("About")); // Set the Dialog window name.
}

AboutDialog::~AboutDialog()
{}

void AboutDialog::buildInterface()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* bottomLayout = new QHBoxLayout(this);

    QLabel* aboutLabel = new QLabel(
        "<p>"
        "<b>name:</b> " SAP_NAME "<br>"
        "<b>version:</b> " SAP_VERSION "<br>"
        "<b>license:</b> " SAP_LICENSE "<br>"
        "<br>"
        "<b>simple-audio-library:</b><br>"
        "<b>version:</b> 0.1<br>"
        "<b>license:</b> MIT<br>"
        "<br>"
        "<b>Qt</b><br>"
        "<b>version:</br> " QT_VERSION_STR "<br>"
        "<b>license:</br> LGPL <br>"
        "</p>"
    );
    aboutLabel->setAlignment(Qt::AlignVCenter);
    mainLayout->addWidget(aboutLabel);

    // Line separation
    QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    mainLayout->addWidget(line);

    mainLayout->addLayout(bottomLayout);

    QPushButton* okBtn = new QPushButton(tr("Ok"), this);
    connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);

    bottomLayout->addStretch(1);
    bottomLayout->addWidget(okBtn);
}