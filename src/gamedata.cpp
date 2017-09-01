#include <QThread>
#include <QDebug>
#include <cstdlib>
#include <QtGlobal>

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
    //init();
    //resetGameData();
    //game.start();
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

void GameData::inspect()
{

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
    startingNewMatch = true;
    resetGameData();
    //game.stop();
    //game.start();
    emit restartSM();
    //readingInGame = false;
}

void GameData::continueGame()
{
    readingInGame = true;
    resetGameData();
    //game.start();
    emit restartSM();

}

void GameData::initaliseGame()
{
    if (readingInGame)
        emit readInGame();
    else if (startingNewMatch)
        emit setUpNewGame();
    readingInGame = false;
    startingNewMatch = false;
}

void GameData::cutForDeal()
{ 
    aiPlayer->setOpponent(humanPlayer);
    humanPlayer->setOpponent(aiPlayer);
    //activePlayer = aiPlayer;
    int r = qrand() % 2;
    //int rr = r % 2;
    //activePlayer = (rr==0) ? aiPlayer : humanPlayer;
    if (r == 0)
    {
        activePlayer = humanPlayer;
        emit userMessage("Cut for deal"
                    ,"That means you play first"
                    ,"Your opponent won the cut for deal");
    }
    else
    {
        activePlayer = aiPlayer;
        emit userMessage("Cut for deal"
                    ,"That means your opponet plays first"
                    ,"You won the cut for deal");
    }
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
        humanPlayer->setCanLeadCards();
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

void GameData::readIn()
{
    beziqueMatch->loadMatch();
    emit changedAiPlayer();
    emit changedHumanPlayer();
    emit changedFaceCard();
    emit handsDealt();
}


// call from qml
void GameData::humanMeld(bool meldMade, int index, bool meldRow)
{
    if (meldMade)
    {
        humanPlayer->meldCard(humanPlayer->getHand()->getLink(index, meldRow)
                              , trumps
                              , meldedSeven);
        //if (humanPlayer->canMeld())
        //    emit waitingForMeld();
        //else
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
    //if (!isEndgame)
    drawFromStock();

    if (activePlayer->won())
        emit gameOver(!activePlayer->isAi());

    humanPlayer->getHand()->syncHands();
    aiPlayer->getHand()->syncHands();
    humanPlayer->dump();
    aiPlayer->dump();
    beziqueMatch->trickOver();

    if (getDeck()->empty())
    {
        ResetBoardForEndgame();
    }
    else
        emit melded();
}

void GameData::drawFromStock()
{
    int aiCardId = getDeck()->dealTop();
    aiPlayer->giveCard(aiCardId);
    humanPlayer->getUnseen().haveSeen(aiCardId);

    int humanCardId = getDeck()->dealTop();
    humanPlayer->giveCard(humanCardId);
    aiPlayer->getUnseen().haveSeen(humanCardId);
    qDebug() << "Deck size: " << getDeck()->size();
    setCardsInStock(getDeck()->size());
}

bool GameData::isGameInProgress()
{
    return game.isRunning();
}

void GameData::initiliseGameData()
{
    resetGameData();
    game.start();
}

void GameData::ResetBoardForEndgame()
{
    aiPlayer->getHand()->moveAllHidden();
    humanPlayer->getHand()->moveAllHidden();
    faceCard->clearCard();
    isEndgame = true;
    emit startEndgame();
}

void GameData::resetGameData()
{
    meldedSeven = false;
    aiPlayer->resetData();
    emit changedAiPlayer();
    humanPlayer->resetData();
    emit changedHumanPlayer();
}

void GameData::setBeziqueMatch(BeziqueMatch *value)
{
    beziqueMatch = value;
}

int GameData::winningThreshold() const
{
    return m_winningThreshold;
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

void GameData::finishEndTrick()
{
    if (trickOver) return;
    emit trickFinished();
    trickOver = true;

    if (activePlayer == aiPlayer )
        activePlayer = humansCard->beats(*aisCard, trumps) ? humanPlayer : aiPlayer;
    else
        activePlayer = aisCard->beats(*humansCard, trumps) ? aiPlayer : humanPlayer;

    if (Card::Ace == humansCard->getRank() || Card::Ten == humansCard->getRank())
        activePlayer->incScore(10);
    if (Card::Ace == aisCard->getRank() || Card::Ten == aisCard->getRank())
        activePlayer->incScore(10);

    humansCard = NULL;
    aisCard = NULL;

    if (activePlayer->won())
        emit gameOver(!activePlayer->isAi());

    if (activePlayer->handEmpty())
    {
        activePlayer->incScore(10);
        if (activePlayer->won())
            emit gameOver(!activePlayer->isAi());
        else
            emit handOver();
    }

    humanPlayer->getHand()->syncHands();
    aiPlayer->getHand()->syncHands();
    beziqueMatch->trickOver();

    emit inspectedEndCards();
}

void GameData::setWinningThreshold(int winningThreshold)
{
    if (m_winningThreshold == winningThreshold)
        return;

    m_winningThreshold = winningThreshold;
    emit winningThresholdChanged(m_winningThreshold);
}

void GameData::read(const QJsonObject &json)
{
    QJsonObject faceCardObject = json["faceCard"].toObject();
    faceCard->read(faceCardObject);

    QJsonObject playerObject = json["player"].toObject();
    humanPlayer->read(playerObject);

    QJsonObject computerObject = json["ai"].toObject();
    aiPlayer->read(computerObject);

    aiPlayer->setOpponent(humanPlayer);
    humanPlayer->setOpponent(aiPlayer);

    activePlayer = json["aiActivePlayer"].toBool() ? aiPlayer : humanPlayer;

    QJsonObject deckObject = json["deck"].toObject();
    getDeck()->read(deckObject);
    setCardsInStock(getDeck()->size());

    meldedSeven = json["meldedSeven"].toBool();
    humanStarted = json["humanStarted"].toBool();
    activePlayer = json["humanActivePlayer"].toBool() ? humanPlayer : aiPlayer;
    setTrumps(json["trumps"].toInt());
    setWinningThreshold(json["winningThreshold"].toInt());
    isEndgame = json["isEndgame"].toBool();

    isHandOver = false;
    isGameOver = false;
    reset = false;

    //emit readInGame();
    QString gameState = json["gameState"].toString();
    if (gameState == GS_HAND_OVER)
    {
        isHandOver = true;
        emit handOver();
    }
    else if (gameState == GS_GAME_OVER)
    {
        isGameOver = true;
        emit handOver();
    }
    else if (gameState == GS_RESET)
    {
        reset = true;
        emit handOver();
    }
    else if (gameState == GS_ENDGAME)
    {
        emit nextEndTrick();
    }
    else if (gameState == GS_EARLY_GAME)
    {
        emit nextTrick();
    }
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

    json["aiActivePlayer"] = activePlayer->isAi();

    QJsonObject deckObject;
    deck.write(deckObject);
    json["deck"] = deckObject;

    json["meldedSeven"] = meldedSeven;
    json["humanStarted"] = humanStarted;
    json["humanActivePlayer"] = activePlayer == humanPlayer;
    json["trumps"] = trumps;
    json["isEndgame"] = isEndgame;
    json["winningThreshold"] = winningThreshold();

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




























































