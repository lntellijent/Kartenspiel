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
status round_sequence(size_t round_index);

/**
 * @brief Dient als Platzhalter für CLI-Nachrichten, wenn eine Karte gespielt wird.
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 */
status card_played(const wchar_t *player_name, const Card *card, boolean follow_up);

/**
 * @brief Dient als Platzhalter für CLI-Nachrichten
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 */
status clash_decided(const wchar_t *player_name, const Card* card1, const Card* card2);

/**
 * @brief Dient als Platzhalter für CLI-Nachrichten am Ende des Spiels.
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 */
status game_winner(const wchar_t *winning_player_name, size_t winning_player_points);

/**
 *
 * @param deck Das Deck, welches angezeigt werden soll
 * @param player_isAttacker Teilt mit, ob der Spieler angreift oder verteidigt.
 * @param print_indexes Anzeige der Indexnummern jeder Karte eine Zeile unter den Karten
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - NULL_POINT_ERROR: Deck ist leer oder =NULL
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 */
status print_deck(Deck *deck, boolean player_isAttacker, boolean print_indexes);

/**
 * @brief Parst eine einzelne Ziffer von der Konsole
 * @param out Die gelesene Ziffer
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - USER_INPUT_ERROR: Es konnte keine einzelne Zahl isoliert werden
 */
status read_single_digit(size_t *out);

status read_yes_no(boolean *out);

status read_string(wchar_t out[32]);
#endif //KARTENSPIEL_CLI_H
