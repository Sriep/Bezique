#include "aiendplay.h"




AiEndPlay::AiEndPlay(QList<Card *> max
                     , QList<Card *> min
                     , Card::Suit trumps
                     , Card* lead
                    )
    : ai(max)
    , opp(min)
    , trumps(trumps)
    , lead(lead)
{
    init();
}

int AiEndPlay::operator()()
{
    if (ai.size() == 1)
        return 0;
    Card* card;
    if (NULL == lead)
        card = leadCard(ai);
    else
        card = followCard();
    return findIndex(card);
}

void AiEndPlay::init()
{
    for ( int s=0 ; s < Card::Suit::NumSuits ; s++ )
    {
        for ( int i=0 ; i < ai.length() ; i++ )
            if (ai[i]->getSuit() == s)
                rankSortAppend(aiSuits[s], ai[i]);
        for ( int j=0 ; j < opp.length() ; j++ )
            if (opp[j]->getSuit() == s)
                rankSortAppend(oppSuits[s], opp[j]);
    }
}
//legalFollow(const QList<Card *> hand
//                              , Card *lead
//                              , Card::Suit trumps
//                              , QList<Card*>& legal)
Card *AiEndPlay::followCard()
{
    QList<Card*> legal;// = legalFollow(ai, lead);
    winTrick = legalFollow(ai, lead, trumps, legal);

    if (legal.length() == 1) return legal[0];
    int indexTen = NOT_FOUND;
    int indexAce = NOT_FOUND;
    int indexLowest = NOT_FOUND;
    int lowestRank = NOT_FOUND;
    for ( int i=0 ; i<legal.length() ; i++ )
    {
        if (legal[i]->getRank() == Card::Ten)
            indexTen = i;
        else if (legal[i]->getRank() == Card::Ace)
            indexAce = i;
        else if (indexLowest == NOT_FOUND || legal[i]->getRank() < lowestRank)
        {
            indexLowest = i;
            lowestRank = legal[i]->getRank();
        }
    }
    if (winTrick)
    {
        if (indexTen != NOT_FOUND)
            return legal[indexTen];
        if (indexAce != NOT_FOUND)
        {
            if (oppSuits[legal[0]->getSuit()].length() == 0
                    || indexLowest == NOT_FOUND)
              return legal[indexAce];
        }
        return legal[indexLowest];
    }
    else
    {
        if (lowestRank != NOT_FOUND)
            return legal[indexLowest];
        if (indexTen != NOT_FOUND)
            return legal[indexTen];
        return legal[indexAce];
    }
}
/*
QList<Card *> AiEndPlay::legalFollow(QList<Card *> hand, Card *lead)
{
    int rank = lead->getRank();
    int suit = lead->getSuit();
    QList<Card*> loosingCards;
    QList<Card*> winningCards;

    for ( int i=0 ; i < hand.length() ; i++ )
    {
        if (hand[i]->getSuit() == suit)
        {
            if (hand[i]->getRank() > rank)
                winningCards.append(hand[i]);
            else
                loosingCards.append(hand[i]);
        }
    }
    if (winningCards.length() > 0)
    {
        winTrick = true;
        return winningCards;
    }
    else if (loosingCards.length() > 0)
    {
        winTrick = false;
        return loosingCards;
    }

    if (suit != trumps)
    {
        QList<Card*> trumpCards;
        for ( int i=0 ; i < hand.length() ; i++ )
        {
            if (hand[i]->getSuit() == trumps)
                trumpCards.append(hand[i]);
        }
        if (trumpCards.length() > 0)
        {
            winTrick = true;
            return trumpCards;
        }
    }
    winTrick = false;
    return hand;
}
*/
Card* AiEndPlay::leadCard(QList<Card*> hand)
{
    // Drop singleton aceTen
    Card* dropSingletonAceTen = candropSingeltonAceTen();
    if (dropSingletonAceTen != NULL) return dropSingletonAceTen;

    // Drop ten
    Card* dropTen = canDropTen();
    if (dropTen != NULL) return dropTen;

    // Lead into void ten only against longer trumps
    Card* leadIntoVoid = canleadIntoVoid();
    if (leadIntoVoid != NULL) return leadIntoVoid;

    // Lead longest suit differnece
    Card* bestDelta = leadSuitPositiveDelta();
    if (bestDelta != NULL) return bestDelta;

    // Lead trumps from top
    Card* topTrump = leadTrumps();
    if (topTrump != NULL) return topTrump;

    return hand[0];
}

