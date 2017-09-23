#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QIODevice>
#include <QJsonObject>
#include <QSettings>

#include "beziquematch.h"

BeziqueMatch::BeziqueMatch(bool , QQuickItem *parent)
    : QQuickItem(parent)
{    
    QSettings settings;
    setSaveAvaliable(settings.contains("data/gameData"));
    //qsrand(time(NULL));
    /*if (restart)
    {
        setSaveAvaliable(false);
    }
    else
    {
        QFile loadFile(QStringLiteral("save.json"));
        setSaveAvaliable(loadFile.exists());
        //if (saveAvaliable) loadMatch();
    }*/
    //loadMatch();
}

bool BeziqueMatch::loadMatch()
{

    QSettings settings;
    if (!settings.contains("data/gameData"))
        return false;
    else
    {

        QByteArray ba(settings.value("data/gameData").toByteArray());
        QJsonDocument gameData;
        gameData = QJsonDocument::fromJson(ba);
        read(gameData.object());
        return true;
    }
}

bool BeziqueMatch::saveMatch()
{
    QJsonObject gameObject;
    write(gameObject);
    QJsonDocument saveDoc(gameObject);

    QSettings settings;
    QByteArray ba;
    ba = saveDoc.toJson(QJsonDocument::Compact);//(QJsonDocument::Indented);
    settings.setValue("data/gameData", QVariant(ba));
    setSaveAvaliable(true);
    return true;
}

QString BeziqueMatch::getBottomName() const
{
    return bottomName;
}

void BeziqueMatch::setBottomName(const QString &value)
{
    if (bottomName != value)
    {
        bottomName = value;
        emit bottomNameChanged();
    }
}

QString BeziqueMatch::getTopName() const
{
    return topName;
}

void BeziqueMatch::setTopName(const QString &value)
{
    if (topName != value)
    {
        topName = value;
        emit topNameChanged();
    }
}

int BeziqueMatch::getBottomGamesWon() const
{
    return bottomGamesWon;
}

void BeziqueMatch::setBottomGamesWon(int value)
{
    if (bottomGamesWon != value)
    {
        bottomGamesWon = value;
        emit bottomGamesWonChanged();
    }
}

int BeziqueMatch::getTopGamesWon() const
{
    return topGamesWon;
}

void BeziqueMatch::setTopGamesWon(int value)
{
    if (topGamesWon != value)
    {
        topGamesWon = value;
        emit topGamesWonChanged();
    }
}

GameData *BeziqueMatch::getGameData() const
{
    return gameData;
}

void BeziqueMatch::setGameData(GameData *value)
{
    if(gameData != value)
    {
        gameData = value;
        gameData->setBeziqueMatch(this);
        emit gameDataChanged();
    }
}

void BeziqueMatch::gameFinished(int topScore, int bottomScore)
{
    if (topScore > bottomScore)
        setTopGamesWon(getTopGamesWon() + 1);
    else
        setBottomGamesWon(getBottomGamesWon() + 1);
}

void BeziqueMatch::trickOver()
{
    saveMatch();
}

void BeziqueMatch::setSaveAvaliable(bool value)
{
    if (saveAvaliable == value)
        return;

    saveAvaliable = value;
    emit saveAvaliableChanged(saveAvaliable);
}

bool BeziqueMatch::getSaveAvaliable() const
{
    return saveAvaliable;
}

void BeziqueMatch::read(const QJsonObject &json)
{
    bottomName = json["bottomName"].toString();
    topName = json["topName"].toString();
    bottomGamesWon = json["bottomGamesWon"].toInt();
    topGamesWon = json["topGamesWon"].toInt();

    QJsonObject gameDataObject = json["gameData"].toObject();
    gameData->read(gameDataObject);
}

void BeziqueMatch::write(QJsonObject &json) const
{
    json["bottomName"] = bottomName;
    json["topName"] = topName;
    json["bottomGamesWon"] = bottomGamesWon;
    json["topGamesWon"] = topGamesWon;

    QJsonObject gameDataObject;
    gameData->write(gameDataObject);
    json["gameData"] = gameDataObject;
}













































