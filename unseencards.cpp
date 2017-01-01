#include <QtDebug>

#include "unseencards.h"
#include "beziquehand.h"
#include "card.h"

UnseenCards::UnseenCards()
{
    reset();
}

void UnseenCards::reset()
{
    unseenCards.clear();
    for ( int i = 0 ; i < Card::NumRanks ; i++ )
    {
        QList<int> rank;
        for ( int j = 0 ; j < Card::NumSuits ; j++ )
        {
            rank.clear();
            rank << 2 << 2 << 2 << 2;
        }
        unseenCards.append(rank);
    }
    //Dump();
}

void UnseenCards::haveSeen(const Card &card)
{
    unseenCards[card.getRank()][card.getSuit()]--;
    //Dump();
}

void UnseenCards::haveSeen(int cardId)
{
        unseenCards[cardId / 8][cardId % 4]--;
        //Dump();
}

void UnseenCards::haveSeen(int rank, int suit)
{
    unseenCards[rank][suit]--;
    //Dump();
}

void UnseenCards::haveSeenHand(BeziqueHand *hand)
{
    for ( int i = 0 ; i < BeziqueHand::HAND_SIZE ; i++ )
    {
        haveSeen(*(hand->cardList().at(i)));
    }
    //Dump();
}

int UnseenCards::numUnseen(int rank, int suit) const
{
    //Dump();
    int count = unseenCards[rank][suit];
    return count;
}

int UnseenCards::numUnseenRank(int rank) const
{
    //Dump();
    int count = 0;
    for ( int j = 0 ; j < Card::NumSuits ; j++ )
        count += unseenCards[rank][j];
    return count;
}

int UnseenCards::numUnseen() const
{
    //Dump();
    int count;
    for ( int i = 0 ; i < Card::NumRanks ; i++ )
        for ( int j = 0 ; j < Card::NumSuits ; j++ )
            count += unseenCards[i][j];
    return count;
}

int UnseenCards::unseenThatBeat(Card *card, int trumps) const
{
    int count  = 0;
    for ( int i = card->getRank() + 1 ; i < Card::Rank::NumRanks ; i++ )
    {
        count += unseenCards[i][card->getSuit()];
    }
    if ( card->getSuit() != trumps )
    {
        for ( int i = 0 ; i < Card::Rank::NumRanks ; i++ )
        {
            count += unseenCards[i][trumps];
        }
    }
    return count;
}

void UnseenCards::Dump() const
{
    qDebug() << "Unseen cards\n";
    for ( int i = 0 ; i < unseenCards.size() ; i++ )
    {

        qDebug() << i << "Diamands" << unseenCards[i][0] << " | "
                << i << "Clubs" << unseenCards[i][1] << " | "
                << i << "Hearts" << unseenCards[i][2] << " | "
                << i << "Spades" << unseenCards[i][3];
    }
    qDebug() << "Finished Unseen cards\n";
}

void UnseenCards::read(const QJsonObject &json)
{
    unseenCards.clear();

    QJsonArray unseenArray = json["unseenCards"].toArray();
    for (int i = 0; i < unseenArray.size(); ++i) {
        QJsonObject rankObject = unseenArray[i].toObject();
        QList<int> unknownRank;
        unknownRank.append(rankObject["diamonds"].toInt());
        unknownRank.append(rankObject["clubs"].toInt());
        unknownRank.append(rankObject["hearts"].toInt());
        unknownRank.append(rankObject["spades"].toInt());
        unseenCards.append(unknownRank);
    }
}

void UnseenCards::write(QJsonObject &json) const
{
    QJsonArray unseenArray;
    for ( int i=0 ; i < unseenCards.length() ; i++ )
    {
        QJsonObject rankObject;
        rankObject["diamonds"] = unseenCards[i][0];
        rankObject["clubs"] = unseenCards[i][0];
        rankObject["hearts"] = unseenCards[i][0];
        rankObject["spades"] = unseenCards[i][0];

        unseenArray.append(rankObject);
    }
    json["unseenCards"] = unseenArray;

}


























