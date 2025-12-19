//
// Created by hoerz on 18.12.2025.
//

#include "player.h"

#include <stdio.h>

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
 */
int player_play_card(const gamer player, Card* played_card) {
    switch (player.strategy) {
        case 0:
            if (deck_draw_top(player.hand, played_card) != 0) {
                return -1;
            }
            return 0;
        default: return -825;
    }
}