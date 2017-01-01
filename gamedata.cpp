#include <QThread>
#include <QDebug>

#include "player.h"
#include "gamedata.h"
#include "gamestate.h"
#include "unseencards.h"
#include "beziquematch.h"
#include "scores.h"

GameData::GameData(QQuickItem *parent)
    : QQuickItem(parent)
    , game(this)

{
    init();
}

void GameData::init()
{
   // if (beziqueMatch->getSaveAvaliable())
  //      beziqueMatch->loadMatch();
   // game.start();
}

void GameData::checkSeven(Card *aisCard, Player *activePlayer)
{
    if (aisCard->getSuit() == trumps
            && aisCard->getRank() == Card::Rank::Seven
            && !meldedSeven)
    {
        activePlayer->incScore(valueSeven(activePlayer->getScore()));
        meldedSeven = true;
    }
}

Card* GameData::getHumansCard() const
{
    return humansCard;
}

void GameData::setHumansCard(Card* value)
{
    humansCard = value;
    emit changedHumanPlayer();
}

Card* GameData::getAisCard() const
{
    return aisCard;
}

void GameData::setAisCard(Card* value)
{
    aisCard = value;
    emit changedAiPlayer();
}

Player *GameData::getAiPlayer() const
{
    return aiPlayer;
}

void GameData::setAiPlayer(Player *value)
{
    aiPlayer = value;
    aiPlayer->setGameData(this);
}

void GameData::meldSeven()
{
    // id for seven of trumps is tumps
    int sevenTrumps = trumps;

    getDeck()->swapBottom(sevenTrumps);
    faceCard->setCard(sevenTrumps);
    emit changedFaceCard();
    if (activePlayer != aiPlayer)
        aiPlayer->getUnseen().haveSeen(getDeck()->peekBottom());
    if (activePlayer != humanPlayer)
        humanPlayer->getUnseen().haveSeen(getDeck()->peekBottom());
    meldedSeven = true;
}

Player *GameData::getHumanPlayer() const
{
    return humanPlayer;
}

void GameData::setHumanPlayer(Player *value)
{
    humanPlayer = value;
    humanPlayer->setGameData(this);
}

Card *GameData::getFaceCard() const
{
    return faceCard;
}

void GameData::setFaceCard(Card *value)
{
    faceCard = value;
}

void GameData::startNewGame()
{
    game.start();
}

void GameData::cutForDeal()
{ 
    aiPlayer->setOpponent(humanPlayer);
    humanPlayer->setOpponent(aiPlayer);

    activePlayer = aiPlayer;
    emit deckCut();
}

void GameData::dealCards()
{
    isEndgame = false;
    getDeck()->shuffle();
    aiPlayer->dealtHand(getDeck()->dealHand());
    humanPlayer->dealtHand(getDeck()->dealHand());

    faceCard->setCard(getDeck()->peekBottom());
    aiPlayer->getUnseen().haveSeen(getDeck()->peekBottom());
    humanPlayer->getUnseen().haveSeen(getDeck()->peekBottom());
    humanPlayer->getHand()->syncHands();
    aiPlayer->getHand()->syncHands();
    meldedSeven = false;

    emit changedFaceCard();
    setTrumps(faceCard->getSuit());
    emit handsDealt();
    setCardsInStock(getDeck()->size());

}

void GameData::leadToTrick()
{
    trickOver = false;
    if (activePlayer->isAi())
    {
        aisCard = activePlayer->playFirstCard(isEndgame);
        if  (!isEndgame)
            checkSeven(aisCard, activePlayer);
        emit changedAisCard();
        switchActivePlayer();
        humanPlayer->setCanFollowCards(aisCard, isEndgame, trumps);
        emit leadCardPlayed();
    }
    else
    {
        isPlayFirstCard = true;
        emit waitingForCard();
    }
}

