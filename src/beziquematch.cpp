#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QIODevice>
#include <QJsonObject>
#include <QSettings>

#include "beziquematch.h"

BeziqueMatch::BeziqueMatch(bool restart, QQuickItem *parent)
    : QQuickItem(parent)
{    
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
/*    QFile loadFile(saveFormat == Json
                   ? QStringLiteral("save.json")
                   : QStringLiteral("save.dat"));

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(saveFormat == Json
        ? QJsonDocument::fromJson(saveData)
        : QJsonDocument::fromBinaryData(saveData));
*/
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

bool BeziqueMatch::saveMatch() const
{
    QJsonObject gameObject;
    write(gameObject);
    QJsonDocument saveDoc(gameObject);
/*
    QFile saveFile(saveFormat == Json
                   ? QStringLiteral("save.json")
                   : QStringLiteral("save.dat"));
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    saveFile.write(saveFormat == Json
        ? saveDoc.toJson()
        : saveDoc.toBinaryData());
*/
    QSettings settings;
    QByteArray ba;
    ba = saveDoc.toJson(QJsonDocument::Compact);//(QJsonDocument::Indented);
    settings.setValue("data/gameData", QVariant(ba));

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













































