//
// Created by hoerz on 15.12.2025.
//

#ifndef DECK_H
#define DECK_H
#include "card.h"

typedef struct {
    Card* data;
    int   size;
    int   capacity;
} Deck;

Deck* deck_create_standard(void);
Deck* deck_create_empty(int initial_capacity);
void deck_shuffle(const Deck* d);
int deck_is_empty(const Deck* d);
int deck_draw_top(Deck* d, Card* out);
int deck_insert(Deck* d, const Card* c);
unsigned int deck_count_worth(Deck* d);
int card_deal(Deck* stack, Deck* d, int count);
#endif
