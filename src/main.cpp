#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSettings>
#include <QDebug>
#include <QTime>
#include <QtGlobal>

#include "beziquehand.h"
#include "beziquematch.h"
#include "gamestate.h"
#include "player.h"
#include "card.h"
#include "gamedata.h"
#include "test.h"

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //QGuiApplication app(argc, argv);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    app.setOrganizationName("BeziqueGame");
    app.setOrganizationDomain("https://github.com/Sriep/Bezique");
    app.setApplicationName("Bezique");

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings;
    QString fileName = settings.fileName();
    qDebug() << fileName;

    QTime now = QTime::currentTime();
    int seed = now.msec();
    qDebug() << "random number seed" <<  seed;
    qsrand(seed);

    qmlRegisterType<GameData>("Bezique", 1,0, "GameData");
    qmlRegisterType<Player>("Bezique", 1,0, "Player");
    qmlRegisterType<BeziqueHand>("Bezique", 1,0, "BeziqueHand");
    qmlRegisterType<Card>("Bezique", 1,0, "Card");
    qmlRegisterType<BeziqueMatch>("Bezique", 1,0, "BeziqueMatch");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    //Game game;
    //game.start();

    return app.exec();
}
