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

//GameState::GameState(bool newGame, GameData *gameData, QStateMachine *parent)
//: QStateMachine(parent), gameData(gameData)
//{
//    init();
//}

GameState::~GameState()
{
}

void GameState::init()
{
    QState *initalState = new QState();
    QState *cutForDeal = new QState();
    QState *dealCards = new QState();

    QState *leadToTrick = new QState();
    QState *followToTrick = new QState();
    QState *meld = new QState();

    QState *leadEndTrick = new QState();
    QState *followEndTrick = new QState();
    QState *finishEndTrick = new QState();
    QState *readIn = new QState();

    QFinalState *cleanUp = new QFinalState();

    QObject::connect(initalState, SIGNAL(entered()), this->gameData, SLOT(initaliseGame()));
    initalState->addTransition(this->gameData, SIGNAL(setUpNewGame()), cutForDeal);
    initalState->addTransition(this->gameData, SIGNAL(readInGame()), readIn);

    QObject::connect(cutForDeal, SIGNAL(entered()), this->gameData, SLOT(cutForDeal()));
    cutForDeal->addTransition(this->gameData, SIGNAL(deckCut()), dealCards);

    QObject::connect(dealCards, SIGNAL(entered()), this->gameData, SLOT(dealCards()));
    dealCards->addTransition(this->gameData, SIGNAL(handsDealt()), leadToTrick);

    leadToTrick->addTransition(this->gameData, SIGNAL(leadCardPlayed()), followToTrick);
    followToTrick->addTransition(this->gameData, SIGNAL(followedToTrick()), meld);
    meld->addTransition(this->gameData, SIGNAL(melded()), leadToTrick);
    meld->addTransition(this->gameData, SIGNAL(startEndgame()), leadEndTrick);
    meld->addTransition(this->gameData, SIGNAL(gameOver(bool)), cleanUp);

    QObject::connect(leadToTrick, SIGNAL(entered()), this->gameData, SLOT(leadToTrick()));
    QObject::connect(followToTrick, SIGNAL(entered()), this->gameData, SLOT(followToTrick()));
    QObject::connect(meld, SIGNAL(entered()), this->gameData, SLOT(meld()));

    //QObject::connect(endGameTrick, SIGNAL(entered()), this->gameData, SLOT(playEndTrick()));
    leadEndTrick->addTransition(this->gameData, SIGNAL(leadCardPlayed()), followEndTrick);
    followEndTrick->addTransition(this->gameData, SIGNAL(followedToTrick()), finishEndTrick);
    finishEndTrick->addTransition(this->gameData, SIGNAL(trickFinished()), leadEndTrick);
    finishEndTrick->addTransition(this->gameData, SIGNAL(handOver()), dealCards);
    finishEndTrick->addTransition(this->gameData, SIGNAL(gameOver(bool)), cleanUp);

    QObject::connect(leadEndTrick, SIGNAL(entered()), this->gameData, SLOT(leadToTrick()));
    QObject::connect(followEndTrick, SIGNAL(entered()), this->gameData, SLOT(followToTrick()));
    QObject::connect(finishEndTrick, SIGNAL(entered()), this->gameData, SLOT(scoreEndTrick()));

    QObject::connect(cleanUp, SIGNAL(entered()), this->gameData, SLOT(endGame()));

    QObject::connect(readIn, SIGNAL(entered()), this->gameData, SLOT(readIn()));
    readIn->addTransition(this->gameData, SIGNAL(handOver()), dealCards);
    readIn->addTransition(this->gameData, SIGNAL(gameOver(bool)), cutForDeal);
    readIn->addTransition(this->gameData, SIGNAL(nextTrick()), leadToTrick);
    readIn->addTransition(this->gameData, SIGNAL(nextEndTrick()), leadEndTrick);

    cutForDeal->addTransition(this->gameData, SIGNAL(readInGame()), readIn);
    dealCards->addTransition(this->gameData, SIGNAL(readInGame()), readIn);
    leadToTrick->addTransition(this->gameData, SIGNAL(readInGame()), readIn);
    followToTrick->addTransition(this->gameData, SIGNAL(readInGame()), readIn);
    meld->addTransition(this->gameData, SIGNAL(readInGame()), readIn);
    leadEndTrick->addTransition(this->gameData, SIGNAL(readInGame()), readIn);
    followEndTrick->addTransition(this->gameData, SIGNAL(readInGame()), readIn);
    finishEndTrick->addTransition(this->gameData, SIGNAL(readInGame()), readIn);

    this->addState(initalState);

    this->addState(readIn);

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

    //this->setInitialState(cutForDeal);
    this->setInitialState(initalState);
}





























