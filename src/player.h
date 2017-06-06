#ifndef PLAYER_H
#define PLAYER_H
//#include <vector>
#include <QList>
#include <QQuickItem>
#include <QObject>
#include "beziqueDeck.h"
#include "beziquehand.h"
#include "gamestate.h"
#include "card.h"
#include "unseencards.h"

using namespace std;

class Player : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(BeziqueHand* hand READ getHand WRITE setHand)
    Q_PROPERTY(bool ai READ isAi WRITE setAi)
    Q_PROPERTY(int score READ getScore WRITE setScore NOTIFY scoreChanged)
public:
    static const int winningThreshold = 1000;

    Player(QQuickItem *parent = 0);
    //Player();

    void dealtHand(QList<int> dealtHand);
    Card *playFirstCard(bool isEndgame = false);
    Card *playSecondCard(Card* leadCard = NULL, bool isEndgame = false);
    Card* playFirstCardEndgame();
    Card *playSecondCardEndgame(Card* firstCard);
    Card* aiPlayCard(bool leadCard);
    void meldAuto(int trumps, bool seven);
    void meldRecursive(int trumps, bool seven);
    void meldCard(int index, int trumps, bool seven);
    void giveCard(int iCard);
    bool handEmpty() const;
    bool won() const;
    void incScore(int increment);
    Card* playCard(int index, bool melded = false);
    bool canMeld();
    void meldSeven(int index);

    int getScore() const;
    void setScore(int value);

    //virtual bool isControlled() const = 0;

    BeziqueHand *getHand() const;
    void setHand(BeziqueHand *value);

    bool isAi() const;
    void setAi(bool value);
    UnseenCards getUnseen();

    Player *getOpponent() const;
    void setOpponent(Player *value);
    bool cardExists(int index, bool melded);
    void setGameData(GameData *value);
    void setCanFollowCards(Card* oppCard, bool isEndgame, int trumps);

    void dump();

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
signals:
    void scoreChanged();

private:
    void init();
    int semiRandomCard() const;

    BeziqueHand* hand;
    Player* opponent;
    UnseenCards unseen;
    GameData* gameData;
    int score = 0;
    bool ai;
};

#endif // PLAYER_H
