#include <QtDebug>

#include "scores.h"
#include "aiendplay.h"
#include "beziquehand.h"
#include "player.h"
#include "unseencards.h"

BeziqueHand::BeziqueHand(QQuickItem *parent)
    : QQuickItem(parent)//, isHidden(isHidden)
{
}

BeziqueHand::~BeziqueHand()
{
}

void BeziqueHand::resetCards(QList<int> newHand)
{
    if (newHand.size() > cards.size()) qFatal("Hand length mismatch");
    for (int i = 0; i < newHand.size(); ++i) {
        cards[i]->setCard(newHand[i], i);
        hiddedCards[i]->setCard(newHand[i], i);
    }
}

void BeziqueHand::clearCards(QList<Card *> c)
{
    for ( int i=0 ; i < c.length() ; i++ )
    {
        c[i]->clearCard();
    }
}

bool BeziqueHand::isEmpty() const
{
    for (int i = 0; i < cards.size(); ++i) {
        if (!cards[i]->isCleard())
            return false;
    }
    return true;
}

void BeziqueHand::addCard(int cardId)
{
    int iCard = 0;
    while ( iCard < HAND_SIZE && cards[iCard]->getLink() < HAND_SIZE )
        iCard++;
    cards.at(iCard)->setCard(cardId, iCard);

    int iHide = 0;
    while ( iHide < HAND_SIZE && hiddedCards[iHide]->getLink() < HAND_SIZE )
        iHide++;
    hiddedCards.at(iHide)->setCard(cardId, iCard);
}

const Card *BeziqueHand::peek(int index)
{
    return cards[index];
}

Card* BeziqueHand::playCard(int index, bool melded)
{
    qDebug() << "played card " << index;
    Card* playedCard;
    if (melded)
    {
        QString name = cards[meldedCards[index]->getLink()]->getName();
        qDebug() << name <<" melded";

        playedCard = new Card( meldedCards[index] );
        cards[meldedCards[index]->getLink()]->clearCard();
        meldedCards[index]->clearCard();
    }
    else
    {
        qDebug() << cards[hiddedCards[index]->getLink()]->getName();

        playedCard = new Card( hiddedCards[index]);
        cards[hiddedCards[index]->getLink()]->clearCard();
        hiddedCards[index]->clearCard();
    }

    emit enginPlayedCard(index);
    return playedCard;
}

int BeziqueHand::findLink(int index, bool melded)
{
    QList<Card*>& list = melded ? meldedCards : hiddedCards;
    for ( int i = 0 ; i < list.size() ; i++)
    {
        if (list[i]->getLink() == index)
            return i;
    }
    return -1;
}

void BeziqueHand::refreshMelds(int trumps, bool seven)
{
    for ( int i = 0 ; i < cards.size() ; i++ )
    {
       cards[i]->clearCanMeldStatus();
    }
    for ( int i = 0 ; i < cards.size() ; i++ )
    {
        if (!cards[i]->isCleard())
        {
            bool can = canMeld(i, trumps, seven);
            cards[i]->setCanMeld(can);
            int hiddenLink = findLinkHidden(i);
            if (hiddenLink != NOT_FOUND)
            {
                //hiddedCards[hiddenLink]->setCanMeld(can);
                hiddedCards[hiddenLink]->copyCard(*cards[i]);
                emit hiddedCards[hiddenLink]->cardChanged();
                emit hiddedCards[hiddenLink]->canMeldChanged();
            }
            else
            {
                int meldedLink = findLinkMelded(i);
                if (meldedLink != NOT_FOUND)
                    //meldedCards[meldedLink]->setCanMeld(can);
                    meldedCards[meldedLink]->copyCard(*cards[i]);
                    emit meldedCards[meldedLink]->cardChanged();
                    emit meldedCards[meldedLink]->canMeldChanged();
            } // else
        } // if
    } //for
}

