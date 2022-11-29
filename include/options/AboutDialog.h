#ifndef SIMPLEAUDIOPLAYER_ABOUTDIALOG_H_
#define SIMPLEAUDIOPLAYER_ABOUTDIALOG_H_

#include <QtWidgets/QDialog>

/*
A Simple Dialog displaying information about the application.
*/
class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    AboutDialog();
    ~AboutDialog();

private:
    void buildInterface();
};

#endif // SIMPLEAUDIOPLAYER_ABOUTDIALOG_H_