//
// Created by hoerz on 15.12.2025.
//

#ifndef CARD_H
#define CARD_H

typedef enum {
    SUIT_SPADES, // ♠
    SUIT_CLUBS, // ♣
    SUIT_HEARTS, // ♥
    SUIT_DIAMONDS // ♦
} Suit;

typedef enum {
    RANK_2 = 2, RANK_3, RANK_4, RANK_5, RANK_6,
    RANK_7, RANK_8, RANK_9, RANK_10,
    RANK_J, RANK_Q, RANK_K, RANK_A
} Rank;

typedef struct {
    Suit suit;
    Rank rank;
} Card;

const char* card_rank_str(Rank r);     // "2", "J", "A" ...
const char* card_suit_str(Suit s);     // "♠", "♥" ...
int card_compare(const Card* a, const Card* b); // -1/0/+1 (optional)

#endif
