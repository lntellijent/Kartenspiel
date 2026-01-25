//
// Created by hoerz on 18.12.2025.
//

#ifndef KARTENSPIEL_PLAYER_H
#define KARTENSPIEL_PLAYER_H

#include "deck.h"

typedef struct {
    Deck *hand;
    Deck *points;
    int strategy;
    char* name;
} player;

/**
 * @brief Spielt eine Karte
 *
 * (Unabhängig davon ob KI-Gegner oder menschlicher Spieler)
 * @param player Der Spieler, welcher eine Karte spielen soll
 * @param played_card Entält die Karte die gespielt wird
 * @return Fehler-/Statuscodes:
 * - OK: Fehlerfrei
 * - NULLPOINT_ERROR: Deck ist leer oder =NULL
 * - USER_INPUT_ERROR: invalide Strategie ausgewählt
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 * @note Strategien:
 * 1. Zufall
 * 2. niedrigste zuerst
 * 3. höchste zuerst
 * 4. Intelligent
 */
status player_play_card(player player, Card *played_card);

status get_lowest_card(Deck *deck, Card *lowest_card);

status get_alternating_card(Deck *deck, Card *highest_card);

status get_intelligent_card(Deck *deck, Card *intelligent_card);
#endif
