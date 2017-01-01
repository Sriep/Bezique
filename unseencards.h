#ifndef UNSEENCARDS_H
#define UNSEENCARDS_H
#include <QList>
#include <QJsonArray>
#include <QJsonObject>

#include "card.h"

class BeziqueHand;
class UnseenCards
{
public:
    UnseenCards();
    void reset();
    void haveSeen(const Card& card);
    void haveSeen(int cardId);
    void haveSeen(int rank, int suit);
    void haveSeenHand(BeziqueHand* hand);
    int numUnseen(int rank, int suit) const;
    int numUnseenRank(int rank) const;
    int numUnseen() const;
    int unseenThatBeat(Card* card, int trumps) const;

    void Dump() const;
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
private:
    QList<QList<int> > unseenCards;
};

#endif // UNSEENCARDS_H
