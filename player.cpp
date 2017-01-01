#include "player.h"
#include "aievaluate.h"
#include "gamedata.h"
#include "aiendplay.h"
#include "scores.h"

Player::Player(QQuickItem *parent)
    : QQuickItem(parent), score(0)
{
}

void Player::init()
{
}

Player *Player::getOpponent() const
{
    return opponent;
}

void Player::setOpponent(Player *value)
{
    opponent = value;
}

bool Player::cardExists(int index, bool melded)
{
    if (melded)
        return hand->meldedCards[index]->getLink() != Card::EMPTY;
    else
        return hand->hiddedCards[index]->getLink() != Card::EMPTY;
}

UnseenCards Player::getUnseen()
{
    return unseen;
}

void Player::setAi(bool value)
{
    ai = value;
}

bool Player::isAi() const
{
    return ai;
}

void Player::dealtHand(QList<int> dealtHand)
{
    hand->resetCards(dealtHand);
    unseen.haveSeenHand(hand);
}

int Player::semiRandomCard() const
{
/*    int minRank = 15;
    int index = 0;
    for ( int i = 0 ; i < BeziqueHand::HAND_SIZE ; i++ )
    {
        if (hand->cards[i]->getRank() == Card::Rank::Ten)
        {
            index = i;
            break;
        }

        if ( hand->cards[i]->getRank() < minRank)
        {
            index = i;
            minRank = hand->cards[i]->getRank();
        }
    }

    int meldedId = hand->findLinkMelded(index);
    if (meldedId != BeziqueHand::NOT_FOUND)
        return (int) hand->playCard(meldedId, true);

    int hiddenId = hand->findLinkHidden(index);
    if (hiddenId != BeziqueHand::NOT_FOUND)
        return (int) hand->playCard(hiddenId, false);*/
    return 0;
}

void Player::setGameData(GameData *value)
{
    gameData = value;
}

void Player::setCanFollowCards(Card *oppCard, bool isEndgame, int trumps)
{
    hand->setCanFollowCards(oppCard, isEndgame, trumps);
}

void Player::dump()
{
    qDebug() << (ai ? "\nAi" : "\nHuman");
    qDebug() << "score" << score << "\nHand";
    hand->dump();
}

void Player::read(const QJsonObject &json)
{
    score = json["sore"].toInt();
    hand->read(json);
    unseen.read(json);
}

void Player::write(QJsonObject &json) const
{
    json["score"] = score;
    hand->write(json);
    unseen.write(json);
}

Card* Player::playFirstCard(bool isEndgame)
{
    if (!isEndgame)
        return aiPlayCard(true);
    else
        return playFirstCardEndgame();
}

Card* Player::playSecondCard(Card *leadCard, bool isEndgame)
{
    if (!isEndgame)
        return aiPlayCard(false);
    else
        return playSecondCardEndgame(leadCard);
}

Card *Player::aiPlayCard(bool leadCard)
{
    AiEvaluate aiEvaluate(hand
                           , opponent->hand->meldedCardList()
                           , &unseen
                           , gameData
                           , leadCard
                           , score
                         );
    int index = aiEvaluate();
    int hiddenId = hand->findLinkHidden(index);
    if (BeziqueHand::NOT_FOUND != hiddenId)
    {
        return hand->playCard(hiddenId, false);
    }
    else
    {
        int meldedId = hand->findLinkMelded(index);
        return hand->playCard(meldedId, true);
    }
}

Card *Player::playFirstCardEndgame()
{
    AiEndPlay aiEndPlay(hand->hiddedCards
                        , opponent->hand->hiddedCards
                        , (Card::Suit) gameData->getTrumps()
                        , NULL);
    return hand->playCard(aiEndPlay());
}

Card* Player::playSecondCardEndgame(Card *firstCard)
{
    AiEndPlay aiEndPlay(hand->hiddedCards
                        , opponent->hand->hiddedCards
                        , (Card::Suit) gameData->getTrumps()
                        , firstCard);
    return hand->playCard(aiEndPlay());
}

void Player::meldAuto(int trumps, bool seven)
{
    hand->refreshMelds(trumps, seven);
    meldRecursive(trumps, seven);
}

void Player::meldRecursive(int trumps, bool seven)
{
    bool melded = false;
    int index = 0;
    while(!melded && index < BeziqueHand::HAND_SIZE)
    {
        if (hand->cards[index]->getCanMeld())
        {
            meldCard(index, trumps, seven);
            melded = true;
        }
        index++;
    }
    if (melded) meldAuto(trumps, seven);
}

void Player::meldCard(int index, int trumps, bool seven)
{
    if (hand->cards[index]->getRank() == Card::Rank::Seven
            && hand->cards[index]->getSuit() == trumps)
    {
        meldSeven(index);
        seven = true;
        incScore(SCORE_SEVEN);
    }
    else
    {
        incScore(hand->meld(index, opponent));
    }
    hand->refreshMelds(trumps, seven);
}

void Player::meldSeven(int index)
{
    hand->cards[index]->copyCard(*gameData->getFaceCard());
    hand->cards[index]->setLink(index);
    hand->syncIndex(index);
    gameData->meldSeven();
}

void Player::giveCard(int iCard)
{
    hand->addCard(iCard);
}

void Player::incScore(int increment)
{
    score += increment;
    if (increment != 0)
        emit scoreChanged();
}

Card* Player::playCard(int index, bool melded)
{
    return hand->playCard(index, melded);
}

bool Player::canMeld()
{
    QList<Card*>::const_iterator i;
    for ( i = hand->cards.constBegin() ; i != hand->cards.constEnd() ; ++i )
        if ((**i).getCanMeld())
            return true;
    return false;
}

int Player::getScore() const
{
    return score;
}

void Player::setScore(int value)
{
    if (score != value)
        emit scoreChanged();
    score = value;
}

BeziqueHand *Player::getHand() const
{
    return hand;
}

void Player::setHand(BeziqueHand *value)
{
    hand = value;
}

bool Player::handEmpty() const
{
    return hand->isEmpty();
}

bool Player::won() const
{
    return score >= winningThreshold;
}
