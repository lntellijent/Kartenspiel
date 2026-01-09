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
 * - OK: Fehlerfrei
 * - NULLPOINT_ERROR: Deck ist leer
 * - USER_INPUT_ERROR: invalide Strategie ausgewählt
 * @note Strategien:
 * 1. Zufall
 * 2. niedrigste zuerst
 * 3. höchste zuerst
 * 4. Intelligent
 */
status player_play_card(const player player, Card* played_card) {
    switch (player.strategy) {
        case 0:
            if (player.hand->size <=0) return NULLPOINT_ERROR;
            print_deck(player.hand, 1);

            if (player.hand->size > 1) {
                char name[4];
                int chosen_card = -1;
                while (chosen_card == -1) {
                    fgets(name, sizeof(name), stdin);
                    const int input = strtol(name, NULL, 10);
                    if (input >= 0 && input < player.hand->size)
                        chosen_card = input;
                }

                status err;
                if ((err = deck_draw_index(player.hand, played_card, chosen_card)) != OK) return err;
            } else {
                int err;
                if ((err = deck_draw_top(player.hand, played_card)) != OK) return err;
            }
            return OK;
        case 1:
            if (deck_draw_top(player.hand, played_card) != OK) return NULLPOINT_ERROR;
            return OK;
        default: return USER_INPUT_ERROR;
    }
}