#include <QState>
#include <QFinalState>
#include <cstdlib>
#include "gamestate.h"
#include "player.h"
#include "card.h"
#include "gamedata.h"

GameState::GameState(GameData* gameData, QStateMachine *parent)
    : QStateMachine(parent), gameData(gameData)
{
    init();
}

GameState::GameState(QState *initalState, GameData *gameData, QStateMachine *parent)
: QStateMachine(parent), gameData(gameData)
{
    init(initalState);
}

GameState::~GameState()
{
}

void GameState::init(QState *initalState)
{
    QState *cutForDeal = new QState();
    if (NULL == initalState)
        initalState = cutForDeal;
    QState *dealCards = new QState();

    QState *leadToTrick = new QState();
    QState *followToTrick = new QState();
    QState *meld = new QState();

    QState *leadEndTrick = new QState();
    QState *followEndTrick = new QState();
    QState *finishEndTrick = new QState();

    QFinalState *cleanUp = new QFinalState();

    QObject::connect(cutForDeal, SIGNAL(entered()), this->gameData, SLOT(cutForDeal()));
    cutForDeal->addTransition(this->gameData, SIGNAL(deckCut()), dealCards);

    QObject::connect(dealCards, SIGNAL(entered()), this->gameData, SLOT(dealCards()));
    dealCards->addTransition(this->gameData, SIGNAL(handsDealt()), leadToTrick);

    leadToTrick->addTransition(this->gameData, SIGNAL(leadCardPlayed()), followToTrick);
    followToTrick->addTransition(this->gameData, SIGNAL(followedToTrick()), meld);
    meld->addTransition(this->gameData, SIGNAL(melded()), leadToTrick);
    meld->addTransition(this->gameData, SIGNAL(startEndgame()), leadEndTrick);
    meld->addTransition(this->gameData, SIGNAL(gameOver()), cleanUp);

    QObject::connect(leadToTrick, SIGNAL(entered()), this->gameData, SLOT(leadToTrick()));
    QObject::connect(followToTrick, SIGNAL(entered()), this->gameData, SLOT(followToTrick()));
    QObject::connect(meld, SIGNAL(entered()), this->gameData, SLOT(meld()));

    //QObject::connect(endGameTrick, SIGNAL(entered()), this->gameData, SLOT(playEndTrick()));
    leadEndTrick->addTransition(this->gameData, SIGNAL(leadCardPlayed()), followEndTrick);
    followEndTrick->addTransition(this->gameData, SIGNAL(followedToTrick()), finishEndTrick);
    finishEndTrick->addTransition(this->gameData, SIGNAL(trickFinished()), leadEndTrick);
    finishEndTrick->addTransition(this->gameData, SIGNAL(handOver()), dealCards);
    finishEndTrick->addTransition(this->gameData, SIGNAL(gameOver()), cleanUp);

    QObject::connect(leadEndTrick, SIGNAL(entered()), this->gameData, SLOT(leadToTrick()));
    QObject::connect(followEndTrick, SIGNAL(entered()), this->gameData, SLOT(followToTrick()));
    QObject::connect(finishEndTrick, SIGNAL(entered()), this->gameData, SLOT(ScoreEndTrick()));

    QObject::connect(cleanUp, SIGNAL(entered()), this->gameData, SLOT(endGame()));       

    this->addState(cutForDeal);
    this->addState(dealCards);

    this->addState(leadToTrick);
    this->addState(followToTrick);
    this->addState(meld);
;
    this->addState(leadEndTrick);
    this->addState(followEndTrick);
    this->addState(finishEndTrick);

    this->addState(cleanUp);

    this->setInitialState(initalState);
}





























