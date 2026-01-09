//
// Created by hoerz on 15.12.2025.
//

#ifndef DECK_H
#define DECK_H
#include "card.h"
#include "main.h"

typedef struct {
    Card* data;
    int   size;
    int   capacity;
} Deck;

Deck* deck_create_standard(void);
Deck* deck_create_empty(int initial_capacity);
status deck_shuffle(const Deck* d);
boolean deck_is_empty(const Deck* d);
status deck_draw_top(Deck* d, Card* out);
status deck_draw_index(Deck* d, Card* out, int index);
status deck_insert(Deck* d, const Card* c);
int deck_consume_and_count_worth(Deck* d);
status card_deal(Deck* stack, Deck* d, int count);
status print_deck(Deck* d,int print_indexes);
#endif