bool BeziqueHand::canMeld(int index, int trumps, bool hasSeven) const
{
    switch (cards[index]->getRank()) {
    case Card::Rank::Seven:
        if (cards[index]->getSuit() == trumps
            &&  cards[index]->getRank() == Card::Rank::Seven
            && !hasSeven)
        {
            cards[index]->setCanSeven(true);
            return true;
        }
        break;
    case Card::Rank::Jack:
        return canMeldJack(index, trumps);
    case Card::Rank::Queen:
        return canMeldQueen(index, trumps);
    case Card::Rank::King:
        return canMeldKing(index, trumps);
    case Card::Rank::Ten:
        return canMeldTen(index, trumps);
    case Card::Rank::Ace:
        return canMeldAce(index, trumps);
    default:
        break;
    }
    return false;
}

bool BeziqueHand::canMeldJack(int index, int trumps) const
{
    int countJacks =1;
    bool canMeld = false;
    bool flush[5] = {true, false, false, false, false};
    int countBeziqueCards = 0;
    bool hasMeldedOneBezique = false;
    bool hasDoubleBeziqued = false;
    bool hasRoyalQueenMarried = false;
    bool hasRoyalKingMarried = false;
    for ( int i = 0 ; i < cards.size() ; i++ )
    {
        if (index != i && cards[i]->getLink() != Card::EMPTY)
        {
            switch (cards[i]->getRank()) {
            case Card::Rank::Jack:
                if (!cards[i]->hasFourKinded)
                    countJacks++;
                if (Card::Suit::Diamonds == cards[i]->getSuit()
                        && Card::Suit::Diamonds == cards[index]->getSuit() )
                {
                     countBeziqueCards++;
                     hasMeldedOneBezique = cards[i]->hasBeziqued;
                     if (cards[i]->hasDoubleBeziqued)
                         hasDoubleBeziqued = true;
                }

                break;
            case Card::Rank::Queen:
                if (Card::Suit::Spades == cards[i]->getSuit()
                        && Card::Suit::Diamonds == cards[index]->getSuit() )
                {                    
                    countBeziqueCards++;
                    if (cards[index]->hasBeziqued)
                        hasMeldedOneBezique = true;
                    if (!cards[i]->hasBeziqued && !cards[index]->hasBeziqued)
                    {
                        //cards[index]->canBezique = true;
                        cards[index]->setCanBezique(true);
                        canMeld = true;
                    }
                    if (cards[i]->hasDoubleBeziqued)
                        hasDoubleBeziqued = true;
                }
                if (cards[i]->getSuit() == trumps)
                {
                    if (!cards[i]->canMarry)
                        flush[1] = true;
                    else if (cards[i]->hasMarried)
                        hasRoyalQueenMarried = true;
                }
            case Card::Rank::King:
                if (cards[i]->getSuit() == trumps && cards[i]->hasMarried)
                        hasRoyalKingMarried = true;
            case Card::Rank::Ten:
            case Card::Rank::Ace:
                if (cards[i]->getSuit() == trumps
                        && false == cards[i]->canMarry)
                    flush[cards[i]->getRank() - CONVERT_FLUSH_INDEX] = true;
            default:
                break;
            }
        }
    }
    if (4 == countBeziqueCards
         && hasMeldedOneBezique
         && hasDoubleBeziqued
         && !cards[index]->hasBeziqued
         && !cards[index]->hasDoubleBeziqued)
    {
        cards[index]->canDoubleBezique = true;
        cards[index]->canBezique = false;
        canMeld = true;
    }

    if (countJacks >= 4 && !cards[index]->hasFourKinded)
    {
        cards[index]->setCanFourKind(true);
        canMeld = true;
    }
    if (flush[0] && flush[1] && flush[2] && flush[3] && flush[4]
            && !cards[index]->hasFlushed
            && cards[index]->getSuit() == trumps
            && hasRoyalQueenMarried && hasRoyalKingMarried)
    {
        cards[index]->setCanFlush(true);
        canMeld = true;
    }

    return canMeld;
}

