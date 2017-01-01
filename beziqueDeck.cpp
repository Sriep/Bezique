#include <cstdlib>
#include <QJsonArray>

#include "beziqueDeck.h"

BeziqueDeck::BeziqueDeck()
{
}

void BeziqueDeck::shuffle()
{
    stackDeck();
    const unsigned int deckSize = deck.size();
    for ( unsigned int i = 0 ; i < deckSize - 1 ; i++ )
    {
        int rndIndex = rand() % (deckSize - i) + i ;
        int temp = deck[rndIndex];
        deck[rndIndex] = deck[i];
        deck[i] = temp;
    }
}

QList<int> BeziqueDeck::dealHand()
{
    QList<int> hand;
    for ( unsigned int i = 0 ; i < beziqueHandSize ; i++ )
    {
       // hand.push_back(Card::intToCard(deck.back()));
        hand.push_back(deck.back());
        deck.pop_back();
    }
    return hand;
}

int BeziqueDeck::peekBottom() const
{
    //return Card::intToCard(deck.front());
    return deck.front();
}

void BeziqueDeck::swapBottom(int id)
{
    deck[0] = id;
}

int BeziqueDeck::dealTop()
{
   // Card card = Card::intToCard(deck.back());
    int card = deck.back();
    deck.pop_back();
    return card;
}

bool BeziqueDeck::empty() const
{
    return deck.empty();
}

int BeziqueDeck::size() const
{
    return deck.size();
}

void BeziqueDeck::read(const QJsonObject &json)
{
    deck.clear();
    QJsonArray deckArray = json["deck"].toArray();
    for ( int i=0 ; i<deckArray.size() ; i++ )
    {
        deck.append(deckArray[i].toInt());
    }

}

void BeziqueDeck::write(QJsonObject &json) const
{
    QJsonArray deckArray;
    for( int i=0 ; i<deck.size() ; i++ )
    {
        deckArray.append(deck[i]);
    }
    json["deck"] = deckArray;
}

void BeziqueDeck::stackDeck()
{
    deck.reserve(beziqueDeckSize);
    for ( unsigned int i = 0 ; i < beziqueDeckSize ; i++ )
    {
        deck.push_back(i);
    }
}





























