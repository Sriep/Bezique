#include <QDebug>
#include <limits>
#include <math.h>
#include <cmath>

#include "scores.h"
#include "aievaluate.h"
#include "beziquehand.h"
#include "unseencards.h"
#include "gamedata.h"

AiEvaluate::AiEvaluate(BeziqueHand *hand
                       , QList<Card *> opponentMelds
                       , UnseenCards *unseen
                       , GameData* gameData
                       , bool leadCard
                       , int score)
    :   hand(hand)
        , opponentMelds(opponentMelds)
        , aiCards(hand->cardList())
        , unseen(unseen)
        , unseenCount((float) max(1, unseen->numUnseen()))
        , trumps((Card::Suit) gameData->getTrumps())
        , sevenPlayed(gameData->getMeldedSeven())
        , opponentLead(gameData->getHumansCard())
        , faceCard(gameData->getFaceCard())
        , deck(gameData->getDeck())
        , tricksLeft((float) deck->size() / 2)
        , leadCard(leadCard)
        , score(score)
{
    //deck = gameData->getDeck();
}

int AiEvaluate::operator()() const
{
    float lowestValue = std::numeric_limits<float>::max();
    int indexLowest = 0;
    for ( int i = 0 ; i < aiCards.length() ; i++ )
    {
        float value = evaluate(aiCards[i]);
        qDebug() << aiCards[i]->getName() << " score: " << value;
        if (value < lowestValue)
        {
            indexLowest = i;
            lowestValue = value;
        }
    }
    qDebug() << "--------------------";
    return indexLowest;
}

float AiEvaluate::evaluate(Card *card) const
{
    float score = 0.0;
    switch (card->getRank()) {
        case Card::Rank::Seven:
            score += evaluateSeven();
            break;
        case Card::Rank::Eight:
            score += evaluateEight();
            break;
        case Card::Rank::Nine:
            score += evaluateNine();
            break;
        case Card::Rank::Ten:
            score += evaluateTen(card);
            break;
        case Card::Rank::Jack:
            score += evaluateJack(card);
            break;
        case Card::Rank::Queen:
            score += evaluateQueen(card);
            break;
        case Card::Rank::King:
            score += evaluateKing(card);
            break;
        case Card::Rank::Ace:
            score += evaluateAce(card);
            break;
        //default:
    }
    score += costOfLead() * probWinTrick(card);
    if (card->getSuit() == trumps)
        score += VALUE_OF_A_TURMP;
    //qDebug() << card->getRank() << card->getSuit() << "Cost of card: " << score;
    return score;
}

float AiEvaluate::costOfLead() const
{
    float cost = 0.0;
    cost += 5 * (1+hand->countTensAces())/9;
    cost -= hand->scoreMelds(trumps) / ((unseenCount + 1)/2);
    if (!leadCard
          && (opponentLead->getRank() == Card::Ace
          ||  opponentLead->getRank() == Card::Ten))
        cost -= 20;

    return cost;
}

float AiEvaluate::probOfFlush() const
{
    float prob = 1.0;
    if (hand->count(Card::Rank::Jack, trumps) == 0)
        prob *= probDealtCard(Card::Rank::Jack, trumps);
    if (hand->count(Card::Rank::Queen, trumps) == 0)
        prob *= probDealtCard(Card::Rank::Queen, trumps);
    if (hand->count(Card::Rank::King, trumps) == 0)
        prob *= probDealtCard(Card::Rank::King, trumps);
    if (hand->count(Card::Rank::Ten, trumps) == 0)
        prob *= probDealtCard(Card::Rank::Ten, trumps);
    if (hand->count(Card::Rank::Ace, trumps) == 0)
        prob *= probDealtCard(Card::Rank::Ace, trumps);
    return prob;
}

float AiEvaluate::probOfBezique() const
{
    float prob = 1.0;
    if (hand->count(Card::Rank::Jack, Card::Suit::Diamonds) == 0)
        prob *= probDealtCard(Card::Rank::Jack, Card::Suit::Diamonds);
    if (hand->count(Card::Rank::Queen, Card::Suit::Spades) == 0)
        prob *= probDealtCard(Card::Rank::Queen, Card::Suit::Spades);
    return prob;
}

