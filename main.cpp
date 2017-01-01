#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "beziquehand.h"
#include "beziquematch.h"
#include "gamestate.h"
#include "player.h"
#include "card.h"
#include "gamedata.h"
#include "test.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    //qmlRegisterType<Test>("Bezique", 1,0, "Test");

    qmlRegisterType<GameData>("Bezique", 1,0, "GameData");
    qmlRegisterType<Player>("Bezique", 1,0, "Player");
    qmlRegisterType<BeziqueHand>("Bezique", 1,0, "BeziqueHand");
    qmlRegisterType<Card>("Bezique", 1,0, "Card");
    qmlRegisterType<BeziqueMatch>("Bezique", 1,0, "BeziqueMatch");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    //Game game;
    //game.start();

    return app.exec();
}
