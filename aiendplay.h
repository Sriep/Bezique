#ifndef AIENDPLAY_H
#define AIENDPLAY_H
#include <QList>

#include "card.h"
static const int NOT_FOUND = -1;
//static void rankSortAppend(QList<Card*>& list, Card*card);
//static int findRank(QList<Card*>& list, Card::Rank rank);

class AiEndPlay
{
public:
    AiEndPlay(QList<Card*> ai
              , QList<Card*> opp
              , Card::Suit trumps
              , Card* lead);
    int operator()();

private:
    void init();
    Card* followCard();
    Card* leadCard(QList<Card*> hand);
    //QList<Card*> legalFollow(QList<Card*> hand, Card* lead);
    Card* candropSingeltonAceTen();
    Card* canDropTen();
    Card* canleadIntoVoid();
    Card* leadSuitPositiveDelta();
    Card* leadTrumps();
    int findIndex(Card* card);

    //bool finiessableTen(Card::Suit suit);


    QList<Card*> ai;
    QList<Card*> opp;
    Card::Suit trumps;
    Card* lead;

    bool winTrick = false;
    QList<Card*> aiSuits [Card::Suit::NumSuits];
    QList<Card*> oppSuits [Card::Suit::NumSuits];
};

inline static void rankSortAppend(QList<Card*> &list, Card *card)
{
    if (list.length() == 0)
    {
        list.append(card);
        return;
    }
    for ( int i=0 ; i< list.length() ; i++)
    {
        if (card->getRank() > list[i]->getRank())
        {
            list.insert(i,card);
            return;
        }
    }
    list.append(card);
}

inline static int findRank(QList<Card *> &list, Card::Rank rank)
{
    for ( int i=0 ; i < list.length() ; i++ )
    {
        if (list[i]->getRank() == rank)
            return i;
    }
    return NOT_FOUND;
}

inline static bool legalFollow(const QList<Card *>& hand
                               , const Card *lead
                               , Card::Suit trumps
                               , QList<Card*>& legal)
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
        legal = winningCards;
        return true;
    }
    else if (loosingCards.length() > 0)
    {
        legal = loosingCards;
        return false;
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
            legal = trumpCards;
            return true;
        }
    }
    legal = hand;
    return false;
}


































#endif // AIENDPLAY_H
