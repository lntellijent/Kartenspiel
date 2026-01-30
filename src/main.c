#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#include <fcntl.h>

#include "../header/player.h"
#include "../header/cli.h"

#define DEFAULT_GAME_REPEAT_SETTING FALSE
#define DEFAULT_GAME_REPEAT_ON_ENTER_SETTING TRUE

//C-Level C11

status game_start() {
    status error;
    Deck *main_deck = create_standard_deck();
    // Initialisiserung fehlgeschlagen
    if ((error = shuffle(main_deck)) != OK) return error;

    // Spieler(-Decks) initialisieren
    const size_t hand_size = 10;
    player players[2] = {
        {.hand = create_empty_deck(hand_size), .points = create_empty_deck(hand_size), .strategy = 0}, // Spieler
        {.hand = create_empty_deck(hand_size), .points = create_empty_deck(hand_size), .strategy = 4} // Gegner
    };

    // Initialisierung fehlgeschlagen
    if (!players[0].hand || !players[0].points || !players[1].hand || !players[1].points) return NULL_POINT_ERROR;


    for (size_t player_index = 0; player_index < sizeof(players)/sizeof(players[0]); player_index++) {
        if ((error = player_name(&players[player_index])) != OK) return error;
    }

    // Austeilen der Karten
    for (size_t player_index = 0; player_index < sizeof(players) / sizeof(players[0]); player_index++)
        if ((error = card_deal(main_deck, players[player_index].hand, hand_size)) != OK) return error;

    if (is_empty(players[0].hand) || is_empty(players[1].hand)) return NULL_POINT_ERROR;

    // Start-Sequenz des Spiels
    if ((error = start_sequence()) != OK) return error;

    Card attacker_card, defender_card;
    size_t attacker_index = 1, defender_index = 0;

    const size_t player_size = sizeof(players) / sizeof(players[0]);
    size_t round_index = 0;
    while (!(is_empty(players[0].hand) && is_empty(players[1].hand))) {
        round_index++;

        // Runden-Sequenz des Spiels
        if ((error = round_sequence(round_index)) != OK) return error;

        // Angreifer legt eine Karte
        if ((error = player_play_card(&players[attacker_index], &attacker_card, players[defender_index], attacker_card, TRUE)) != OK) return error;
        if ((error = card_played(players[attacker_index].name, &attacker_card, FALSE)) != OK) return error;

        // Verteidiger legt eine Karte
        if ((error = player_play_card(&players[defender_index], &defender_card, players[attacker_index], attacker_card, FALSE)) != OK) return error;
        if ((error = card_played(players[defender_index].name, &defender_card, defender_index)) != OK) return error;


        // Stich wird entschieden
        switch (card_clash(attacker_card, defender_card)) {
            default: return NULL_POINT_ERROR;
            case DEFENDER_WINS:
                if ((error = insert(players[defender_index].points, &attacker_card)) != OK) return error;
                if ((error = insert(players[defender_index].points, &defender_card)) != OK) return error;
                if ((error = clash_decided(players[defender_index].name)) != OK) return error;
                // Angreifer-Verteidigerrolle wird getauscht
                const size_t temp_role_index = attacker_index;
                attacker_index = defender_index;
                defender_index = temp_role_index;
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

    size_t winning_player_index = 0, winning_player_points = 0;
    if (wprintf(L"\n\n") < 0) return PRINT_ERROR;
    for (size_t player_index = 0; player_index < player_size; player_index++) {
        const size_t points = consume_and_count_worth(players[player_index].points);
        if (points == -1) return NULL_POINT_ERROR;
        if (winning_player_points < points) {
            winning_player_index = player_index;
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
static void trim(char *s) { // #ToDo
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
boolean ask_yes_no() { // #ToDo
    wchar_t line[2];

    for (;;) {
        wprintf(L"Noch ein mal Spielen? [j/n]: ");
        fflush(stdout);

        // Defensiv als nein gewertet bzw anhand der Einstellung
        if (wscanf(L"%1ls", line)) return DEFAULT_GAME_REPEAT_SETTING; // #ToDo

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