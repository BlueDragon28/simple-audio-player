#ifndef SIMPLEAUDIOPLAYER_SAPOPTIONS_H_
#define SIMPLEAUDIOPLAYER_SAPOPTIONS_H_

#include <QtWidgets/QDialog>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

/*
The Dialog with the options selection inside.
*/
class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    OptionsDialog();
    ~OptionsDialog();

private:
    void buildInterface();
};

/*
An QML interface to interact and launch the options dialog.
*/
class SAPOptions : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    SAPOptions();
    ~SAPOptions();

public slots:
    /*
    Open the options dialog.
    */
    void openOptions();

private:
    OptionsDialog* m_dialog;
};

#endif // SIMPLEAUDIOPLAYER_SAPOPTIONS_H_