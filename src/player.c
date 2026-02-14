//
// Created by hoerz on 18.12.2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../header/player.h"
#include "../header/cli.h"
#include "../header/strategy.h"

#define SHOW_OPPONENT_CARDS FALSE

status player_name(player *player) {
    if (player->strategy == get_humanoid_card) {
        while (1) {
            ask_name();
            wbuf wb = {.ptr = player->name, .cap = sizeof(player->name) / sizeof(player->name[0])};
            const status error = read_with(read_string, &wb);
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
    } else {
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
    }
    return OK;
}

void player_free(const player *player) {
    deck_free(player->hand);
    deck_free(player->points);
}