#ifndef BEZIQUEMATCH_H
#define BEZIQUEMATCH_H

#include <QQuickItem>

#include "gamedata.h"

enum SaveFormat {Json, Binary };
static const QString saveFilename = "save";

class BeziqueMatch : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(GameData* gameData READ getGameData WRITE setGameData NOTIFY gameDataChanged)
    Q_PROPERTY(QString bottomName READ getBottomName WRITE setBottomName NOTIFY bottomNameChanged)
    Q_PROPERTY(QString topName READ getTopName WRITE setTopName NOTIFY topNameChanged)
    Q_PROPERTY(int bottomGamesWon READ getBottomGamesWon WRITE setBottomGamesWon NOTIFY bottomGamesWonChanged)
    Q_PROPERTY(int topGamesWon READ getTopGamesWon WRITE setTopGamesWon NOTIFY topGamesWonChanged)
    Q_PROPERTY(bool saveAvaliable READ getSaveAvaliable WRITE setSaveAvaliable NOTIFY saveAvaliableChanged)
    Q_INTERFACES(QQmlParserStatus)

public:
    //BeziqueMatch(QQuickItem *parent = 0);

   /* virtual void classBegin()
    {
    }
    virtual void componentComplete()
    {
    }*/

    BeziqueMatch(bool restart = false, QQuickItem *parent = 0);
    bool loadMatch(SaveFormat saveFormat = Json);
    bool saveMatch(SaveFormat saveFormat = Json) const;

    QString getBottomName() const;
    void setBottomName(const QString &value);
    QString getTopName() const;
    void setTopName(const QString &value);
    int getBottomGamesWon() const;
    void setBottomGamesWon(int value);
    int getTopGamesWon() const;
    void setTopGamesWon(int value);

    GameData *getGameData() const;
    void setGameData(GameData *value);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    bool getSaveAvaliable() const;

signals:
    void gameDataChanged();
    void bottomNameChanged();
    void topNameChanged();
    void bottomGamesWonChanged();
    void topGamesWonChanged();
    void saveAvaliableChanged(bool getSaveAvaliable);

public slots:
    void gameFinished(int topScore, int bottomScore);
    void trickOver();
    void setSaveAvaliable(bool value);

private:
    void init();

    GameData* gameData;
    QString bottomName = "human";
    QString topName = "top";
    int bottomGamesWon = 0;
    int topGamesWon = 0;
    bool saveAvaliable = false;
};

#endif // BEZIQUEMATCH_H


