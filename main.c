#include <stdio.h>
#include "deck.h"
#include "card.h"
//C-Level C11

int main(void) {
    Deck* d = deck_create_standard();
    Card c;
    while (deck_draw_top(d, &c) == 0) {
        printf("gezogene Karte: %u/%u\n",c.suit,c.rank);
    }
    return 0;
}