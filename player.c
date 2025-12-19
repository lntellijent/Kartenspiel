//
// Created by hoerz on 18.12.2025.
//

#include "player.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Spielt eine Karte
 *
 * (Unabhängig davon ob KI-Gegner oder menschlicher Spieler)
 * @param player Der Spieler, welcher eine Karte spielen soll
 * @param played_card Entält die Karte die gespielt wird
 * @return Fehler-/Statuscodes:
 * - 0: Fehlerfrei
 * - -1: Deck ist leer
 * - -825: invalide Strategie ausgewählt
 * @note Strategien:
 * 1. Zufall
 * 2. niedrigste zuerst
 * 3. höchste zuerst
 * 4. Intelligent
 */
int player_play_card(const gamer player, Card* played_card) {
    switch (player.strategy) {
        case 0:
            if (player.hand->size <=0)
                return -1;

            printf("Deine Karten:\n%5s","");
            for (int i = 0 ; i < player.hand->size ; i++)
                printf("%s%2s%c%s", i != 0 ? " " : "", rank_arr[player.hand->data[i].rank], suit_arr[player.hand->data[i].suit], i + 1 == player.hand->size ? ".\n" : "");
            printf("%5s", "");
            for (int i = 0 ; i < player.hand->size ; i++)
                printf("%1s[%d]", "", i);
            printf("\n");

            if (player.hand->size > 1) {
                char name[4];
                int chosen_card = -1;
                while (chosen_card == -1) {
                    fgets(name, sizeof(name), stdin);
                    const int input = strtol(name, NULL, 10);
                    if (input >= 0 && input < player.hand->size)
                        chosen_card = input;
                }

                int err;
                if ((err = deck_draw_index(player.hand, played_card, chosen_card)))
                    return err;
            } else {
                int err;
                if ((err = deck_draw_top(player.hand, played_card)))
                    return err;
            }
            return 0;
        case 1:
            if (deck_draw_top(player.hand, played_card) != 0) {
                return -1;
            }
            return 0;
        default: return -825;
    }
}