void GameData::followToTrick()
{
    if (activePlayer->isAi())
    {
        aisCard = activePlayer->playSecondCard(humansCard, isEndgame);
        if  (!isEndgame)
            checkSeven(aisCard, activePlayer);
        emit changedAisCard();
        emit followedToTrick();
    }
   else
    {
        isPlayFirstCard = false;
        emit waitingForCard();
    }
}

void GameData::cardPlayed(int index, bool melded)
{
    if (!activePlayer->cardExists(index, melded))
        emit waitingForCard();
    else if (isPlayFirstCard)
    {
        humansCard = activePlayer->playCard(index, melded);
        if  (!isEndgame)
            checkSeven(humansCard, activePlayer);
        emit changedHumansCard();
        switchActivePlayer();
        emit leadCardPlayed();
    }
    else
    {
        humansCard = activePlayer->playCard(index, melded);
        if  (!isEndgame)
            checkSeven(humansCard, activePlayer);
        emit changedHumansCard();
        emit followedToTrick();
    }
}

void GameData::meld()
{
    trickOver = false;
    if (activePlayer == aiPlayer )
        activePlayer = humansCard->beats(*aisCard, trumps) ? humanPlayer : aiPlayer;
    else
        activePlayer = aisCard->beats(*humansCard, trumps) ? aiPlayer : humanPlayer;

    if (Card::Ace == humansCard->getRank() || Card::Ten == humansCard->getRank())
        activePlayer->incScore(10);
    if (Card::Ace == aisCard->getRank() || Card::Ten == aisCard->getRank())
        activePlayer->incScore(10);

    if (activePlayer->isAi())
    {
        activePlayer->meldAuto(trumps, meldedSeven);
        emit drawing();
    }
    else
    {
        humanPlayer->getHand()->refreshMelds(trumps, meldedSeven);
        if (humanPlayer->canMeld())
            emit waitingForMeld();
        else
            emit drawing();
    }
}

void GameData::endHand()
{
    isEndgame = false;
}

void GameData::endGame()
{
    isEndgame  = false;
}

// call from qml
void GameData::humanMeld(bool meldMade, int index, bool meldRow)
{
    if (meldMade)
    {
        humanPlayer->meldCard(humanPlayer->getHand()->getLink(index, meldRow)
                              , trumps
                              , meldedSeven);
        if (humanPlayer->canMeld())
            emit waitingForMeld();
        else
            emit drawing();
    }
    else
        emit drawing();
}

void GameData::finishTrick()
{
    if (trickOver) return;
    emit trickFinished();
    trickOver = true;

    humansCard = NULL;
    aisCard = NULL;

    humanPlayer->dump();
    aiPlayer->dump();

    int aiCardId = getDeck()->dealTop();
    aiPlayer->giveCard(aiCardId);
    humanPlayer->getUnseen().haveSeen(aiCardId);

    int humanCardId = getDeck()->dealTop();
    humanPlayer->giveCard(humanCardId);
    aiPlayer->getUnseen().haveSeen(humanCardId);
    qDebug() << "Deck size: " << getDeck()->size();
    setCardsInStock(getDeck()->size());

    humanPlayer->getHand()->syncHands();
    aiPlayer->getHand()->syncHands();

    humanPlayer->dump();
    aiPlayer->dump();
    beziqueMatch->trickOver();

    if (activePlayer->won())
        emit gameOver();
    else if (getDeck()->empty())
    {
        ResetBoardForEndgame();
    }
    else
        emit melded();
}

void GameData::ResetBoardForEndgame()
{
    aiPlayer->getHand()->moveAllHidden();
    humanPlayer->getHand()->moveAllHidden();
    faceCard->clearCard();
    isEndgame = true;
    emit startEndgame();
}

void GameData::setBeziqueMatch(BeziqueMatch *value)
{
    beziqueMatch = value;
}

int GameData::getCardsInStock() const
{
    return cardsInStock;
}

