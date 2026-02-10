//
// Created by hoerz on 30.01.2026.
//

#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>

#include "../header/cli.h"

#include <ctype.h>

status ask_name() {
    if (wprintf(L"Spielername: ") > 0) return PRINT_ERROR;
    return OK;
}

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
    if (wprintf(L"\n%hs[Zug %llu]%hs\n\n", "------------------------- ", round_index, " -------------------------") < 0)
        return PRINT_ERROR;
    return OK;
} // #ToDo

status card_played(const wchar_t *player_name, const Card *card, const boolean follow_up) {
    if (follow_up)
        if (wprintf(L", ") < 0) return PRINT_ERROR;
    if (wprintf(L"%ls legt %hs%.1ls", player_name, rank[card->rank], &suit[card->suit]) < 0)
        return PRINT_ERROR;
    return OK;
} // #ToDo

status clash_decided(const wchar_t *player_name, const Card *card1, const Card *card2) {
    if (wprintf(L" - %ls gewinnt (%hs > %hs)\n", player_name, rank[card1->rank], rank[card2->rank]) < 0)
        return PRINT_ERROR;
    return OK;
} // #ToDo


status game_winner(const wchar_t *winning_player_name, const size_t winning_player_points, const wchar_t *loosing_player_name, const size_t loosing_player_points) {
    if (wprintf(L"%ls gewinnt mit %llu Punkten! (%ls hatte %llu Punkte)\n", winning_player_name, winning_player_points, loosing_player_name, loosing_player_points) < 0) return PRINT_ERROR;
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

void discard_line(void) {
    wint_t wc;
    while ((wc = getwchar()) != L'\n' && wc != WEOF) {}
}

status read_single_digit(size_t *out) {
    const size_t len = 32;
    wchar_t line[len];
    if (!fgetws(line, (int) (sizeof(line) / sizeof(line[0])), stdin)) return USER_INPUT_ERROR;

    // Wenn die Zeile mit '\n' endet, entferne ihn
    boolean new_line_found = FALSE;
    for (size_t i = 0; i < len; i++) {
        if (line[i] == L'\n') {
            line[i] = L'\0';
            new_line_found = TRUE;
            break;
        }
    }
    if (!new_line_found) {
        // Kein '\n' im Puffer -> Eingabe war länger als in den Buffer passt
        // => Zeile wurde abgeschnitten; Rest bis zum Zeilenende verwerfen
        discard_line();
    }

    // Optional: führende und folgende Whitespaces erlauben
    wchar_t *start = line;
    while (*start && iswspace(*start)) ++start;
    wchar_t *end = start;
    while (*end) ++end;
    while (end > start && iswspace(end[-1])) --end;
    *end = L'\0';

    // Gültig, wenn exakt 1 Ziffer übrig bleibt
    if (start[0] >= L'0' && start[0] <= L'9' && start[1] == L'\0') {
        *out = (size_t) (start[0] - L'0');
        return OK;
    }
    return USER_INPUT_ERROR;
}

status read_yes_no(boolean *out) {
    const size_t len = 12;
    wchar_t line[len];
    if (!fgetws(line, (int) (sizeof(line) / sizeof(line[0])), stdin)) return USER_INPUT_ERROR;

    // Wenn die Zeile mit '\n' endet, entferne ihn
    boolean new_line_found = FALSE;
    for (size_t i = 0; i < len; i++) {
        if (line[i] == L'\n') {
            line[i] = L'\0';
            new_line_found = TRUE;
            break;
        }
    }
    if (!new_line_found) {
        // Kein '\n' im Puffer -> Eingabe war länger als in den Buffer passt
        // => Zeile wurde abgeschnitten; Rest bis zum Zeilenende verwerfen
        discard_line();
    }


    // Zeilenumbruch entfernen (falls vorhanden)
    wchar_t *p = line;
    while (*p != L'\0' && *p != L'\n') ++p;
    if (*p == L'\n') *p = L'\0';

    // Optional: führende und folgende Whitespaces erlauben
    wchar_t *start = line;
    while (*start && iswspace(*start)) ++start;
    wchar_t *end = start;
    while (*end) ++end;
    while (end > start && iswspace(end[-1])) --end;
    *end = L'\0';

    // Gültig, wenn exakt j oder n übrig bleibt
    if (tolower(start[0]) == L'j' && start[1] == L'\0') {
        *out = TRUE;
        return OK;
    }
    if (tolower(start[0]) == L'n' && start[1] == L'\0') {
        *out = FALSE;
        return OK;
    }
    return USER_INPUT_ERROR;
}


status read_string(wchar_t out[32]) {
    const size_t len = 32;
    wchar_t line[len];
    if (!fgetws(line, 32, stdin))
        return USER_INPUT_ERROR;

    // Wenn die Zeile mit '\n' endet, entferne ihn
    boolean new_line_found = FALSE;
    for (size_t i = 0; i < len; i++) {
        if (line[i] == L'\n') {
            line[i] = L'\0';
            new_line_found = TRUE;
            break;
        }
    }
    if (!new_line_found) {
        // Kein '\n' im Puffer -> Eingabe war länger als in den Buffer passt
        // => Zeile wurde abgeschnitten; Rest bis zum Zeilenende verwerfen
        discard_line();
    }

    // führende Whitespaces entfernen
    wchar_t *start = line;
    while (*start && iswspace(*start)) ++start;

    // trailing Whitespaces entfernen
    wchar_t *end = start;
    while (*end) ++end;
    while (end > start && iswspace(end[-1])) --end;
    *end = L'\0';

    const size_t new_len = (size_t) (end - start);
    if (new_len > 0) {
        wmemcpy(out, start, len);
    }
    out[new_len] = L'\0';

    return OK;
}