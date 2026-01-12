//
// Created by hoerz on 15.12.2025.
//

#ifndef DECK_H
#define DECK_H
#include "card.h"
#include "main.h"

typedef struct {
    Card *cards;
    int card_count;
    int capacity;
} Deck;

Deck *create_standard_deck(void);

Deck *create_empty_deck(int initial_capacity);

status shuffle(const Deck *d);

boolean is_empty(const Deck *d);

status deck_draw_top(Deck *d, Card *out);

status deck_draw_index(Deck *source_deck, Card *out, int index);

status insert(Deck *source_deck, const Card *card_output);

int consume_and_count_worth(Deck *source_deck);

status card_deal(Deck *main_deck, Deck *destination_deck, int card_count);

status print_deck(Deck *source_deck, boolean print_indexes);
#endif
