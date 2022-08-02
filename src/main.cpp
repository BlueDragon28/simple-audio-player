#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Player.h"
#include "CoverImageProvider.h"
#include "MusicCollectionList.h"

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);

    // Instanciate MusicCollectionList.
    MusicCollectionList::instance();

    QQmlApplicationEngine engine;
    engine.addImageProvider("coverArt", new CoverImageProvider());
    engine.addImportPath("qrc:/imports");
    engine.load(QUrl("qrc:///qml/MainWindow.qml"));

    if (engine.rootObjects().isEmpty())
    {
        qDebug() << "Failed to create the main window!";
        return EXIT_FAILURE;
    }

    return app.exec();
}
