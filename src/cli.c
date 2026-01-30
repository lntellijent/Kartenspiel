//
// Created by hoerz on 30.01.2026.
//

#include <stdio.h>

#include "../header/player.h"

status ask_name() {
    if (wprintf(L"Spielername: ") > 0) return PRINT_ERROR;
    return OK;
} // #ToDo

status invalid_user_response() {
    if (wprintf(L"Diese Eingabe ist ungültig, bitte probiere es erneut: ") < 0) return PRINT_ERROR;
    return OK;
} // #ToDo

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
} // #ToDo

status round_sequence(const int round) {
    if (wprintf(L"\n%hs[Zug %d]%hs\n\n", "--------------------- ", round, " ---------------------") < 0) return PRINT_ERROR;
    return OK;
} // #ToDo

status card_played(const wchar_t* player_name, const Card *card, const boolean follow_up) {
    if (follow_up)
        if (wprintf(L", ") < 0) return PRINT_ERROR;
    if (wprintf(L"%ls legt %hs%.1ls", player_name, ranks[card->rank], &suits[card->suit]) < 0)
        return PRINT_ERROR;
    return OK;
} // #ToDo

status clash_decided(const wchar_t* player_name) {
    if (wprintf(L" - %ls gewinnt\n", player_name) < 0) return PRINT_ERROR;

    return OK;
} // #ToDo


status game_winner(const wchar_t* winning_player, const int winning_player_points) {
    if (wprintf(L"%ls gewinnt mit %d Punkten!\n", winning_player, winning_player_points) < 0) return PRINT_ERROR;
    return OK;
} // #ToDo

status deal_card_by_index(Deck *deck, Card *card, const int index) {
    if (!deck || !card) return NULL_POINT_ERROR;
    status error;
    if ((error = deck_draw_index(deck, card, index) != OK)) return error;
    return OK;
} // #ToDo
