#ifndef SIMPLEAUDIOPLAYER_SAPOPTIONS_H_
#define SIMPLEAUDIOPLAYER_SAPOPTIONS_H_

#include <QtWidgets/QDialog>
#include <QtWidgets/QBoxLayout>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class QVBoxLayout;
class QListWidget;
class AboutDialog;
class LicenseDialog;

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

    /*
    Creating the QTabWidget view with all the content.
    */
    void createMainView(QVBoxLayout* layout);

    /*
    Save the settings when the apply button is clicked.
    */
    void applyChange();

    /*
    ========================================================
    ================== QTabWidget tab ======================
    ========================================================
    */
    /*
    Musics Collection
    */
    QWidget* createMusicTab();
    void createMusicCollectionList(QBoxLayout* layout, QWidget* parent);
    void initCollectionListWidget();
    void saveCollectionList();

    QListWidget* m_folderList;
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

    /*
    Open the about dialog.
    */
    void openAbout();

    /*
    Open the license dialog.
    */
    void openLicense();

private:
    /*
    Template code to open a dialog.
    */
    template<typename T>
    void openDialog(T** memberVar);

    OptionsDialog* m_optionsDialog;
    AboutDialog* m_aboutDialog;
    LicenseDialog* m_licenseDialog;
};

#endif // SIMPLEAUDIOPLAYER_SAPOPTIONS_H_