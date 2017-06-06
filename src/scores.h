#ifndef SCORES_H
#define SCORES_H
#include  <algorithm>
#include "player.h"

static const int GAME_SCORE = 1000;
static const int SCORE_SEVEN = 10;
static const int SCORE_MARRAGE = 20;
static const int SCORE_ROYAL_MARRAGE = 40;
static const int SCORE_FLUSH = 250;
static const int SCORE_BEZIQUE = 40;
static const int SCORE_DOUBLE_BEZIQUE = 500;
static const int SCORE_ACE = 10;
static const int SCORE_TEN = 10;
static const int SCORE_LAST_TRICK = 10;
static const int SCORE_FOUR_JACKS = 40;
static const int SCORE_FOUR_QUEENS = 60;
static const int SCORE_FOUR_KINGS = 80;
static const int SCORE_FOUR_ACES = 100;

/*
static int valueSeven(int score = 1000);
static int valueMarrage(int score = 1000);
static int valueRoyalMarrage(int score = 1000);
static int valueFlush(int score = 1000);
static int valueBezique(int score = 1000);
static int valueDoubleBezique(int score = 1000);
static int valueFourJacks(int score = 1000);
static int valueFourQueens(int score = 1000);
static int valueFourKings(int score = 1000);
static int valueFourAces(int score = 1000);
*/
inline static int valueSeven(int score)
{
    return min(SCORE_SEVEN, GAME_SCORE - score);
}

inline static  int valueMarrage(int score)
{
    return min(SCORE_MARRAGE, GAME_SCORE - score);
}

inline static  int valueRoyalMarrage(int score)
{
    return min(SCORE_ROYAL_MARRAGE, GAME_SCORE - score);
}

inline static  int valueFlush(int score)
{
     return min(SCORE_FLUSH, GAME_SCORE - score);
}

inline static  int valueBezique(int score)
{
     return min(SCORE_BEZIQUE, GAME_SCORE - score);
}

inline static  int valueDoubleBezique(int score)
{
     return min(SCORE_DOUBLE_BEZIQUE, GAME_SCORE - score);
}

inline static  int valueFourJacks(int score)
{
    return min(SCORE_FOUR_JACKS, GAME_SCORE - score);
}

inline static  int valueFourQueens(int score)
{
    return min(SCORE_FOUR_QUEENS, GAME_SCORE - score);
}

inline static  int valueFourKings(int score)
{
     return min(SCORE_FOUR_KINGS, GAME_SCORE - score);
}

inline static  int valueFourAces(int score)
{
     return min(SCORE_FOUR_ACES, GAME_SCORE - score);
}

#endif // SCORES_H
