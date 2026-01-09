#include <stdio.h>
#include "card.h"
#include "deck.h"
#include "player.h"

#include <fcntl.h>
#include <io.h>
#include <stdlib.h>

//C-Level C11

status game_start() {
    Deck* d = deck_create_standard();
    // Initialisiserung fehlgeschlagen
    if (!d) return NULLPOINT_ERROR;
    deck_shuffle(d);

    // Spieler(-Decks) initialisieren
    const int hand_size = 10;
    const player players[2] = {
        {.hand = deck_create_empty(hand_size), .points = deck_create_empty(hand_size), .strategy = 0}, // Spieler
        {.hand = deck_create_empty(hand_size), .points = deck_create_empty(hand_size), .strategy = 1} // Gegner
    };

    // Initialisierung fehlgeschlagen
    if (!players[0].hand || !players[0].points || !players[1].hand || !players[1].points) return NULLPOINT_ERROR;

    // Austeilen der Karten
    for (int p = 0; p < sizeof(players)/sizeof(players[0]); p++) {
        status err;
        if ((err = card_deal(d, players[p].hand, hand_size)) != OK) return err;
    }

    if (deck_is_empty(players[0].hand) || deck_is_empty(players[1].hand)) return NULLPOINT_ERROR;

    Card c_att, c_def;
    int att = 1, def = 0;

    const int player_size = sizeof(players)/sizeof(players[0]);
    int round = 0;
    while ( !(deck_is_empty(players[0].hand) && deck_is_empty(players[1].hand)) ) {
        round += 2;
        printf("Zug %d\n", round / player_size);

        /*
         * Attacker legt eine Karte
         */
        status err;
        if ((err = player_play_card(players[att], &c_att)) != OK)
            return err;
        printf("Spieler %d legt %s%c\n", att, rank_arr[c_att.rank], suit_arr[c_att.suit]);

        /*
         * Defender legt eine Karte
         */
        if ((err = player_play_card(players[def], &c_def)) != OK)
            return err;
        printf("Spieler %d legt %s%c\n", def, rank_arr[c_def.rank], suit_arr[c_def.suit]);

        // Stich wird entschieden
        switch (card_clash(&c_att, &c_def)) {
            default:
                return NULLPOINT_ERROR;
            case DEFENDER_WINS:
                if ((err = deck_insert(players[def].points, &c_att)) != OK) return err;
                if ((err = deck_insert(players[def].points, &c_def)) != OK) return err;
                printf("Spieler %d gewinnt\n", def);

                // Angreifer-Verteidigerrolle wird getauscht
                const int temp = att;
                att = def;
                def = temp;
                break;
            case TIE: // Angreifer gewinnt ebenfalls, wenn die Gleiche Karte gelegt wurde
            case ATTACKER_WINS:
                if((err = deck_insert(players[att].points, &c_att)) != OK) return err;
                if ((err = deck_insert(players[att].points, &c_def)) != OK) return err;
                printf("Spieler %d gewinnt\n", att);
                // Angreifer-Verteidigerrolle wird nicht getauscht
                break;
        }
    }

    printf("\n\n");
    for (int p = 0; p < player_size; p++) {
        const int points = deck_consume_and_count_worth(players[p].points);
        if (points == -1) return NULLPOINT_ERROR;
        printf("Spieler %d erzielte %d Punkte (%d)\n", p, points, players[p].hand->size);
    }

    free(players[0].hand);
    free(players[0].points);
    free(players[1].hand);
    free(players[1].points);
    free(d);

    return OK;
}

/**
 * @brief Startet das Programm und behandelt errors
 * @return Errorcode; bei fehlerfreier Ausführung = 0
 */
int main() {

    /*_setmode(_fileno(stdout), _O_U16TEXT);

    wprintf(L"Pik: \u2660  Kreuz: \u2663  Herz: \u2665  Karo: \u2666\n");*/

    status err;
    if ((err = game_start()) != OK)
        switch (err) {
            case NULLPOINT_ERROR:
                printf("Initialisierungsfehler (Nullpointerexception)\n");
                break;
            case CRITICAL_ERROR:
                printf("Wachstum fehlgeschlagen (%d)", err);
                break;
            case USER_INPUT_ERROR:
                printf("Eingabe konnte nicht verarbeitet werden");
                break;
            default: return err;
        }
    return err;
}