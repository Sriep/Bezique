#ifndef BEZIQUEHAND_H
#define BEZIQUEHAND_H
#include <QList>
#include <QQuickItem>
#include <QJsonArray>
#include <QJsonObject>

#include "card.h"


//using namespace std;
class Player;
class BeziqueHand;

class BeziqueHand : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Card> cards READ getCards)
    Q_PROPERTY(QQmlListProperty<Card> meldedCards READ getMeldedCards)
    Q_PROPERTY(QQmlListProperty<Card> hiddedCards READ getHiddenCards)
public:
    friend class Player;

    static const int HAND_SIZE = 8;
    static const int CONVERT_FLUSH_INDEX = 3;
    static const int NOT_FOUND = -1;

    BeziqueHand(QQuickItem *parent = 0);
    virtual ~BeziqueHand();

    void resetCards(QList<int> newHand);
    void clearCards(QList<Card*> c);
    bool isEmpty() const;
    void addCard(int cardId);
    const Card *peek(int index);
    Card *playCard(int index, bool melded = false);
    int findLink(int index, bool melded = false);
    void refreshMelds(int trumps, bool seven);
    int meld(int index, Player *opponent);
    void moveAllHidden();
    int scoreMelds(int trumps) const;
    int countTensAces() const;
    int count(Card::Rank rank, Card::Suit suit) const;
    int countRank(Card::Rank rank) const;
    int countMelded() const;
    int indexMelded(int index) const;

    QQmlListProperty<Card> getCards();
    QQmlListProperty<Card> getMeldedCards();
    QQmlListProperty<Card> getHiddenCards();
    const QList<Card*> cardList() const;
    const QList<Card*> meldedCardList() const;

    int getLink(int index, bool meldRow) const;
    void syncIndex(int index);
    void syncHands();
    void setCanFollowCards(Card* oppCard, bool isEndgame, int trumps);

    void dump();
    void dump(const QList<Card *> &h);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
signals:
    void enginPlayedCard(int index);
public slots:
private:
    int findLinkHidden(int link) const;
    int findLinkMelded(int link) const;

    bool canMeld(int index, int trumps, bool hasSeven) const;
    bool canMeldJack(int index, int trumps) const;
    bool canMeldQueen(int index, int trumps) const;
    bool canMeldKing(int index, int trumps) const;
    bool canMeldTen(int index, int trumps) const;
    bool canMeldAce(int index, int trumps) const;

    int findFlush(QList<int>& meld) const;
    int findBezique(QList<int>& meld) const;
    int findDoubleBezique(QList<int>& meld) const;
    int findFourKind(QList<int>& meld) const;
    int findMarrage(QList<int>& meld) const;
    void moveMelded(const QList<int>& meld, Player *player = 0);
    void moveHiddenMelded(int index);
    void moveMeldedHidden(int index);

    static void appendCard(QQmlListProperty<Card> *list, Card *card);
    static void appendMeldedCard(QQmlListProperty<Card> *list, Card *card);
    static void appendHiddenCard(QQmlListProperty<Card> *list, Card *card);
    QList<Card*> cards;
    QList<Card*> hiddedCards;
    QList<Card*> meldedCards;
};

#endif // BEZIQUEHAND_H













































