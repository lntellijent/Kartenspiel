//
// Created by hoerz on 15.12.2025.
//

#ifndef DECK_H
#define DECK_H
#include "card.h"

typedef struct {
    Card* data;     // dynamisches Array (52 Karten)
    int   size;     // aktuelle Anzahl
    int   capacity; // typ. 52
} Deck;

Deck* deck_create_standard(void);            // 52 geordnete Karten
void  deck_free(Deck* d);

void  deck_shuffle(Deck* d);                 // Fisher-Yates mit RNG-API
int   deck_is_empty(const Deck* d);

int   deck_draw_top(Deck* d, Card* out);     // LIFO-Top; returns 0 ok / -1 empty
int   deck_draw_random(Deck* d, Card* out);  // zufällige Karte; 0/-1
#endif