bool BeziqueHand::canMeldQueen(int index, int trumps) const
{
    int countQueens =1;
    bool canMeld = false;
    bool flush[5] = {false, true, false, false, false};
    int countBeziqueCards = 0;
    bool hasMeldedOneBezique = false;
    bool hasDoubleBeziqued = false;
    bool hasRoyalKingMarried = false;
    bool hasRoyalQueenMarried = cards[index]->getSuit() == trumps
                                && cards[index]->hasMarried;
    for ( int i = 0 ; i < cards.size() ; i++ )
    {
        if (index != i && cards[i]->getLink() != Card::EMPTY)
        {
            switch (cards[i]->getRank()) {
            case Card::Rank::Jack:
                if (Card::Suit::Diamonds == cards[i]->getSuit()
                        && Card::Suit::Spades == cards[index]->getSuit())
                {
                    countBeziqueCards++;
                    if (cards[index]->hasBeziqued)
                        hasMeldedOneBezique = true;
                    if (!cards[i]->hasBeziqued && !cards[index]->hasBeziqued)
                    {
                        cards[index]->canBezique = true;
                        canMeld = true;
                    }
                    if (cards[i]->hasDoubleBeziqued)
                        hasDoubleBeziqued = true;
                }
                if (!cards[i]->hasFlushed)
                    flush[1] = true;
                break;
            case Card::Rank::Queen:
                if (false == cards[i]->hasFourKinded)
                    countQueens++;
                if (Card::Suit::Spades == cards[i]->getSuit()
                        && Card::Suit::Spades == cards[index]->getSuit() )
                {
                     countBeziqueCards++;
                     hasMeldedOneBezique = cards[i]->hasBeziqued;
                     if (cards[i]->hasDoubleBeziqued)
                         hasDoubleBeziqued = true;
                }
                break;
            case Card::Rank::King:
                if (cards[i]->getSuit() == cards[index]->getSuit()
                        && !cards[i]->hasMarried
                        && !cards[index]->hasMarried)
                {
                    cards[index]->canMarry = true;
                    canMeld = true;
                }
                if (cards[i]->getSuit() == trumps && cards[i]->hasMarried)
                        hasRoyalKingMarried = true;
            case Card::Rank::Ten:
            case Card::Rank::Ace:
                if (cards[i]->getSuit() == trumps
                        && !cards[i]->hasFlushed)
                    flush[cards[i]->getRank() - CONVERT_FLUSH_INDEX] = true;
            default:
                break;
            }
        }
    }

    if (4 == countBeziqueCards
         && hasMeldedOneBezique
         && hasDoubleBeziqued
         && !cards[index]->hasBeziqued
         && !cards[index]->hasDoubleBeziqued)
    {
        cards[index]->canDoubleBezique = true;
        cards[index]->canBezique = false;
        canMeld = true;
    }

    if (countQueens >= 4 && !cards[index]->hasFourKinded)
    {
        cards[index]->canFourKind = true;
        canMeld = true;
    }
    if (flush[0] && flush[1] && flush[2] && flush[3] && flush[4]
            && !cards[index]->hasFlushed
            && cards[index]->getSuit() == trumps
            && hasRoyalQueenMarried && hasRoyalKingMarried)
    {
        cards[index]->canFlush = true;
        canMeld = true;
    }
    return canMeld;
}

bool BeziqueHand::canMeldKing(int index, int trumps) const
{
    int countKings =1;
    bool canMeld = false;
    bool flush[5] = {false, false, true, false, false};
    bool hasRoyalQueenMarried = false;
    bool hasRoyalKingMarried = cards[index]->getSuit() == trumps
                                && cards[index]->hasMarried;
    for ( int i = 0 ; i < cards.size() ; i++ )
    {
        if (index != i && cards[i]->getLink() != Card::EMPTY)
        {
            switch (cards[i]->getRank()) {
            case Card::Rank::King:
                if (!cards[i]->hasFourKinded)
                    countKings++;
                break;
            case Card::Rank::Queen:
                if (cards[i]->getSuit() == cards[index]->getSuit()
                        && !cards[i]->hasMarried
                        && !cards[index]->hasMarried)
                {
                    cards[index]->canMarry = true;
                    canMeld = true;
                }
                if (cards[i]->getSuit() == trumps && cards[i]->hasMarried)
                        hasRoyalQueenMarried = true;
            case Card::Rank::Jack:
            case Card::Rank::Ten:
            case Card::Rank::Ace:
                if (cards[i]->getSuit() == trumps
                        && !cards[i]->hasFlushed)
                    flush[cards[i]->getRank() - CONVERT_FLUSH_INDEX] = true;
            default:
                break;
            }
        }
    }
    if (countKings >= 4 && !cards[index]->hasFourKinded)
    {
        cards[index]->canFourKind = true;
        canMeld = true;
    }
    if (flush[0] && flush[1] && flush[2] && flush[3] && flush[4]
            && !cards[index]->hasFlushed
            && cards[index]->getSuit() == trumps
            && hasRoyalQueenMarried && hasRoyalKingMarried)
    {
        cards[index]->canFlush = true;
        canMeld = true;
    }
    return canMeld;
}

