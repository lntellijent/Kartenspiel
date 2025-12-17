#include <stdio.h>
#include "deck.h"
#include "card.h"
//C-Level C11

int main(void) {
    Deck* d = deck_create_standard();
    deck_shuffle(d);
    Card c;

    const int suit_arr[] = {6,5,3,4}; // charnummern für Pik, Kreuz, Herz und Karo
    const char *rank_arr[] = {"","","2","3","4","5","6","7","8","9","10", "B", "D", "K", "A"};
    // charrarrays für die Zahlen 2 bis 10, Bube, Dame, König und Ass

    Deck* Spieler = deck_create_empty(10);
    int i = 0;
    while (deck_draw_top(d, &c) == 0 && ++i < 10) {
        //printf("input Karte: %c/%s\n", suit_arr[c.suit], rank_arr[c.rank]);
        deck_insert(Spieler, &c);
    }

    i = 0;
    while (deck_draw_top(Spieler, &c) == 0 && ++i < 10) {
        printf("Spieler zieht: %c/%s\n", suit_arr[c.suit], rank_arr[c.rank]);
    }

    printf("\n%d", d->size);

    return 0;
}