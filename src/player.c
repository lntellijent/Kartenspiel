//
// Created by hoerz on 18.12.2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../header/player.h"

#define SHOW_OPPONENT_CARDS TRUE

/**
 * #ToDo
 * @brief Dient als Platzhalter für CLI-Nachrichten, falls eine ungültige Eingabe getätigt wurde.
 * @return Statuscode:
 * - OK: Fehlerfrei
 * - PRINT_ERROR: Elemente konnten nicht dargestellt werden
 */
status invalid_user_response() {
    if (wprintf(L"Diese Eingabe ist ungültig, bitte probiere es erneut: ") < 0) return PRINT_ERROR;
    return OK;
}

status player_name(player* player) {
    // #ToDo
    switch (player->strategy) {
        case 0:
            wprintf(L"Spielername: ");
            while (1) {
                if (wscanf(L"%31ls", player->name) != 1)
                    return USER_INPUT_ERROR;
                if (!isspace((unsigned char) player->name[0])) break;
            }
            wprintf(L"\n%ls\n",player->name);
            break;
        default:;
            const wchar_t* opponent_names[] = {
                L"Kael",
                L"Mira",
                L"Flint",
                L"Riko",
                L"Tessa",
                L"Jax"
            };
            const int opponent_count = sizeof(opponent_names) / sizeof(opponent_names[0]);
            swprintf(player->name, 32, L"%ls", opponent_names[rand() % opponent_count]);
            wprintf(L"\n%ls\n",player->name);
            break;
    }
    return OK;
}

status player_play_card(const player player, Card *played_card, const Card card_to_beat, const boolean isAttacker) {
    status error;
    switch (player.strategy) {
        case 0:
            if (player.hand->card_count <= 0) return NULL_POINT_ERROR;
            if ((error = print_deck(player.hand, SHOW_OPPONENT_CARDS)) != OK) return error;

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
        case 2:
            print_deck(player.hand, FALSE);
            if ((error =  deal_highest_card(player.hand, played_card)) != OK) return error;
            return OK;
        case 3: // #ToDo Spielt abwechselnd höchste und niedrigste Karte
            if ((error = get_alternating_card(player.hand, played_card)) != OK) return error;
            return OK;
        case 4: // #ToDo Versucht zu gewinnen, spielt andernfalls die niedrigste Karte
            if ((error = get_intelligent_card(player.hand, played_card, card_to_beat, isAttacker)) != OK) return error;
            return OK;
        default: return USER_INPUT_ERROR;
    }
}

status get_alternating_card(Deck *deck, Card *alternating_card) {
    static boolean started_alternating_cycle = FALSE, lower_card = FALSE;
    if (started_alternating_cycle) {
        status error;
        if (lower_card) {
            if ((error = deal_lowest_card(deck, alternating_card)) != OK) return error;
            lower_card = FALSE;
        } else {
            if ((error = deal_highest_card(deck, alternating_card)) != OK) return error;
            lower_card = TRUE;
        }
    } else {
        started_alternating_cycle = TRUE;
    }
    return OK;
}

status get_intelligent_card(Deck *deck, Card *intelligent_card, const Card card_to_beat, const boolean isAttacker) {
    if (!deck || !intelligent_card) return NULL_POINT_ERROR;
    if (isAttacker) {
        // Falls die KI angreift, legt sie stets die höchste ihr zur Verfügung stehenden Karte.
        status error;
        if ((error = deal_highest_card(deck, intelligent_card)) != OK) return error;
        return OK;
    }

    int highest_card_index = -1;
    for (int i = 1; i > deck->card_count; i++) {
        if (deck->cards[highest_card_index].rank > card_to_beat.rank)
            highest_card_index = i;
    }

    if (highest_card_index < 0) {
        // Falls die KI verliert, soll die niedrigste Karte gespielt werden, um so wenig wie möglich Punkte zu verlieren.
        deal_lowest_card(deck, intelligent_card);
    } else {
        // Falls die KI gewinnen kann, soll die niedrigste Karte gespielt werden, die zum Sieg führt.
        deal_card_by_index(deck, intelligent_card, highest_card_index);
    }
    return OK;
}