bool BeziqueHand::canMeldTen(int index, int trumps) const
{

    if (cards[index]->hasFlushed) return false;
    bool canMeld = false;
    bool flush[5] = {false, false, false, true, false};
    bool hasRoyalQueenMarried = false;
    bool hasRoyalKingMarried = false;
    for ( int i = 0 ; i < cards.size() ; i++ )
    {
        if (index != i && cards[i]->getLink() != Card::EMPTY)
        {
            switch (cards[i]->getRank()) {
            case Card::Rank::Queen:
                if (cards[i]->getSuit() == trumps && cards[i]->hasMarried)
                        hasRoyalQueenMarried = true;
                if (cards[i]->getSuit() == trumps
                        && !cards[i]->hasFlushed)
                    flush[cards[i]->getRank() - CONVERT_FLUSH_INDEX] = true;
                break;
            case Card::Rank::King:
                if (cards[i]->getSuit() == trumps && cards[i]->hasMarried)
                        hasRoyalKingMarried = true;
            case Card::Rank::Jack:
            case Card::Rank::Ten:
            case Card::Rank::Ace:
                if (cards[i]->getSuit() == trumps
                        && !cards[i]->hasFlushed)
                    flush[cards[i]->getRank() - CONVERT_FLUSH_INDEX] = true;
            default:
                break;
            }
        }
    }
    if (flush[0] && flush[1] && flush[2] && flush[3] && flush[4]
            && !cards[index]->hasFlushed
            && cards[index]->getSuit() == trumps
            && hasRoyalQueenMarried && hasRoyalKingMarried)
    {
        cards[index]->canFlush = true;
        canMeld = true;
    }
    return canMeld;
}

bool BeziqueHand::canMeldAce(int index, int trumps) const
{
    int countAces =1;
    bool canMeld = false;
    bool flush[5] = {false, false, false, false, true};
    bool hasRoyalQueenMarried = false;
    bool hasRoyalKingMarried = false;
    for ( int i = 0 ; i < cards.size() ; i++ )
    {
        if (index != i && cards[i]->getLink() != Card::EMPTY)
        {
            switch (cards[i]->getRank()) {
            case Card::Rank::Ace:
                if (!cards[i]->hasFourKinded)
                    countAces++;
                break;
            case Card::Rank::Queen:
                if (cards[i]->getSuit() == trumps && cards[i]->hasMarried)
                        hasRoyalQueenMarried = true;
                if (cards[i]->getSuit() == trumps
                        && !cards[i]->hasFlushed)
                    flush[cards[i]->getRank() - CONVERT_FLUSH_INDEX] = true;
                break;
            case Card::Rank::King:
                if (cards[i]->getSuit() == trumps && cards[i]->hasMarried)
                        hasRoyalKingMarried = true;
            case Card::Rank::Jack:
            case Card::Rank::Ten:
                if (cards[i]->getSuit() == trumps
                        && !cards[i]->hasFlushed)
                    flush[cards[i]->getRank() - CONVERT_FLUSH_INDEX] = true;
            default:
                break;
            }
        }
    }
    if (countAces >= 4 && !cards[index]->hasFourKinded)
    {
        cards[index]->canFourKind = true;
        canMeld = true;
    }
    if (flush[0] && flush[1] && flush[2] && flush[3] && flush[4]
            && !cards[index]->hasFlushed
            && cards[index]->getSuit() == trumps
            && hasRoyalQueenMarried && hasRoyalKingMarried)
    {
        cards[index]->canFlush = true;
        canMeld = true;
    }
    return canMeld;
}

int BeziqueHand::meld(int index, Player* opponent)
{
    if (!cards[index]->canMeld) qWarning("melding not possable");

    QList<int> meld;
    meld.append(index);
    int score = 0;
    if (cards[index]->canSeven) score = SCORE_SEVEN;
    else if (cards[index]->canFlush) score = findFlush(meld);
    else if (cards[index]->canBezique) score = findBezique(meld);
    else if (cards[index]->canBezique) score = findDoubleBezique(meld);
    else if (cards[index]->canFourKind) score = findFourKind(meld);
    else if (cards[index]->canMarry) score = findMarrage(meld);

    moveMelded(meld, opponent);
    return score;
}

