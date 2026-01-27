#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#include <fcntl.h>

#include "../header/player.h"

#define DEFAULT_GAME_REPEAT_SETTING FALSE
#define DEFAULT_GAME_REPEAT_ON_ENTER_SETTING TRUE

//C-Level C11

/**
 * #ToDo
 * @brief Dient als Platzhalter für CLI-Nachrichten am Start des Spiels.
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 */
status start_sequence() {
    const char *banner =
            ".---------.\t.---------.\t.---------.\t.---------.\n"
            "|         |\t|         |\t|         |\t|         |\n"
            "|         |\t|         |\t|         |\t|         |\n"
            "|         |\t|         |\t|         |\t|         |\n"
            "|         |\t|         |\t|         |\t|         |\n"
            "'---------'\t'---------'\t'---------'\t'---------'\n"
            "                      Das Kartenspiel             \n";
    if (wprintf(L"\n%hs\n\n", banner) < 0) return PRINT_ERROR;
    return OK;
}

/**
 * #ToDo
 * @brief Dient als Platzhalter für CLI-Nachrichten für den Anfang einer Runde.
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 */
status round_sequence(const int round) {
    if (wprintf(L"\n%hs[Zug %d]%hs\n\n", "--------------------- ", round, " ---------------------") < 0) return PRINT_ERROR;
    return OK;
}

/**
 * #ToDo
 * @brief Dient als Platzhalter für CLI-Nachrichten, wenn eine Karte gespielt wird.
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 */
status card_played(const wchar_t* player_name, const Card *card, const boolean follow_up) {
    if (follow_up)
        if (wprintf(L", ") < 0) return PRINT_ERROR;
    if (wprintf(L"%ls legt %hs%.1ls", player_name, ranks[card->rank], &suits[card->suit]) < 0)
        return PRINT_ERROR;
    return OK;
}

/**
 * #ToDo
 * @brief Dient als Platzhalter für CLI-Nachrichten
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 */
status clash_decided(const wchar_t* player_name) {
    if (wprintf(L" - %ls gewinnt\n", player_name) < 0) return PRINT_ERROR;

    return OK;
}

/**
 * #ToDo
 * @brief Dient als Platzhalter für CLI-Nachrichten am Ende des Spiels.
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 */
status game_winner(const wchar_t* winning_player, const int winning_player_points) {
    if (wprintf(L"%ls gewinnt mit %d Punkten!\n", winning_player, winning_player_points) < 0) return PRINT_ERROR;
    return OK;
}

status game_start() {
    status error;
    Deck *main_deck = create_standard_deck();
    // Initialisiserung fehlgeschlagen
    if ((error = shuffle(main_deck)) != OK) return error;

    // Spieler(-Decks) initialisieren
    const int hand_size = 10;
    player players[2] = {
        {.hand = create_empty_deck(hand_size), .points = create_empty_deck(hand_size), .strategy = 0}, // Spieler
        {.hand = create_empty_deck(hand_size), .points = create_empty_deck(hand_size), .strategy = 4} // Gegner
    };

    // Initialisierung fehlgeschlagen
    if (!players[0].hand || !players[0].points || !players[1].hand || !players[1].points) return NULL_POINT_ERROR;


    for (int i = 0; i < sizeof(players)/sizeof(players[0]); i++) {
        if ((error = player_name(&players[i])) != OK) return error;
    }

    // Austeilen der Karten
    for (int p = 0; p < sizeof(players) / sizeof(players[0]); p++)
        if ((error = card_deal(main_deck, players[p].hand, hand_size)) != OK) return error;

    if (is_empty(players[0].hand) || is_empty(players[1].hand)) return NULL_POINT_ERROR;

    // Start-Sequenz des Spiels
    if ((error = start_sequence()) != OK) return error;

    Card attacker_card, defender_card;
    int attacker_index = 1, defender_index = 0;

    const int player_size = sizeof(players) / sizeof(players[0]);
    int round_x_turn = 0;
    while (!(is_empty(players[0].hand) && is_empty(players[1].hand))) {
        round_x_turn += 2;

        // Runden-Sequenz des Spiels
        if ((error = round_sequence(round_x_turn / 2)) != OK) return error;

        // Angreifer legt eine Karte
        if ((error = player_play_card(players[attacker_index], &attacker_card, players[defender_index], attacker_card, TRUE)) != OK) return error;
        if ((error = card_played(players[attacker_index].name, &attacker_card, FALSE)) != OK) return error;

        // Verteidiger legt eine Karte
        if ((error = player_play_card(players[defender_index], &defender_card, players[attacker_index], attacker_card, FALSE)) != OK) return error;
        if ((error = card_played(players[defender_index].name, &defender_card, defender_index)) != OK) return error;


        // Stich wird entschieden
        switch (card_clash(&attacker_card, &defender_card)) {
            default: return NULL_POINT_ERROR;
            case DEFENDER_WINS:
                if ((error = insert(players[defender_index].points, &attacker_card)) != OK) return error;
                if ((error = insert(players[defender_index].points, &defender_card)) != OK) return error;
                if ((error = clash_decided(players[defender_index].name)) != OK) return error;
                // Angreifer-Verteidigerrolle wird getauscht
                const int temp = attacker_index;
                attacker_index = defender_index;
                defender_index = temp;
                break;
            case TIE: // Angreifer gewinnt ebenfalls, wenn die Gleiche Karte gelegt wurde
            case ATTACKER_WINS:
                if ((error = insert(players[attacker_index].points, &attacker_card)) != OK) return error;
                if ((error = insert(players[attacker_index].points, &defender_card)) != OK) return error;
                if ((error = clash_decided(players[attacker_index].name)) != OK) return error;
                // Angreifer-Verteidigerrolle wird nicht getauscht
                break;
        }
    }

    int winning_player_index = 0, winning_player_points = 0;
    if (wprintf(L"\n\n") < 0) return PRINT_ERROR;
    for (int p = 0; p < player_size; p++) {
        const int points = consume_and_count_worth(players[p].points);
        if (points == -1) return NULL_POINT_ERROR;
        if (winning_player_points < points) {
            winning_player_index = p;
            winning_player_points = points;
        }
    }

    if ((error = game_winner(players[winning_player_index].name, winning_player_points)) != OK) return error;

    // Speicher freigeben
    free(players[0].hand->cards);
    free(players[0].hand);
    free(players[0].points->cards);
    free(players[0].points);
    free(players[1].hand->cards);
    free(players[1].hand);
    free(players[1].points->cards);
    free(players[1].points);
    free(main_deck->cards);
    free(main_deck);

    return OK;
}

