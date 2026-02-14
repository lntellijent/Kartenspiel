//
// Created by hoerz on 15.12.2025.
//

#ifndef CARD_H
#define CARD_H

#include <wchar.h>
#include "../header/main.h"

typedef enum {
    SUIT_SPADES, // ♠
    SUIT_CLUBS, // ♣
    SUIT_HEARTS, // ♥
    SUIT_DIAMONDS, // ♦
    SUIT_COUNT
} Suit;

typedef enum {
    RANK_2 = 2, RANK_3, RANK_4, RANK_5, RANK_6,
    RANK_7, RANK_8, RANK_9, RANK_10,
    RANK_J, RANK_Q, RANK_K, RANK_A,
    RANK_COUNT
} Rank;

typedef struct {
    Suit suit;
    Rank rank;
} Card;

typedef enum {
    DEFENDER_WINS,
    TIE,
    ATTACKER_WINS
} winner;


static const wchar_t *suit = L"♠♣♥♦";
static const wchar_t *rank[RANK_COUNT] = {
    L"_", L"_", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"B", L"D", L"K", L"A"
};

status print_card(Card card, wchar_t *s);


#endif