int BeziqueHand::findFlush(QList<int> &meld) const
{
    bool flush[5] = {false, false, false, false, false};
    flush[meld.first() - CONVERT_FLUSH_INDEX] = true;
    int trumps = cards[meld.first()]->getSuit();

    for (int i = 0 ; i < HAND_SIZE ; i++)
    {
        if (cards[i]->getSuit() == trumps)
        {
            if (cards[i]->getRank() >= Card::Rank::Jack
                   && !flush[cards[i]->getRank() - CONVERT_FLUSH_INDEX]
                   && cards[i]->canFlush)
            {
                flush[cards[i]->getRank() - CONVERT_FLUSH_INDEX] = true;
                meld.append(i);
                cards[i]->setHasFlushed(true);
            }
        }
    }
    cards[meld.first()]->setHasFlushed(true);
    return SCORE_FLUSH;
}

int BeziqueHand::findBezique(QList<int> &meld) const
{
    cards[meld.first()]->setHasBeziqued(true);
    int findRank = cards[meld.first()]->getRank() == Card::Rank::Jack ?
                Card::Rank::Queen : Card::Rank::Jack;
    int findSuit = cards[meld.first()]->getSuit() == Card::Suit::Spades ?
                Card::Suit::Diamonds : Card::Suit::Spades;
    for (int i = 0 ; i < HAND_SIZE ; i++ )
    {
        if (cards[i]->getRank() == findRank
                && cards[i]->getSuit() == findSuit
                && cards[i]->canBezique)
        {
            meld.append(i);
            cards[i]->setHasBeziqued(true);
            return SCORE_BEZIQUE;
        }
    }
    qWarning() << "End of BeziqueHand::findBezique, no bezique found.";
    return 0;
}

int BeziqueHand::findDoubleBezique(QList<int> &meld) const
{
    cards[meld.first()]->setHasDoubleBeziqued(true);
    for ( int i=0 ; i<cards.length() ; i++ )
    {
        if (cards[i]->canDoubleBezique)
        {
            meld.append(i);
            cards[i]->setHasDoubleBeziqued(true);
        }
    }
    return SCORE_DOUBLE_BEZIQUE;
}

int BeziqueHand::findFourKind(QList<int> &meld) const
{
    //cards[meld.first()]->hasFourKinded = true;
    cards[meld.first()]->setHasFourKinded(true);
    int rankMatches = 1;
    int rank = cards[meld.first()]->getRank();
    int i = 0;
    while ( i < HAND_SIZE && rankMatches < 4 )
    {
        if (cards[i]->getRank() == rank
                && i != meld.first()
                && cards[i]->canFourKind)
        {
            meld.append(i);
            //cards[i]->hasFourKinded = true;
            cards[i]->setHasFourKinded(true);
            rankMatches++;
        }
        i++;
    }
    switch (rank) {
    case Card::Rank::Jack:
        return SCORE_FOUR_JACKS;
    case Card::Rank::Queen:
        return SCORE_FOUR_QUEENS;
    case Card::Rank::King:
        return SCORE_FOUR_KINGS;
    case Card::Rank::Ace:
        return SCORE_FOUR_ACES;
    default:
        break;
    }
    qWarning() << "End of BeziqueHand::findFourKind, no findFourKind found.";
    return 0;
}

int BeziqueHand::findMarrage(QList<int> &meld) const
{
    cards[meld.first()]->setHasMarried(true);
    int findRank = cards[meld.first()]->getRank() == Card::Rank::Queen ?
                Card::Rank::King : Card::Rank::Queen;
    int findSuit = cards[meld.first()]->getSuit();
    for ( int i = 0 ; i < HAND_SIZE ; i++ )
    {
        if (cards[i]->getRank() == findRank
                && cards[i]->getSuit() == findSuit
                && cards[i]->canMarry)
        {
            meld.append(i);
            cards[i]->setHasMarried(true);
            return SCORE_MARRAGE;
        }
    }
    qWarning() << "End of BeziqueHand::findMarrage, no Marrage found.";
    return 0;
}

