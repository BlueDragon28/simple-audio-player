#ifndef SIMPLEAUDIOPLAYER_LICENSEDIALOG_H_
#define SIMPLEAUDIOPLAYER_LICENSEDIALOG_H_

#include <qdialog.h>

/*
This dialog display the license of the program.
*/
class LicenseDialog : public QDialog
{
    Q_OBJECT

public:
    LicenseDialog();
    ~LicenseDialog();

private:
    void buildInterface();
};

#endif // SIMPLEAUDIOPLAYER_LICENSEDIALOG_H_