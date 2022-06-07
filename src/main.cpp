#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Player.h"

// Register the singletone type Player inside the module SAL.
void registerTypes()
{
    qmlRegisterSingletonType<Player>("SAL", 1, 0, "Player", Player::qmlRegistration);
}

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);

    registerTypes();

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/imports");
    engine.load(QUrl("qrc:///qml/MainWindow.qml"));

    if (engine.rootObjects().isEmpty())
    {
        qDebug() << "Failed to create the main window!";
        return EXIT_FAILURE;
    }

    return app.exec();
}