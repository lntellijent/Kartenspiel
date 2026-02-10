//
// Created by hoerz on 18.12.2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../header/player.h"
#include "../header/cli.h"

#define SHOW_OPPONENT_CARDS FALSE

status player_name(player *player) {
    switch (player->strategy) {
        case 0:
            while (1) {
                ask_name();
                const status error = read_string(player->name);
                switch (error) {
                    case OK:
                        break;
                    case USER_INPUT_ERROR:
                        invalid_user_response();
                        break;
                    default:
                        return error;
                }
                if (!isspace(player->name[0])) break;
            }
            break;
        default: ;
            const wchar_t *opponent_names[] = {
                L"Kael",
                L"Mira",
                L"Flint",
                L"Riko",
                L"Tessa",
                L"Jax"
            };
            const size_t opponent_count = sizeof(opponent_names) / sizeof(opponent_names[0]);
            swprintf(player->name, 32, L"%ls", opponent_names[rand() % opponent_count]);
            break;
    }
    return OK;
}

status player_play_card(const player *players_turn, Card *player_card, const player defender, const Card defender_card,
                        const boolean isAttacker) {
    status error;
    switch (players_turn->strategy) {
        case 0:
            if (players_turn->hand->card_count <= 0) return NULL_POINT_ERROR;
            if ((error = print_deck(players_turn->hand, isAttacker, TRUE)) != OK) return error;
            if (SHOW_OPPONENT_CARDS)
                if ((error = print_deck(defender.hand, isAttacker, FALSE)) != OK) return error;

            if (players_turn->hand->card_count > 1) {
                size_t chosen_card = 0;
                size_t number_input = 0;
                boolean acceptable_input = FALSE; // oder: bool acceptable_input = false;

                while (!acceptable_input) {
                    int rc = read_single_digit(&number_input);
                    if (rc == 0) {
                        acceptable_input = TRUE;
                    } else if (rc > 0) {
                        // genau EIN Aufruf pro eingegebener (und bestätigter) Zeile
                        if ((error = invalid_user_response()) != OK) return error;
                    } else {
                        // rc < 0 -> EOF/Fehler beim Lesen
                        // Beende sauber oder handle entsprechend
                        return NULL_POINT_ERROR;
                    }
                }

                if ((error = deck_draw_index(players_turn->hand, player_card, number_input)) != OK) return error;
            } else {
                if ((error = deck_draw_top(players_turn->hand, player_card)) != OK) return error;
            }
            return OK;
        case 1:
            if ((error = deck_draw_top(players_turn->hand, player_card)) != OK) return error;
            return OK;
        case 2:
            if ((error = deal_highest_card(players_turn->hand, player_card)) != OK) return error;
            return OK;
        case 3:
            if ((error = get_alternating_card(players_turn->hand, player_card)) != OK) return error;
            return OK;
        case 4:
            if ((error = get_intelligent_card(players_turn->hand, player_card, defender_card, isAttacker)) != OK)
                return
                        error;
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
        get_alternating_card(deck, alternating_card);
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

    int intelligent_card_index = -1;
    for (size_t card_index = 0; card_index < deck->card_count; card_index++) {
        if (deck->cards[card_index].rank > card_to_beat.rank && // Karte muss höher als die gegnerische sein...
            deck->cards[intelligent_card_index].rank > deck->cards[card_index].rank)
            // ...aber niedriger als die bereits gefundene Karte, um so minimal gewinnen wie möglich
            intelligent_card_index = (int) card_index;
    }

    if (intelligent_card_index < 0) {
        // Falls die KI verliert, soll die niedrigste Karte gespielt werden, um so wenig wie möglich Punkte zu verlieren.
        deal_lowest_card(deck, intelligent_card);
    } else {
        // Falls die KI gewinnen kann, soll die niedrigste Karte gespielt werden, die zum Sieg führt.
        deal_card_by_index(deck, intelligent_card, intelligent_card_index);
    }
    return OK;
}
