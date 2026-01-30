//
// Created by hoerz on 18.12.2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../header/player.h"
#include "../src/cli.c"

#define SHOW_OPPONENT_CARDS FALSE

status player_name(player* player) {
    switch (player->strategy) {
        case 0:
            ask_name();
            while (1) {
                if (wscanf(L"%31ls", player->name) != 1) // #ToDo
                    return USER_INPUT_ERROR;
                if (!isspace((unsigned char) player->name[0])) break;
            }
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
            break;
    }
    return OK;
}

status player_play_card(const player hplayer, Card *played_card, const player opponent, const Card card_to_beat, const boolean isAttacker) {
    status error;
    switch (hplayer.strategy) {
        case 0:
            if (hplayer.hand->card_count <= 0) return NULL_POINT_ERROR;
            if ((error = print_deck(hplayer.hand, isAttacker, TRUE)) != OK) return error;
            if (SHOW_OPPONENT_CARDS)
                if ((error = print_deck(opponent.hand, isAttacker, FALSE)) != OK) return error;

            if (hplayer.hand->card_count > 1) {
                wchar_t number_holder[2];
                int chosen_card = -1;
                while (chosen_card == -1)
                    if (wscanf(L"%1ls", number_holder) == 1) { // #ToDo
                        const int number_input = (int)wcstol(number_holder, NULL, 10);
                        if (number_input >= 0 && number_input < hplayer.hand->card_count)
                            chosen_card = number_input;
                        else
                            if ((error = invalid_user_response()) != OK) return error;
                    }

                if ((error = deck_draw_index(hplayer.hand, played_card, chosen_card)) != OK) return error;
            } else {
                if ((error = deck_draw_top(hplayer.hand, played_card)) != OK) return error;
            }
            return OK;
        case 1:
            if ((error = deck_draw_top(hplayer.hand, played_card)) != OK) return error;
            return OK;
        case 2:
            if ((error =  deal_highest_card(hplayer.hand, played_card)) != OK) return error;
            return OK;
        case 3:
            if ((error = get_alternating_card(hplayer.hand, played_card)) != OK) return error;
            return OK;
        case 4:
            if ((error = get_intelligent_card(hplayer.hand, played_card, card_to_beat, isAttacker)) != OK) return error;
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
    for (int i = 0; i < deck->card_count; i++) {
        if (deck->cards[i].rank > card_to_beat.rank && deck->cards[highest_card_index].rank > deck->cards[i].rank)
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
