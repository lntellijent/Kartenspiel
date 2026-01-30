//
// Created by hoerz on 30.01.2026.
//

#ifndef KARTENSPIEL_CLI_H
#define KARTENSPIEL_CLI_H
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
status round_sequence(const int round)

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
status game_winner(const wchar_t* winning_player, const int winning_player_points);

/**
 * @brief Zieht eine bestimmte Karte aus dem Deck. Hier: Die niedrigste
 * @param deck Das Deck aus welchem die Karte genommen wird
 * @param card die Karte selbst; hier die Karte des Decks am übergebenen Index
 * @param index der Index der Karte, die gezogen werden soll
 * @return statuscodes:
 * - OK: Fehlerfrei
 * - NULL_POINT_ERROR: Karte oder Deck ist nicht initialisiert/ist NULL
 */
status deal_card_by_index(Deck *deck, Card *card, const int index);

#endif //KARTENSPIEL_CLI_H