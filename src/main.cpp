#include <QApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QQuickStyle>
#include <qdebug.h>
#include "Player.h"
#include "CoverImageProvider.h"
#include "MusicCollectionList.h"
#include "AppConfig.h"
#include "SystemMediaControl.h"
#include "config.h"
#ifdef WIN32
#include "windows/ListenMediaKeys.h"
#endif

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QQuickStyle::setStyle("Fusion");

    // Setting application icon
    app.setWindowIcon(QIcon(":/images/musiqueIcon.svg"));

    // Reading the application config.
    AppConfig::loadConfig();

    // Instanciate MusicCollectionList.
    MusicCollectionList::instance();

    // Initialize SystemMediaControl system.
    SystemMediaControl::init();

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

#ifdef WIN32
#ifdef USE_SMTC
        // If on Windows, deinit ListenMidiaKeys
        ListenMediaKeys::deinit();
#endif
#endif
    }

    return result;
}
