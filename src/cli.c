//
// Created by hoerz on 30.01.2026.
//

#include <stdio.h>

#include "../header/cli.h"

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

status round_sequence(const size_t round_index) {
    if (wprintf(L"\n%hs[Zug %llu]%hs\n\n", "--------------------- ", round_index, " ---------------------") < 0) return PRINT_ERROR;
    return OK;
} // #ToDo

status card_played(const wchar_t* player_name, const Card *card, const boolean follow_up) {
    if (follow_up)
        if (wprintf(L", ") < 0) return PRINT_ERROR;
    if (wprintf(L"%ls legt %hs%.1ls", player_name, rank[card->rank], &suit[card->suit]) < 0)
        return PRINT_ERROR;
    return OK;
} // #ToDo

status clash_decided(const wchar_t* player_name) {
    if (wprintf(L" - %ls gewinnt\n", player_name) < 0) return PRINT_ERROR;
    return OK;
} // #ToDo


status game_winner(const wchar_t* winning_player_name, const size_t winning_player_points) {
    if (wprintf(L"%ls gewinnt mit %llu Punkten!\n", winning_player_name, winning_player_points) < 0) return PRINT_ERROR;
    return OK;
} // #ToDo

status print_deck(Deck *deck, const boolean player_isAttacker, const boolean print_indexes) {
    if (!deck || deck->card_count <= 0) return NULL_POINT_ERROR;
    if (print_indexes) {
        if (!player_isAttacker)
            wprintf(L", ");
        if (wprintf(L"deine Karten:\n%5hs", "") < 0) return PRINT_ERROR;
    } else
        if (wprintf(L"%5hs", "") < 0) return PRINT_ERROR;

    for (size_t i = 0; i < deck->card_count; i++)
        if (wprintf(L"%hs%2hs%.1ls%hs",
                    i != 0 ? " " : "",
                    rank[deck->cards[i].rank],
                    &suit[deck->cards[i].suit],
                    i + 1 == deck->card_count ? ".\n" : "") < 0)
            return PRINT_ERROR;
    if (wprintf(L"%5hs", "") < 0) return PRINT_ERROR;

    if (print_indexes) {
        for (size_t i = 0; i < deck->card_count; i++)
            if (wprintf(L"%1hs[%llu]", "", i) < 0) return PRINT_ERROR;
        if (wprintf(L"\n") < 0) return PRINT_ERROR;
    }
    return OK;
}