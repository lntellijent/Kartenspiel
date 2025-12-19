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
    deck_shuffle(d);

    const int hand_size = 10;
    const gamer players[2] = {
        {.hand = deck_create_empty(hand_size), .points = deck_create_empty(hand_size), .strategy = 0}, // Spieler
        {.hand = deck_create_empty(hand_size), .points = deck_create_empty(hand_size), .strategy = 0} // Gegner
    };

    // Austeilen der Karten
    for (int p = 0; p < sizeof(players)/sizeof(players[0]); p++) {
        card_deal(d, players[p].hand, hand_size);
    }

    Card c_att, c_def;

    const int player_size = sizeof(players)/sizeof(players[0]);
    int round = 0;
    const int player_1 = round;
    const int player_2 = round + 1;
    while (round < hand_size * player_size) {
        round += 2;

        printf("Zug %d\n", round / player_size);
        // Attacker legt eine Karte
        int err;
        if ((err = player_play_card(players[player_1], &c_att)) != 0) {
            return err;
        }
        printf("Spieler %d legt %s%c\n", player_1, rank_arr[c_att.rank], suit_arr[c_att.suit]);

        // Defender legt eine Karte

        if (player_play_card(players[player_2], &c_def) != 0) {
            return -2;
        }
        printf("Spieler %d legt %s%c\n", player_2, rank_arr[c_def.rank], suit_arr[c_def.suit]);

        // Stich wird entschieden
        switch (card_clash(&c_att, &c_def)) {
            default:
                return -5;
            case 0:
                deck_insert(players[player_2].points, &c_att);
                deck_insert(players[player_2].points, &c_def);
                printf("Spieler %d gewinnt\n", player_2);
                break;
            case 1:
                deck_insert(players[player_1].points, &c_att);
                deck_insert(players[player_1].points, &c_def);
                printf("Spieler %d gewinnt\n", player_1);
                break;
        }
    }

    printf("\n\n");
    for (int p = 0; p < player_size; p++) {
        printf("Spieler %d erzielte %d Punkte\n", p, deck_count_worth(players[p].points));
    }

    return 0;
}