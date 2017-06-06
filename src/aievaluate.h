#ifndef AIEVALUATE_H
#define AIEVALUATE_H
#include <QList>
#include "card.h"

class BeziqueHand;
class UnseenCards;
class GameData;
class BeziqueDeck;

class AiEvaluate
{
public:
    AiEvaluate(BeziqueHand* hand
               , QList<Card*> opponentMelds
               , UnseenCards* unseen
               , GameData* gameData
               , bool leadCard
               , int score = 0);

    int operator()() const;

public:
    static const int COST_LOOSING_TEN = 6;
    static const int COST_LOOSING_ACE = 10;
    static const int VALUE_OF_A_TURMP = 1;

private:
    float evaluate(Card* card) const;
    float evaluateSeven() const;
    float evaluateEight() const;
    float evaluateNine() const;
    float evaluateTen(Card *card) const;
    float evaluateJack(Card *card) const;
    float evaluateQueen(Card *card) const;
    float evaluateKing(Card *card) const;
    float evaluateAce(Card *card) const;

    float costOfLead() const;
    float probOfFlush() const;
    float probOfFourKind(Card::Rank rank) const;
    float probOfBezique() const;
    float probOfDoubleBezique() const;
    float probOfMarrage(Card::Suit suit) const;
    float probWinTrick(Card* card) const;

    float probDealtCard(Card::Rank rank, Card::Suit suit) const;
    float probDealt(float numLeft, float tricks, float unseenCount) const;
    float probDealtN(int n, float numLeft) const;

    BeziqueHand* hand;
    QList<Card*> opponentMelds;
    QList<Card*> aiCards;
    UnseenCards* unseen;
    float unseenCount;
    Card::Suit trumps;
    bool sevenPlayed;
    Card* opponentLead;
    Card* faceCard;
    BeziqueDeck* deck;
    float tricksLeft;
    bool leadCard;
    int score;
};

#endif // AIEVALUATE_H