float AiEvaluate::probOfDoubleBezique() const
{
    float prob = 1.0;
    if (hand->count(Card::Rank::Jack, Card::Suit::Diamonds) == 1)
    {
        prob *= probDealtCard(Card::Rank::Jack, Card::Suit::Diamonds);
    }
    else  if (hand->count(Card::Rank::Jack, Card::Suit::Diamonds) == 0)
    {
            prob *= probDealtN(2
                   , unseen->numUnseen(Card::Rank::Jack, Card::Suit::Diamonds));
    }

    if (hand->count(Card::Rank::Queen, Card::Suit::Spades) == 1)
    {
        prob *= probDealtCard(Card::Rank::Queen, Card::Suit::Spades);
    }
    else  if (hand->count(Card::Rank::Queen, Card::Suit::Spades) == 0)
    {
            prob *= probDealtN(2
                   , unseen->numUnseen(Card::Rank::Queen, Card::Suit::Spades));
    }

    return prob;
}

float AiEvaluate::probOfMarrage(Card::Suit suit) const
{
    float prob =1.0;
    if (hand->count(Card::Rank::King, suit) == 0)
        prob *= probDealtCard(Card::Rank::King, suit);
    if (hand->count(Card::Rank::Queen, suit) == 0)
        prob *= probDealtCard(Card::Rank::Queen, suit);
    return prob;
}

float AiEvaluate::probOfFourKind(Card::Rank rank) const
{
    float prob = 1.0;
    if (hand->countRank(rank) == 3)
    {
        prob *= probDealtN(1, unseen->numUnseenRank(rank));
    }
    else if (hand->countRank(rank) == 2)
    {
         prob *= probDealtN(2, unseen->numUnseenRank(rank));;
    }
    else if (hand->countRank(rank) == 1)
    {
        prob *= probDealtN(3, unseen->numUnseenRank(rank));
    }
    else if (hand->countRank(rank) == 0)
    {
        prob *= probDealtN(4, unseen->numUnseenRank(rank));
    }
    return prob;
}

float AiEvaluate::probDealtCard(Card::Rank rank, Card::Suit suit) const
{
    return probDealt(unseen->numUnseen(rank, suit), tricksLeft, unseenCount);
}

float AiEvaluate::probDealt(float numLeft, float tricks, float unseenCount) const
{
    float prob;
    if (numLeft > 0)
        prob = 1 - pow((unseenCount - tricks) / unseenCount, numLeft);
    else
        prob = 0.0;
    return prob;
}

float AiEvaluate::probDealtN(int n, float numLeft) const
{
     if (numLeft < n) return 0.0;
    float prob = 1.0;
    for ( int i=0 ; i<n ; i++ )
    {
        prob *= probDealt(numLeft-i, tricksLeft-i, unseenCount-i);
    }
    return prob;
}

float AiEvaluate::evaluateSeven() const
{
    float value = 0.0;
    if (1 == hand->count(Card::Seven, trumps))
    {
        if (faceCard->getRank() != Card::Seven)
            value += evaluate(faceCard)/2;
    }
    return value;
}

float AiEvaluate::evaluateEight() const
{
    return 0.0;
}

float AiEvaluate::evaluateNine() const
{
    return 0.0;
}

float AiEvaluate::evaluateTen(Card *card) const
{
    float value = 0.0;
    if (card->getSuit() == trumps && hand->count(Card::Rank::Ten, trumps) < 2)
    {
        value += valueFlush(score) * probOfFlush()
                  * ( 1 - probDealtCard((Card::Rank)card->getRank(), trumps));
    }

    if (leadCard)
    {
        bool beatsOurTen = false;
        for ( int i=0 ; i < opponentMelds.length() ; i++ )
        {
            if (!card->beats(*opponentMelds[i], trumps) )
                beatsOurTen = true;
        }
        if (beatsOurTen)
            value += COST_LOOSING_TEN;
        else
            value += COST_LOOSING_TEN * ( 1 -
                      pow(1- unseen->unseenThatBeat(card, trumps) /unseenCount
                          , BeziqueHand::HAND_SIZE - opponentMelds.size()));
    }
    else
    {
        if (opponentLead->beats(*card, trumps))
            value += COST_LOOSING_TEN;
        else
            value -=  2*SCORE_TEN - COST_LOOSING_TEN;
    }
    return value;
}

float AiEvaluate::evaluateJack(Card *card) const
{
    float value = 0.0;
    if (card->getSuit() == trumps && hand->count(Card::Rank::Jack, trumps) < 2)
    {
        value += valueFlush(score) * probOfFlush()
            * ( 1 -  probDealtCard((Card::Rank)card->getRank(), trumps));
    }

    if (hand->countRank(Card::Rank::Jack) < 5)
    {
        value += valueFourJacks(score) * probOfFourKind(Card::Rank::Jack)
              * ( 1 -  probDealtN(1, unseen->numUnseenRank(Card::Rank::Jack)));
    }

    if(card->getSuit() == Card::Suit::Diamonds)
    {
        if (hand->count(Card::Rank::Jack, Card::Suit::Diamonds) == 1)
        {
            value += valueBezique(score) * probOfBezique()
                * ( 1 -   probDealtCard(Card::Rank::Jack, Card::Suit::Diamonds));
        //}
        //if (hand->count(Card::Rank::Jack, Card::Suit::Diamonds) == 2)
        //{
            value += valueDoubleBezique(score) * probOfDoubleBezique();
        }
    }

    return value;
}

