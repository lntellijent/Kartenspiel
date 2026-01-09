//
// Created by hoerz on 15.12.2025.
//

#ifndef CARD_H
#define CARD_H

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


static const int suit_arr[SUIT_COUNT] = { 6,5,3,4 };
static const char* rank_arr[RANK_COUNT] = {
    "_","_", "2","3","4","5","6","7","8","9","10","B","D","K","A"
};
   // "♠", "♥" ...
winner card_clash(const Card* a, const Card* b); // -1/0/+1 (optional)
#endif
