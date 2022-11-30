#include "options/LicenseDialog.h"
#include <qboxlayout.h>
#include <qframe.h>
#include <qlabel.h>
#include <qpushbutton.h>

LicenseDialog::LicenseDialog() :
    QDialog()
{
    buildInterface();
    connect(this, &QDialog::finished, this, &QDialog::deleteLater);
    setWindowTitle(tr("License"));
}

LicenseDialog::~LicenseDialog()
{}

void LicenseDialog::buildInterface()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* bottomLayout = new QHBoxLayout(this);

    QLabel* licenseStr = new QLabel(
        "<p>MIT License</p>"

        "<p>Copyright (c) 2022 Erwan Saclier de la Bâtie (BlueDragon28)</p>"

        "<p>Permission is hereby granted, free of charge, to any person obtaining a copy<br>"
        "of this software and associated documentation files (the \"Software\"), to deal<br>"
        "in the Software without restriction, including without limitation the rights<br>"
        "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell<br>"
        "copies of the Software, and to permit persons to whom the Software is<br>"
        "furnished to do so, subject to the following conditions:</p>"

        "<p>The above copyright notice and this permission notice shall be included in all<br>"
        "copies or substantial portions of the Software.</p>"

        "<p>THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR<br>"
        "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,<br>"
        "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE<br>"
        "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER<br>"
        "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,<br>"
        "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE<br>"
        "SOFTWARE.</p>");
    mainLayout->addWidget(licenseStr);

    QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    mainLayout->addWidget(line);

    mainLayout->addLayout(bottomLayout);
    bottomLayout->addStretch(1);

    QPushButton* okBtn = new QPushButton(tr("Ok"));
    connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
    bottomLayout->addWidget(okBtn);
}