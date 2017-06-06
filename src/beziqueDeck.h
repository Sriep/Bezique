#ifndef DECK_H
#define DECK_H
#include <QList>
#include <QJsonObject>

#include "card.h"

using namespace std;

class BeziqueDeck
{
    const unsigned int beziqueDeckSize = 8*8;
    const unsigned int beziqueHandSize = 8;
public:
    BeziqueDeck(unsigned int size);
    BeziqueDeck();

    void shuffle();
    QList<int> dealHand();
    int peekBottom() const;
    void swapBottom(int id);
    int dealTop();
    bool empty() const;
    int size() const;

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
private:
    void stackDeck();
    QList<int> deck;
};

#endif // DECK_H
