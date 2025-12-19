#include <stdbool.h>
#include <stdio.h>
#include "card.h"
#include "deck.h"
#include "player.h"
//C-Level C11

int main_test() {
    Deck * deck = deck_create_standard();
    Card c;

    while (deck_draw_top(deck, &c) == 0) {
        printf("%s/%c\n", rank_arr[c.rank], suit_arr[c.suit]);
    }
    return 0;
}

int main() {
    Deck* d = deck_create_standard();
    // Initialisiserung fehlgeschlagen
    if (!d)
        return -116;
    deck_shuffle(d);

    // Spieler(-Decks) initialisieren
    const int hand_size = 10;
    const gamer players[2] = {
        {.hand = deck_create_empty(hand_size), .points = deck_create_empty(hand_size), .strategy = 0}, // Spieler
        {.hand = deck_create_empty(hand_size), .points = deck_create_empty(hand_size), .strategy = 0} // Gegner
    };

    // Initialisierung fehlgeschlagen
    if (!players[0].hand || !players[0].points || !players[1].hand || !players[1].points)
        return -117;

    // Austeilen der Karten
    for (int p = 0; p < sizeof(players)/sizeof(players[0]); p++) {
        int err;
        if ((err = card_deal(d, players[p].hand, hand_size)) != 0)
            return err;
    }

    Card c_att, c_def;

    const int player_size = sizeof(players)/sizeof(players[0]);
    int round = 0;
    while (round < hand_size * player_size) {
        round += 2;
        printf("Zug %d\n", round / player_size);

        /*
         * Attacker legt eine Karte
         */
        int err;
        if ((err = player_play_card(players[0], &c_att)) != 0)
            return err;
        printf("Spieler %d legt %s%c\n", 0, rank_arr[c_att.rank], suit_arr[c_att.suit]);

        /*
         * Defender legt eine Karte
         */
        if ((err = player_play_card(players[1], &c_def)) != 0)
            return err;
        printf("Spieler %d legt %s%c\n", 1, rank_arr[c_def.rank], suit_arr[c_def.suit]);

        // Stich wird entschieden
        switch (card_clash(&c_att, &c_def)) {
            default:
                return -5;
            case 0:
                if ((err = deck_insert(players[1].points, &c_att)) != 0)
                    return err;
                if ((err = deck_insert(players[1].points, &c_def)) != 0)
                    return err;
                printf("Spieler %d gewinnt\n", 1);
                break;
            case 1:
                if((err = deck_insert(players[0].points, &c_att)) != 0)
                    return err;
                if ((err = deck_insert(players[0].points, &c_def)) != 0)
                    return err;
                printf("Spieler %d gewinnt\n", 0);
                break;
        }
    }

    printf("\n\n");
    for (int p = 0; p < player_size; p++) {
        int points = deck_count_worth(players[p].points);
        if (points == -113)
            return -113;
        printf("Spieler %d erzielte %d Punkte\n", p, points);
    }

    return 0;
}