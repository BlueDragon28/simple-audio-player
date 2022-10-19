#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <qdebug.h>
#include "Player.h"
#include "CoverImageProvider.h"
#include "MusicCollectionList.h"
#include "AppConfig.h"

#include "dbus/SAPMPris.h"

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);

    // Setting application icon
    app.setWindowIcon(QIcon(":/images/musiqueIcon.svg"));

    // Instanciate MusicCollectionList.
    MusicCollectionList::instance();

    // Reading the application config.
    AppConfig::loadConfig();

    SAPMPris mpris;

    QQmlApplicationEngine engine;
    engine.addImageProvider("coverArt", new CoverImageProvider());
    engine.addImportPath("qrc:/imports");
    engine.load(QUrl("qrc:///qml/MainWindow.qml"));

    if (engine.rootObjects().isEmpty())
    {
        qDebug() << "Failed to create the main window!";
        return EXIT_FAILURE;
    }

    int result = app.exec();

    // Saving the application config only when the application exited without problems.
    if (!result)
    {
        AppConfig::saveConfig();
    }

    return result;
}
