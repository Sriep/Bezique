#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QIODevice>
#include <QJsonObject>

#include "beziquematch.h"


BeziqueMatch::BeziqueMatch(bool restart, QQuickItem *parent)
    : QQuickItem(parent)
{    
    if (restart)
    {
        setSaveAvaliable(false);
    }
    else
    {
        QFile loadFile(QStringLiteral("save.json"));
        setSaveAvaliable(loadFile.exists());
        //if (saveAvaliable) loadMatch();
    }
}

void BeziqueMatch::init()
{
/*    QFile loadFile(QStringLiteral("save.json"));
    setSaveAvaliable(loadFile.exists());
    if (saveAvaliable)
        loadMatch();

*/
}

bool BeziqueMatch::loadMatch(SaveFormat saveFormat)
{
    QFile loadFile(saveFormat == Json
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

    read(loadDoc.object());

    return true;
}

bool BeziqueMatch::saveMatch(SaveFormat saveFormat) const
{
    QFile saveFile(saveFormat == Json
                   ? QStringLiteral("save.json")
                   : QStringLiteral("save.dat"));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject gameObject;
    write(gameObject);
    QJsonDocument saveDoc(gameObject);
    saveFile.write(saveFormat == Json
        ? saveDoc.toJson()
        : saveDoc.toBinaryData());

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
    saveMatch(Json);
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













































