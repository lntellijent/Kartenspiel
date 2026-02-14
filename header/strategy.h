//
// Created by hoerz on 11.02.2026.
//

#ifndef KARTENSPIEL_STRATEGY_H
#define KARTENSPIEL_STRATEGY_H

#include "main.h"
#include "player.h"

// #ToDo Doc
status card_play(card_strategy_chooser csc, Deck *deck, Card *card, Card *card_to_beat, boolean isAttacker);

// #ToDo Doc
status get_card(Deck *deck, Card *highest_card, Card *card_to_beat, boolean isAttacker);

// #ToDo Doc
status get_Highest_card(Deck *deck, Card *highest_card, Card *card_to_beat, boolean isAttacker);

/**
 * @brief teilt eine Karte aus.
 * @param deck Das Deck aus welchem die Karte gezogen werden soll.
 * @param alternating_card enthält die Karte, die gezogen wurde.
 * @param card_to_beat
 * @param isAttacker
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - NULL_POINT_ERROR: Deck oder Karte nicht initialisiert oder gleich NULL
 */
status get_alternating_card(Deck *deck, Card *alternating_card, Card *card_to_beat, boolean isAttacker);

// #ToDo Doc

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
status get_intelligent_card(Deck *deck, Card *intelligent_card, const Card *card_to_beat, boolean isAttacker);

// #ToDo Doc
status get_humanoid_card(Deck *deck, Card *humanoid_card, Card *card_to_beat, boolean isAttacker);

#endif //KARTENSPIEL_STRATEGY_H