void BeziqueHand::moveMelded(const QList<int> &meld, Player* player)
{
   // QList<int>::const_iterator i;
    for ( QList<int>::const_iterator  i = meld.begin(); i != meld.end(); ++i )
    {
        int hiddenId = findLinkHidden(*i) ;
        if (hiddenId != NOT_FOUND)
        {
            moveHiddenMelded(hiddenId);
            if (player)
            {
                player->getUnseen().haveSeen(*cards[*i]);
            }
        }
    }
}

void BeziqueHand::moveHiddenMelded(int index)
{
    int iMerge = 0;
    while ( meldedCards[iMerge]->getLink() < HAND_SIZE
            && iMerge < HAND_SIZE )
        iMerge++;
    meldedCards[iMerge]->copyCard(*hiddedCards[index]);
    hiddedCards[index]->clearCard();
}

void BeziqueHand::moveMeldedHidden(int index)
{
    int iHide = 0;
    while ( hiddedCards[iHide]->getLink() < HAND_SIZE
            && iHide < HAND_SIZE )
        iHide++;
    hiddedCards[iHide]->copyCard(*hiddedCards[index]);
    meldedCards[index]->clearCard();
}

void BeziqueHand::moveAllHidden()
{
    for ( int i = 0 ; i < cards.size() ; i++ )
    {
        hiddedCards[i]->copyCard(*cards[i]);
    }
    for ( int i = 0 ; i < meldedCards.size() ; i++ )
    {
        meldedCards[i]->clearCard();
    }
}

int BeziqueHand::scoreMelds(int trumps) const
{
    int score = 0;
    bool bezique = false;
    bool flush = false;
    bool doubleBezique = false;
    int marrage[Card::Suit::NumSuits] = {false, false, false, false};
    bool fourKind[5] = {false, false, false, false, false};

    for ( int i=0 ; i<cards.length() ; i++ )
    {
        if (cards[i]->canBezique) bezique = true;
        if (cards[i]->canFlush) flush = true;
        if (cards[i]->canDoubleBezique) doubleBezique = true;
        if (cards[i]->canMarry) marrage[cards[i]->getSuit()] = true;
        if (cards[i]->canFourKind)
            fourKind[cards[i]->getRank() - CONVERT_FLUSH_INDEX] = true;
    }
    score += bezique ? SCORE_BEZIQUE : 0;
    score += flush ? SCORE_FLUSH : 0;
    score += doubleBezique ? SCORE_DOUBLE_BEZIQUE : 0;
    for ( int i=0 ; i < Card::Suit::NumSuits ; i++ )
        if ( marrage[i])
            score += i == trumps ? SCORE_ROYAL_MARRAGE : SCORE_MARRAGE;
    for ( int j=0 ; j < 5 ; j++ )
    {
        if (fourKind[j])
        {
            switch (j) {
            case 0:
                score += SCORE_FOUR_JACKS;
                break;
            case 1:
                score += SCORE_FOUR_QUEENS;
                break;
            case 2:
                score += SCORE_FOUR_KINGS;
                break;
            case 4:
                score += SCORE_FOUR_ACES;
                break;
            default:
                break;
            }
        }
    }

    return score;
}

int BeziqueHand::countTensAces() const
{
    int count = 0;
    for ( int i=0 ; i<cards.size() ; i++ )
    {
        if ( cards[i]->getRank() == Card::Rank::Ten
             || cards[i]->getRank() == Card::Rank::Ace)
            count++;
    }
    return count;
}

int BeziqueHand::count(Card::Rank rank, Card::Suit suit) const
{
    int count = 0;
    for ( int i=0 ; i<cards.size() ; i++ )
    {
        if ( cards[i]->getRank() == rank
             && cards[i]->getSuit() == suit)
            count++;
    }
    return count;
}

int BeziqueHand::countRank(Card::Rank rank) const
{
    int count = 0;
    for ( int i=0 ; i<cards.size() ; i++ )
    {
        if ( cards[i]->getRank() == rank )
            count++;
    }
    return count;
}

int BeziqueHand::countMelded() const
{
    int count = 0;
    for ( int i=0 ; i< meldedCards.size() ; i++ )
    {
        if ( indexMelded(i) )
            count++;
    }
    return count;
}