float AiEvaluate::evaluateQueen(Card *card) const
{
    float value = 0.0;

    if (card->getSuit() == trumps && hand->count(Card::Rank::Queen, trumps) < 2)
    {
        value += valueFlush(score) * probOfFlush()
            * ( 1 -  probDealtCard((Card::Rank)card->getRank(), trumps));
    }

    if (hand->countRank(Card::Rank::Queen) < 5)
    {
        value += valueFourQueens(score) * probOfFourKind(Card::Rank::Queen)
             * ( 1 -  probDealtN(1, unseen->numUnseenRank(Card::Rank::Queen)));
    }

    if(card->getSuit() == Card::Suit::Spades)
    {
        if (hand->count(Card::Rank::Queen, Card::Suit::Spades) == 1)
        {
            value += valueBezique(score) * probOfBezique()
                 * ( 1 -   probDealtCard(Card::Rank::Queen, Card::Suit::Spades));
        //}
        //if (hand->count(Card::Rank::Queen, Card::Suit::Spades) == 2)
        //{
            value += valueDoubleBezique(score) * probOfDoubleBezique();
        }
    }

    if (hand->count(Card::Rank::Queen, (Card::Suit) card->getSuit()) == 1)
    {
        int score = (card->getSuit() == trumps) ? valueRoyalMarrage(score)
                                                : valueMarrage(score);
        value += score * probOfMarrage((Card::Suit)card->getSuit())
        * ( 1 -  probDealtCard(Card::Rank::Queen, (Card::Suit)card->getSuit()));
    }

    return value;
}

float AiEvaluate::evaluateKing(Card *card) const
{
    float value = 0.0;

    if (card->getSuit() == trumps && hand->count(Card::Rank::King, trumps) < 2)
    {
        value += valueFlush(score) * probOfFlush()
            * ( 1 -  probDealtCard((Card::Rank)card->getRank(), trumps));
    }

    if (hand->countRank(Card::Rank::King) < 5)
    {
        value += valueFourKings(score)  * probOfFourKind(Card::Rank::King)
           * ( 1 -  probDealtN(1, unseen->numUnseenRank(Card::Rank::King)));
    }

    if (hand->count(Card::Rank::King, (Card::Suit) card->getSuit()) == 1)
    {
        int score = (card->getSuit() == trumps) ? valueRoyalMarrage(score)
                                                : valueMarrage(score);
        value += score * probOfMarrage((Card::Suit)card->getSuit())
             * ( 1 -  probDealtCard(Card::Rank::King, (Card::Suit)card->getSuit()));
    }

    return value;
}

float AiEvaluate::evaluateAce(Card *card) const
{
    float value = 0.0;

    if (card->getSuit() == trumps && hand->count(Card::Rank::Ace, trumps) < 2)
    {
        value += valueFlush(score) * probOfFlush()
             * ( 1 -  probDealtCard((Card::Rank)card->getRank(), trumps));
    }

    if (hand->countRank(Card::Rank::Ace) < 5)
    {
        value += valueFourAces(score) * probOfFourKind(Card::Rank::Ace)
             * ( 1 -  probDealtN(1, unseen->numUnseenRank(Card::Rank::Ace)));
    }

    if (leadCard)
    {
        bool beatsOurAce = false;
        for ( int i=0 ; i < opponentMelds.length() ; i++ )
        {
            if ( !card->beats(*opponentMelds[i], trumps) )
                beatsOurAce = true;
        }
        if (beatsOurAce)
            value += COST_LOOSING_ACE;
        else
            value += COST_LOOSING_ACE * ( 1 -
                    pow(1- unseen->unseenThatBeat(card, trumps) /unseenCount
                        , BeziqueHand::HAND_SIZE - opponentMelds.size()));
    }
    else
    {
        if (opponentLead->beats(*card, trumps))
            value += COST_LOOSING_ACE;
        else
            value -= 2*SCORE_ACE - COST_LOOSING_ACE;
    }

    return value;
}

float AiEvaluate::probWinTrick(Card *card) const
{
    if (!leadCard)
    {
        return opponentLead->beats(*card, trumps) ? 0 : 1;
    }
    else
    {
        return card->getSuit() == trumps ? 1 : 0.5;
    }
}






























































