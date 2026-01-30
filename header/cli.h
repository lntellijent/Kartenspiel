//
// Created by hoerz on 30.01.2026.
//

#ifndef KARTENSPIEL_CLI_H
#define KARTENSPIEL_CLI_H
#include "card.h"
#include "deck.h"
#include "main.h"

/**
 * @brief printed verschönerte CLI-Elemente, um den Namen zu erfragen.
 * @return Statuscode:
 */
status ask_name();

/**
 * @brief Dient als Platzhalter für CLI-Nachrichten, falls eine ungültige Eingabe getätigt wurde.
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 */
status invalid_user_response();

/**
 * @brief Dient als Platzhalter für CLI-Nachrichten am Start des Spiels.
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 */
status start_sequence();

/**
 * @brief Dient als Platzhalter für CLI-Nachrichten für den Anfang einer Runde.
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 */
status round_sequence(size_t round);

/**
 * @brief Dient als Platzhalter für CLI-Nachrichten, wenn eine Karte gespielt wird.
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 */
status card_played(const wchar_t* player_name, const Card *card, const boolean follow_up);

/**
 * @brief Dient als Platzhalter für CLI-Nachrichten
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 */
status clash_decided(const wchar_t* player_name);

/**
 * @brief Dient als Platzhalter für CLI-Nachrichten am Ende des Spiels.
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 */
status game_winner(const wchar_t* winning_player_name, int winning_player_points);

/**
 * @brief Zieht eine bestimmte Karte aus dem Deck. Hier: Die niedrigste
 * @param deck Das Deck aus welchem die Karte genommen wird
 * @param card die Karte selbst; hier die Karte des Decks am übergebenen Index
 * @param index der Index der Karte, die gezogen werden soll
 * @return statuscodes:
 * - OK: Fehlerfrei
 * - NULL_POINT_ERROR: Karte oder Deck ist nicht initialisiert/ist NULL
 */
status deal_card_by_index(const Deck *deck, const Card *card, int index);

/**
 *
 * @param source_deck Das Deck, welches angezeigt werden soll
 * @param isAttacker Teilt mit, ob der Spieler angreift oder verteidigt.
 * @param print_indexes Anzeige der Indexnummern jeder Karte eine Zeile unter den Karten
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - NULL_POINT_ERROR: Deck ist leer oder =NULL
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 */
status print_deck(Deck *source_deck, boolean isAttacker, boolean print_indexes);

#endif //KARTENSPIEL_CLI_H