Card *AiEndPlay::candropSingeltonAceTen()
{
    for ( int i=0 ; i < Card::Suit::NumSuits ; i++ )
    {
        if (aiSuits[i].length() == 1 && oppSuits[i].length() > 0
            && oppSuits[i][0]->getRank() == aiSuits[i][0]->getRank())
        {
            return aiSuits[i][0];
        }

        if (oppSuits[i].length() == 1 && aiSuits[i].length() > 0
            && aiSuits[i][0]->getRank() == oppSuits[i][0]->getRank())
        {
            return aiSuits[i][0];
        }
    }
    return NULL;
}

Card *AiEndPlay::canDropTen()
{
    for ( int i=0 ; i < Card::Suit::NumSuits ; i++ )
    {
        int tenIndex = findRank(oppSuits[i], Card::Rank::Ten);
        int oppLength = oppSuits[i].length();

        if (tenIndex != NOT_FOUND
                && oppLength > 0
                && aiSuits[i].length() >= oppLength)
        {
            if (oppLength == 1)
            {
                if (aiSuits[i][0]->beatsEnd(*oppSuits[i][0], trumps))
                    return aiSuits[i][0];
            }
            else if (aiSuits[i][oppLength-1]->beatsEnd(*oppSuits[i][tenIndex], trumps)
                && aiSuits[i][oppLength-2]->beatsEnd(*oppSuits[i][0], trumps))
            {
                return aiSuits[i][0];
            }
        }
    }
    return NULL;
}

Card *AiEndPlay::canleadIntoVoid()
{
    Card* aceTenVrsVoid = NULL;
    for ( int s=0 ; s < Card::Suit::NumSuits ; s++ )
    {
        if (s != trumps && oppSuits[s].length() == 0 && aiSuits[s].length()>0)
        {
            for ( int r=0 ; r<aiSuits[s].length() ; r++ )
            {
                if (aiSuits[s][r]->getRank() != Card::Rank::Ten
                        && aiSuits[s][r]->getRank() != Card::Rank::Ace )
                    return aiSuits[s][r];
                else
                    aceTenVrsVoid = aiSuits[s][r];
            }
        }
    }
    if (NULL != aceTenVrsVoid && aiSuits[trumps] < oppSuits[trumps])
        return aceTenVrsVoid;
    else
        return NULL;
}

Card *AiEndPlay::leadSuitPositiveDelta()
{
    int maxDelta = -1;
    int maxDeltaSuit = NOT_FOUND;
    bool tenInSuit = false;
    for ( int s=0 ; s < Card::Suit::NumSuits ; s++ )
    {
        if ( s != trumps && aiSuits[s].length() > 0)
        {
            int delta = aiSuits[s].length() - oppSuits[s].length();
            if (delta > maxDelta)
            {
                maxDeltaSuit = s;
                tenInSuit = findRank(aiSuits[s], Card::Rank::Ten) != NOT_FOUND;
                maxDelta = delta;
            }
            else if (maxDeltaSuit != NOT_FOUND
                     && delta == maxDelta && tenInSuit)
            {
                if (findRank(aiSuits[s], Card::Rank::Ten) == NOT_FOUND)
                {
                    maxDeltaSuit = s;
                    tenInSuit = false;
                    maxDelta = delta;
                }
            } //else
        } // if trumps
    } // for

    if (maxDeltaSuit != NOT_FOUND)
    {
        if( aiSuits[maxDeltaSuit].length() == 1
            || aiSuits[maxDeltaSuit][0]->getRank() != Card::Ten )
            return aiSuits[maxDeltaSuit][0];
        else
            return aiSuits[maxDeltaSuit][1];
    }

    return NULL;
}

Card *AiEndPlay::leadTrumps()
{
    if (aiSuits[trumps].length() == 0)
        return NULL;

    if (aiSuits[trumps].length() == 1
            || aiSuits[trumps][0]->getRank() != Card::Ten )
        return aiSuits[trumps][0];
    else
        return aiSuits[trumps][1];
}

