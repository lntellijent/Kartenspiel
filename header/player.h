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
    wchar_t name[32];
} player;

/**
 * @brief Ermittelt Spielernamen für ein Playerobjekt
 * @param player das entsprechende Spielerobjekt
 * @return statuscode:
 * -
 */
status player_name(player* player);

/**
 * @brief Spielt eine Karte
 *
 * (Unabhängig davon ob KI-Gegner oder menschlicher Spieler)
 * @param hplayer Der Spieler, welcher eine Karte spielen soll
 * @param played_card Entält die Karte die gespielt wird
 * @param card_to_beat Die Karte des Gegners, die es zu schlagen gilt
 * @param isAttacker Teilt mit, ob der Spieler an der Reihe angreift oder abwehrt.
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
status player_play_card(player hplayer, Card *played_card, player opponent, Card card_to_beat, boolean isAttacker);

/**
 * @brief teilt eine Karte aus.
 * @param deck Das Deck aus welchem die Karte gezogen werden soll.
 * @param alternating_card enthält die Karte, die gezogen wurde.
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - NULL_POINT_ERROR: Deck oder Karte nicht initialisiert oder gleich NULL
 */
status get_alternating_card(Deck *deck, Card *alternating_card);

/**
 * @brief teilt eine Karte aus.
 * @param deck Das Deck aus welchem die Karte gezogen werden soll.
 * @param intelligent_card enthält die Karte, die gezogen wurde.
 * @param card_to_beat Die Karte des Gegners, die es zu schlagen gilt
 * @param isAttacker Teilt mit, ob der Spieler an der Reihe angreift oder abwehrt.
 * @return statuscode:
 * - OK: Fehlerfrei
 *
 */
status get_intelligent_card(Deck *deck, Card *intelligent_card, Card card_to_beat, boolean isAttacker);
#endif
