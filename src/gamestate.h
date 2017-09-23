#ifndef GAME_H
#define GAME_H
#include <QStateMachine>
#include <memory>

//#include "card.h"
#include "beziqueDeck.h"

using namespace std;
class Player;
class Card;
class GameData;

class GameState : public QStateMachine
{
    Q_OBJECT
public:
    GameState(GameData* gameData, QStateMachine *parent = 0);
    //GameState(bool newGame, GameData* gameData, QStateMachine *parent= 0 );
    virtual ~GameState();

private slots:
public:
signals:

private:
    void init();

    GameData* gameData;
};


#endif // GAME_H