int AiEndPlay::findIndex(Card *card)
{
    for ( int i=0 ; i < ai.length() ; i++ )
    {
        if (ai[i]->getRank() == card->getRank()
                && ai[i]->getSuit() == card->getSuit())
            return i;
    }
    return 0;
}






/*
bool aiEndPlay::finiessableTen(Card::Suit suit)
{
    if ( aiSuits[suit].length() == 2 && oppSuits[suit].length() >= 2
       && (aiSuits[suit][0]->getRank() == Card::Ten
           || aiSuits[suit][1]->getRank() == Card::Ten) )
    {
        if (oppSuits[suit][0]->getRank() > Card::Nine && oppSuits[suit][0]->getRank() !== Card::Ten
             && oppSuits[suit][1] < Card::Jack )
                return true;
    }

    if ( oppSuits[suit].length() == 2 && aiSuits[suit].length() >= 2
       && (oppSuits[suit][0]->getRank() == Card::Ten || oppSuits[suit][1]->getRank() == Card::Ten) )
    {
        if (aiSuits[suit][0]->getRank() > Card::Nine && aiSuits[suit][0]->getRank() !== Card::Ten
             && aiSuits[suit][1]->getRank() < Card::Jack )
                return true;
    }
    return false;
}

QList<Card*> aiEndPlay::leadCards(QList<Card *> hand)
{
    QList<Card*> prospects;
    for ( int suit = 0 ; suit < Card::Suit::NumSuits ; suit++ )
    {
        int aceIndex = -1, tenIndex = -1, maxNon10Index = -1;
        int maxSoFar = -1;

        for ( int i ; i < hand.length() ; i++ )
        {
            if (hand[i]->getSuit() == suit)
            {
                if (hand[i]->getRank() == Card::Rank::Ace)
                    aceIndex = i;
                else if ( hand[i]->getRank() == Card::Rank::Ten)
                    tenIndex = i;
                else
                {
                    if (hand[i]->getRank() > maxSoFar)
                    {
                        maxSoFar = hand[i]->getRank();
                        maxNon10Index = i;
                    }
                } // else
            } // if
        } // for i

        if (aceIndex >=0)
            prospects.append(hand[aceIndex]);
        if (maxNon10Index >= 0)
            prospects.append(hand[maxNon10Index]);
        if (aceIndex < 0 && maxNon10Index < 0)
            prospects.append(hand[tenIndex]);

    } // for suit
    return prospects;
}

QList<Card *> aiEndPlay::followCards(QList<Card *> hand, Card *lead)
{
    QList<Card*> prospects;

    const int NOT_FOUND = -1;
    int minWinIndex=NOT_FOUND, minLossIndex=NOT_FOUND;
    int aceIndex=NOT_FOUND, tenIndex=NOT_FOUND;
    int minWinRank = Card::Rank::NumRanks, minLossRank = Card::Rank::NumRanks;
    int sameSuitCount = 0;
    for ( int i=0 ; i < hand.length() ; i++ )
    {
        if (hand[i]->getSuit() == lead->getSuit())
        {
            sameSuitCount++;
            int rank = hand[i]->getRank();
            if (rank == Card::Rank::Ace)
                aceIndex = i;
            else if ( rank == Card::Rank::Ten)
                tenIndex = i;
            else
            {
                if (rank > lead->getRank() && rank < minWinRank)
                {
                    minWinIndex = i;
                    minWinRank = rank;
                }
                else if( rank < lead->getRank() && rank < minLossRank
                         && minWinIndex == NOT_FOUND)
                {
                    minLossIndex = i;
                    minLossRank = rank;
                }
            }

        } // else
    } //for int
    if (sameSuitCount > 0)
    {
        if (tenIndex != NOT_FOUND and lead->getRank() < Card::Rank::Queen)
            prospects.append(hand[tenIndex]);
        else
        {
            if (aceIndex != NOT_FOUND)
                prospects.append(hand[aceIndex]);
            if (minWinIndex != NOT_FOUND)
                prospects.append(hand[minWinIndex]);
            else if (minLossIndex != NOT_FOUND)
                prospects.append(hand[minLossRank]);
        }

        return prospects;
    }


    return prospects;
}

*/





















