void GameData::setCardsInStock(int value)
{
    if ( cardsInStock != value)
    {
        cardsInStock = value;
        emit cardsInStockChanged();
    }
}

void GameData::setTrumps(int value)
{
    trumps = value;
    changedTrumps();
}

BeziqueDeck *GameData::getDeck()
{
    return &deck;
}

bool GameData::getMeldedSeven() const
{
    return meldedSeven;
}

int GameData::getTrumps() const
{
    return trumps;
}

void GameData::switchActivePlayer()
{
    activePlayer = activePlayer == aiPlayer ? humanPlayer : aiPlayer;
}

void GameData::scoreEndTrick()
{
    if (trickOver) return;
    trickOver = true;
    if (activePlayer == aiPlayer )
        activePlayer = humansCard->beats(*aisCard, trumps) ? humanPlayer : aiPlayer;
    else
        activePlayer = aisCard->beats(*humansCard, trumps) ? aiPlayer : humanPlayer;

    if (Card::Ace == humansCard->getRank() || Card::Ten == humansCard->getRank())
        activePlayer->incScore(10);
    if (Card::Ace == aisCard->getRank() || Card::Ten == aisCard->getRank())
        activePlayer->incScore(10);

    if (activePlayer->won())
        emit gameOver();
    if (activePlayer->handEmpty())
    {
        activePlayer->incScore(10);
        if (activePlayer->won())
            emit gameOver();
        else
            emit handOver();
    }
    beziqueMatch->trickOver();

    humanPlayer->getHand()->syncHands();
    aiPlayer->getHand()->syncHands();
    emit trickFinished();
}

void GameData::read(const QJsonObject &json)
{
    QJsonObject faceCardObject = json["faceCard"].toObject();
    faceCard->read(faceCardObject);

    QJsonObject playerObject = json["player"].toObject();
    humanPlayer->read(playerObject);

    QJsonObject computerObject = json["ai"].toObject();
    aiPlayer->read(computerObject);

    QJsonObject deckObject = json["deck"].toObject();
    getDeck()->read(deckObject);
    setCardsInStock(getDeck()->size());

    meldedSeven = json["meldedSeven"].toBool();
    humanStarted = json["humanStarted"].toBool();
    activePlayer = json["humanActivePlayer"].toBool() ? humanPlayer : aiPlayer;
    trumps = json["trumps"].toInt();
    isEndgame = json["isEndgame"].toBool();

    isHandOver = false;
    isGameOver = false;
    reset = false;
    isEndgame = false;
    QString gameState = json["gameSteate"].toString();
    if (gameState == GS_HAND_OVER)
        isHandOver = true;
    else if (gameState == GS_GAME_OVER)
        isGameOver = true;
    else if (gameState == GS_RESET)
        reset = true;
    else
        isEndgame = (gameState == GS_ENDGAME);
}

void GameData::write(QJsonObject &json) const
{
    QJsonObject faceCardObject;
    faceCard->write(faceCardObject);
    json["faceCard"] = faceCardObject;

    QJsonObject playerObject;
    humanPlayer->write(playerObject);
    json["player"] = playerObject;

    QJsonObject computerObject;
    aiPlayer->write(computerObject);
    json["ai"] = computerObject;

    QJsonObject deckObject;
    deck.write(deckObject);
    json["deck"] = deckObject;

    json["meldedSeven"] = meldedSeven;
    json["humanStarted"] = humanStarted;
    json["humanActivePlayer"] = activePlayer == humanPlayer;
    json["trumps"] = trumps;
    json["isEndgame"] = isEndgame;

    if (isHandOver)
        json["gameState"] = GS_HAND_OVER;
    else if (isGameOver)
        json["gameState"] = GS_GAME_OVER;
    else if (reset)
        json["gameState"] = GS_RESET;
    else if (isEndgame)
        json["gameState"] = GS_ENDGAME;
    else
        json["gameState"] = GS_EARLY_GAME;

}




























































