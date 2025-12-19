#include <stdio.h>
#include "card.h"
#include "deck.h"
#include "player.h"

#include <fcntl.h>
#include <io.h>

//C-Level C11

int game_start() {
    Deck* d = deck_create_standard();
    // Initialisiserung fehlgeschlagen
    if (!d)
        return -112;
    deck_shuffle(d);

    // Spieler(-Decks) initialisieren
    const int hand_size = 10;
    const gamer players[2] = {
        {.hand = deck_create_empty(hand_size), .points = deck_create_empty(hand_size), .strategy = 0}, // Spieler
        {.hand = deck_create_empty(hand_size), .points = deck_create_empty(hand_size), .strategy = 1} // Gegner
    };

    // Initialisierung fehlgeschlagen
    if (!players[0].hand || !players[0].points || !players[1].hand || !players[1].points)
        return -112;

    // Austeilen der Karten
    for (int p = 0; p < sizeof(players)/sizeof(players[0]); p++) {
        int err;
        if ((err = card_deal(d, players[p].hand, hand_size)) != 0)
            return err;
    }

    Card c_att, c_def;
    int att = 1, def = 0;

    const int player_size = sizeof(players)/sizeof(players[0]);
    int round = 0;
    while (round < hand_size * player_size - 1) {
        printf("\033[2J\033[H");
        round += 2;
        printf("Zug %d\n", round / player_size);

        /*
         * Attacker legt eine Karte
         */
        int err;
        if ((err = player_play_card(players[att], &c_att)) != 0)
            return err;
        printf("Spieler %d legt %s%c\n", att, rank_arr[c_att.rank], suit_arr[c_att.suit]);

        /*
         * Defender legt eine Karte
         */
        if ((err = player_play_card(players[def], &c_def)) != 0)
            return err;
        printf("Spieler %d legt %s%c\n", def, rank_arr[c_def.rank], suit_arr[c_def.suit]);

        // Stich wird entschieden
        switch (card_clash(&c_att, &c_def)) {
            default:
                return -5;
            case 0:
                if ((err = deck_insert(players[def].points, &c_att)) != 0)
                    return err;
                if ((err = deck_insert(players[def].points, &c_def)) != 0)
                    return err;
                printf("Spieler %d gewinnt\n", def);

                // Angreifer-Verteidigerrolle wird getauscht
                const int temp = att;
                att = def;
                def = temp;
                break;
            case 1:
                if((err = deck_insert(players[att].points, &c_att)) != 0)
                    return err;
                if ((err = deck_insert(players[att].points, &c_def)) != 0)
                    return err;
                printf("Spieler %d gewinnt\n", att);
                // Angreifer-Verteidigerrolle wird nicht getauscht
                break;
        }
    }

    printf("\n\n");
    for (int p = 0; p < player_size; p++) {
        int points = deck_count_worth(players[p].points);
        if (points == -113)
            return -113;
        printf("Spieler %d erzielte %d Punkte (%d)\n", p, points, players[p].hand->size);
    }

    return 0;
}

/**
 * @brief Startet das Programm und behandelt errors
 * @return Errorcode; bei fehlerfreier Ausführung = 0
 */
int main() {

    /*_setmode(_fileno(stdout), _O_U16TEXT);

    wprintf(L"Pik: \u2660  Kreuz: \u2663  Herz: \u2665  Karo: \u2666\n");*/

    int err = 0;
    if ((err = game_start()) != 0)
        switch (err) {
            case -1:
                printf("Deck leer (%d)", err);
                break;
            case -112:
                printf("Initialisiserungserror: %d\n", err);
                break;
            case -361:
                printf("Wachstum fehlgeschlagen (%d)", err);
                break;
            case -824:
                printf("ungültige Kapazität für das Arraywachstum (%d)", err);
                break;
            case -825:
                printf("ungültige Gegnerstrategie ausgewählt (%d)", err);
                break;
            default: return err;
        }
    return err;
}