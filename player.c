//
// Created by hoerz on 18.12.2025.
//

#include <stdio.h>
#include <stdlib.h>

#include "player.h"

/**
 * #ToDo
 * @brief Dient als Platzhalter für CLI-Nachrichten, falls eine ungültige Eingabe getätigt wurde.
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 */
status invalid_user_response() {
    if (printf("Diese Eingabe ist ungültig, bitte probiere es erneut: ") < 0) return PRINT_ERROR;
    return OK;
}

/**
 * @brief Spielt eine Karte
 *
 * (Unabhängig davon ob KI-Gegner oder menschlicher Spieler)
 * @param player Der Spieler, welcher eine Karte spielen soll
 * @param played_card Entält die Karte die gespielt wird
 * @return Fehler-/Statuscodes:
 * - OK: Fehlerfrei
 * - NULLPOINT_ERROR: Deck ist leer oder =NULL
 * - USER_INPUT_ERROR: invalide Strategie ausgewählt
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 * @note Strategien:
 * 1. Zufall
 * 2. niedrigste zuerst
 * 3. höchste zuerst
 * 4. Intelligent
 */
status player_play_card(const player player, Card *played_card) {
    status error;
    switch (player.strategy) {
        case 0:
            if (player.hand->card_count <= 0) return NULL_POINT_ERROR;
            if ((error = print_deck(player.hand, 1)) != OK) return error;

            if (player.hand->card_count > 1) {
                char number_holder[4];
                int chosen_card = -1;
                while (chosen_card == -1)
                    if (fgets(number_holder, sizeof(number_holder), stdin) != NULL) {
                        const int number_input = strtol(number_holder, NULL, 10);
                        if (number_input >= 0 && number_input < player.hand->card_count)
                            chosen_card = number_input;
                        else
                            if ((error = invalid_user_response()) != OK) return error;
                    }

                if ((error = deck_draw_index(player.hand, played_card, chosen_card)) != OK) return error;
            } else {
                if ((error = deck_draw_top(player.hand, played_card)) != OK) return error;
            }
            return OK;
        case 1:
            if ((error = deck_draw_top(player.hand, played_card)) != OK) return error;
            return OK;
        case 2: // #ToDo
            return OK;
        case 3: // #ToDo
            return OK;
        case 4: // #ToDo
            return OK;
        default: return USER_INPUT_ERROR;
    }
}
