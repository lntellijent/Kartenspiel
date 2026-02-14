//
// Created by hoerz on 30.01.2026.
//

#ifndef KARTENSPIEL_CLI_H
#define KARTENSPIEL_CLI_H
#define INPUT_LENGTH 32
#include "card.h"
#include "deck.h"
#include "main.h"

typedef status (*reader_fn)(const wchar_t *input, void *output);

typedef struct {
    wchar_t *ptr;
    size_t cap;
} wbuf;

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
status clash_decided(const wchar_t *player_name, const Card *greater_card, const Card *lower_card);

/**
 * @brief Dient als Platzhalter für CLI-Nachrichten am Ende des Spiels.
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 */
status game_winner(const wchar_t *winning_player_name, size_t winning_player_points, const wchar_t *loosing_player_name,
                   size_t loosing_player_points);

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
 * @param input Der getrimmte User-Input
 * @param out_void Die gelesene Ziffer
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - USER_INPUT_ERROR: Es konnte keine einzelne Zahl isoliert werden
 */
status read_single_digit(const wchar_t *input, void *out_void);

/**
 * @brief Parst vordefinierte Buchstaben für eine Ja/Nein-Entscheidung
 * @param input Der getrimmte User-Input
 * @param out_void Die gelesene Ja/Nein Entscheidung als Boolean
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - USER_INPUT_ERROR: Es konnte keine Ja/Nein-Entscheidung isoliert werden
 */
status read_yes_no(const wchar_t *input, void *out_void);

status read_string(const wchar_t *input, void *out_void);

status read_with(reader_fn reader, void *output);
#endif //KARTENSPIEL_CLI_H