int BeziqueHand::indexMelded(int index) const
{
    return meldedCards[index]->link == NOT_FOUND;
}

const QList<Card *> BeziqueHand::meldedCardList() const
{
    QList<Card *> melds;
    for ( int i=0 ; i< meldedCards.size() ; i++ )
    {
        if ( indexMelded(i) )
            melds.append(meldedCards[i]);
    }
    return melds;
}

int BeziqueHand::getLink(int index, bool meldRow) const
{
    if (meldRow)
        return meldedCards[index]->getLink();
    else
        return hiddedCards[index]->getLink();
}

void BeziqueHand::syncIndex(int index)
{
    int hiddenIndex = findLinkHidden(index);
    if (hiddenIndex != NOT_FOUND)
        hiddedCards[hiddenIndex]->copyCard(*cards[index]);
    else
    {
        int meldedIndex = findLinkMelded(index);
        if (meldedIndex != NOT_FOUND)
            meldedCards[meldedIndex]->copyCard(*cards[index]);
    }
}

void BeziqueHand::syncHands()
{
    QList<int> meldLinks;
    for ( int i=0 ; i<meldedCards.length() ; i++ )
    {
        if (meldedCards[i]->getLink() != NOT_FOUND)
        {
            meldLinks << meldedCards[i]->getLink();
            //int hiddenId = findLinkHidden(meldedCards[i]->getLink());
            //if (hiddenId != NOT_FOUND)
            //    hiddedCards[hiddenId]->clearCard();
        }
    }
    QList<Card*> splitSuits [Card::Suit::NumSuits];
    for ( int s=0 ; s < Card::Suit::NumSuits ; s++ )
    {
        for ( int i=0 ; i < cards.length() ; i++ )
        {
            if (cards[i]->getSuit() == s
                 && cards[i]->getLink() != NOT_FOUND)
               rankSortAppend(splitSuits[s], cards[i]);
        }
    }
    int hiddenId = 0;
    for ( int s=0 ; s < Card::Suit::NumSuits ; s++ )
    {
        for ( int i=0 ; i < splitSuits[s].length() ; i++ )
        {
            if (meldLinks.indexOf(splitSuits[s][i]->getLink()) == -1)
                hiddedCards[hiddenId++]->copyCard(*splitSuits[s][i]);
        }
    }
    for ( int j=hiddenId ; j < hiddedCards.length() ; j++ )
    {
        hiddedCards[j]->clearCard();
    }

}

void BeziqueHand::setCanFollowCards(Card *oppCard, bool isEndgame, int trumps)
{
    for ( int i = 0 ; i < cards.size() ; i++ )
    {
        cards[i]->setCanPlay(true);
    }
    for ( int i = 0 ; i < meldedCards.size() ; i++ )
    {
        if (meldedCards[i]->isCleard())
            meldedCards[i]->setCanPlay(false);
        else
            meldedCards[i]->setCanPlay(true);
    }
    for ( int i = 0 ; i < hiddedCards.size() ; i++ )
    {
        if (hiddedCards[i]->isCleard())
            hiddedCards[i]->setCanPlay(false);
        else
            hiddedCards[i]->setCanPlay(true);
    }


    if (isEndgame)
    {
        QList<Card*> legal;
        legalFollow(cards, oppCard, (Card::Suit) trumps, legal);

        for ( int i = 0 ; i < cards.size() ; i++ )
        {
            if (legal.indexOf(cards[i]) == -1)
            {
                cards[i]->setCanPlay(false);
                emit cards[i]->canPlayChanged();
                int hiddenLink = findLinkHidden(i);
                if (hiddenLink != NOT_FOUND)
                {
                    hiddedCards[hiddenLink]->setCanPlay(false);
                    emit hiddedCards[hiddenLink]->canPlayChanged();
                }
                else
                {
                    int meldedLink = findLinkMelded(i);
                    if (meldedLink != NOT_FOUND)
                    {
                        meldedCards[meldedLink]->setCanPlay(false);
                        emit meldedCards[meldedLink]->canPlayChanged();
                    }
                }
            }
        }
    }
}