/**
 * @brief Entfernt führende und nachfolgende Leerzeichen aus einem String
 * @param s den zu trimmenden String
 */
static void trim(char *s) {
    while (*s && isspace((unsigned char) *s)) s++; // führende
    if (*s == '\0') {
        s[0] = '\0';
        return;
    } // nur Spaces
    char *end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char) *end)) end--; // nachfolgende
    *(end + 1) = '\0';
}

/**
 * @brief Zeigt einen Prompt und akzeptiert 'j' für Ja oder 'n' für Nein.
 * @return TRUE oder FALSE, für Ja oder Nein
 * @note Fragt wiederholt nach, bis eine gültige Eingabe erfolgt.
 */
boolean ask_yes_no() {
    wchar_t line[2];

    for (;;) {
        wprintf(L"Noch ein mal Spielen? [j/n]: ");
        fflush(stdout);

        // Defensiv als nein gewertet bzw anhand der Einstellung
        if (wscanf(L"%1ls", line)) return DEFAULT_GAME_REPEAT_SETTING;

        // Standardverhalten für direktes Enter drücken
        if (line[0] == '\0') return DEFAULT_GAME_REPEAT_ON_ENTER_SETTING;

        // Nur erstes Zeichen auswerten
        const unsigned char c = (unsigned char) tolower((unsigned char) line[0]);
        if (c == 'j') return TRUE; // Ja
        if (c == 'n') return FALSE; // Nein
        if (c != 'n' && c != 'j')
            wprintf(L"Ungültige Eingabe: '%c'. Bitte 'j'/'y' für ja oder 'n' für nein.\n", line[0]);
    }
}


/**
 * @brief Startet das Programm und behandelt errors
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - andere Codes: Fehler
 */
int main() {
    setlocale(LC_ALL, "");
#ifdef _WIN32
    _setmode(_fileno(stdout), _O_U8TEXT);
#endif

    boolean repeat = TRUE;
    status error = OK;
    while (repeat) {
        if ((error = game_start()) != OK)
            switch (error) {
            case NULL_POINT_ERROR:
                    if (wprintf(L"Initialisierungsfehler\n") < 0) return PRINT_ERROR;
                    break;
            case CRITICAL_ERROR:
                    if (wprintf(L"Wachstum fehlgeschlagen") < 0) return PRINT_ERROR;
                    break;
            case USER_INPUT_ERROR:
                    if (wprintf(L"Eingabe konnte nicht verarbeitet werden") < 0) return PRINT_ERROR;
                    break;
            case PRINT_ERROR:
                    if (wprintf(L"Darstellung fehlgeschlagen") < 0) return PRINT_ERROR;
                    break;
            default: return error;
            }
        if (!ask_yes_no()) repeat = FALSE;
    }
    return error;
}