#include <stdio.h>
#include "deck.h"
#include "card.h"
//C-Level C11

static char getSuit(Suit s) {
    switch (s) {
        case 0:
            return 'S';
        case 1:
            return 'C';
        case 2:
            return 'H';
        case 3:
            return 'D';
        default:
            return 'N';
    }
}

int main(void) {
    Deck* d = deck_create_standard();
    Card c;
    while (deck_draw_top(d, &c) == 0) {
        printf("gezogene Karte: %c/%u\n",getSuit(c.suit),c.rank);
    }
    return 0;
}