void BeziqueHand::dump()
{
    qDebug() << "Cards";
    dump(cards);
    qDebug() << "Hidden Cards";
    dump(hiddedCards);
    qDebug() << "Melded Cards";
    dump(meldedCards);
}

void BeziqueHand::dump(const QList<Card*> &h)
{
    for ( int i=0 ; i<h.length() ; i++ )
    {
        h[i]->dump();
    }
}

void BeziqueHand::read(const QJsonObject &json)
{
    clearCards(cards);
    QJsonArray cardsArray = json["cards"].toArray();
    for (int i = 0; i < cardsArray.size(); ++i) {
        QJsonObject cardObject = cardsArray[i].toObject();
        cards[i]->read(cardObject);
    }

    clearCards(hiddedCards);
    QJsonArray hiddedCardsArray = json["hiddedCards"].toArray();
    for (int i = 0; i < hiddedCardsArray.size(); ++i) {
        QJsonObject cardObject = hiddedCardsArray[i].toObject();
        hiddedCards[i]->read(cardObject);
    }

    clearCards(meldedCards);
    QJsonArray meldedCardsArray = json["meldedCards"].toArray();
    for (int i = 0; i < meldedCardsArray.size(); ++i) {
        QJsonObject cardObject = meldedCardsArray[i].toObject();
        meldedCards[i]->read(cardObject);
    }
}

void BeziqueHand::write(QJsonObject &json) const
{
    QJsonArray cardsArray;
    foreach (const Card* card, cards)
    {
        QJsonObject cardObject;
        card->write(cardObject);
        cardsArray.append(cardObject);
    }
    json["cards"] = cardsArray;

    QJsonArray hiddedCardsArray;
    foreach (const Card* card, hiddedCards)
    {
        QJsonObject cardObject;
        card->write(cardObject);
        hiddedCardsArray.append(cardObject);
    }
    json["hiddedCards"] = hiddedCardsArray;

    QJsonArray meldedCardsArray;
    foreach (const Card* card, meldedCards)
    {
        QJsonObject cardObject;
        card->write(cardObject);
        meldedCardsArray.append(cardObject);
    }
    json["meldedCards"] = meldedCardsArray;

}

QQmlListProperty<Card> BeziqueHand::getCards()
{
    return QQmlListProperty<Card>(this, 0, &BeziqueHand::appendCard, 0, 0, 0);
}

QQmlListProperty<Card> BeziqueHand::getMeldedCards()
{
    return QQmlListProperty<Card>(this, 0, &BeziqueHand::appendMeldedCard, 0, 0, 0);
}

QQmlListProperty<Card> BeziqueHand::getHiddenCards()
{
    return QQmlListProperty<Card>(this, 0, &BeziqueHand::appendHiddenCard, 0, 0, 0);
}

const QList<Card*> BeziqueHand::cardList() const
{
    return cards;
}

int BeziqueHand::findLinkHidden(int link) const
{
    if (link == NOT_FOUND)
        return NOT_FOUND;
    for ( int i = 0 ; i < HAND_SIZE ; i++ )
    {
        if (hiddedCards[i]->getLink() == link)
            return i;
    }
    return NOT_FOUND;
}

int BeziqueHand::findLinkMelded(int link) const
{
    if (link == NOT_FOUND)
        return NOT_FOUND;
    for ( int i = 0 ; i < HAND_SIZE ; i++ )
    {
        if (meldedCards[i]->getLink() == link)
            return i;
    }
    return NOT_FOUND;
}

void BeziqueHand::appendCard(QQmlListProperty<Card> *list, Card *card)
{
    BeziqueHand *hand = qobject_cast<BeziqueHand*>(list->object);
    if (hand) {
        card->setParentItem(hand);
        hand->cards.append(card);
    }
}

void BeziqueHand::appendMeldedCard(QQmlListProperty<Card> *list, Card *card)
{
    BeziqueHand *hand = qobject_cast<BeziqueHand*>(list->object);
    if (hand) {
        card->setParentItem(hand);
        hand->meldedCards.append(card);
    }
}

void BeziqueHand::appendHiddenCard(QQmlListProperty<Card> *list, Card *card)
{
    BeziqueHand *hand = qobject_cast<BeziqueHand*>(list->object);
    if (hand) {
        card->setParentItem(hand);
        hand->hiddedCards.append(card);
    }
}
























































