//
// Created by hoerz on 11.02.2026.
//

#ifndef KARTENSPIEL_STRATEGY_H
#define KARTENSPIEL_STRATEGY_H

#include "main.h"
#include "player.h"

/**
 * @brief Wrapperfunktion für das Spielen einer Karte
 * @param csc Die Strategie des Spielers
 * @param deck Das Kartendeck des Spielers
 * @param card Die Karte die gespielt wird (Rückgabe)
 * @param card_to_beat Die Karte des Gegners (falls von der Strategie berücksichtigt)
 * @param isAttacker Boolescher Wert, ob der Spieler angreift oder verteidigt
 * @return Status-/Fehlercode:
 * - OK: Fehlerfrei
 * - NULL_POINTER_ERROR: leeres oder nicht initialisiertes Deck
 * - USER_INPUT_ERROR: Falsche beziehungsweise ungültige Eingabe seitens des Nutzers
 */
status card_play(card_strategy_chooser csc, Deck *deck, Card *card, Card *card_to_beat, boolean isAttacker);

/**
 * @brief Gibt eine beliebige Karte aus dem Deck zurück
 * @param deck Das Kartendeck des Spielers
 * @param card Die beliebige Karte (Rückgabe)
 * @param card_to_beat Die von der Strategie zu schlagende Karte (nicht berücksichtigt)
 * @param isAttacker Boolescher Wert, ob der Spieler angreift oder verteidigt
 * @return Status-/Fehlercode:
 * - OK: Fehlerfrei
 * - NULL_POINTER_ERROR: leeres oder nicht initialisiertes Deck
 */
status get_card(Deck *deck, Card *card, Card *card_to_beat, boolean isAttacker);

/**
 * @brief Gibt die höchste Karte aus dem Deck zurück
 * @param deck Das Kartendeck des Spielers
 * @param highest_card Die höchste Karte (Rückgabe)
 * @param card_to_beat Die von der Strategie zu schlagende Karte (nicht berücksichtigt)
 * @param isAttacker Boolescher Wert, ob der Spieler angreift oder verteidigt
 * @return Status-/Fehlercode:
 * - OK: Fehlerfrei
 * - NULL_POINTER_ERROR: leeres oder nicht initialisiertes Deck
 */
status get_Highest_card(Deck *deck, Card *highest_card, Card *card_to_beat, boolean isAttacker);

/**
 * @brief Gibt die höchste Karte aus dem Deck zurück
 * @param deck Das Kartendeck des Spielers
 * @param alternating_card Abwechselnd die höchste und niedrigste Karte (Rückgabe)
 * @param card_to_beat Die von der Strategie zu schlagende Karte (nicht berücksichtigt)
 * @param isAttacker Boolescher Wert, ob der Spieler angreift oder verteidigt
 * @return Status-/Fehlercode:
 * - OK: Fehlerfrei
 * - NULL_POINTER_ERROR: leeres oder nicht initialisiertes Deck
 */
status get_alternating_card(Deck *deck, Card *alternating_card, Card *card_to_beat, boolean isAttacker);

/**
 * @brief Gibt die höchste Karte aus dem Deck zurück
 * @param deck Das Kartendeck des Spielers
 * @param intelligent_card Eine intelligent ausgewählte Karte (Rückgabe)
 * @param card_to_beat Die von der Strategie zu schlagende Karte (berücksichtigt)
 * @param isAttacker Boolescher Wert, ob der Spieler angreift oder verteidigt
 * @return Status-/Fehlercode:
 * - OK: Fehlerfrei
 * - NULL_POINTER_ERROR: leeres oder nicht initialisiertes Deck
 */
status get_intelligent_card(Deck *deck, Card *intelligent_card, const Card *card_to_beat, boolean isAttacker);

/**
 * @brief Fragt den Nutzer, welche Karte er spielen möchte
 * @param deck Das Kartendeck des Spielers
 * @param humanoid_card Auswahl einer Karte über die CLI (Rückgabe)
 * @param card_to_beat Die von der Strategie zu schlagende Karte (falls von Strategie berücksichtigt)
 * @param isAttacker Boolescher Wert, ob der Spieler angreift oder verteidigt
 * @return Status-/Fehlercode:
 * - OK: Fehlerfrei
 * - NULL_POINTER_ERROR: leeres oder nicht initialisiertes Deck
 * - USER_INPUT_ERROR: Falsche beziehungsweise ungültige Eingabe seitens des Nutzers
 */
status get_humanoid_card(Deck *deck, Card *humanoid_card, Card *card_to_beat, boolean isAttacker);

#endif //KARTENSPIEL_STRATEGY_H
