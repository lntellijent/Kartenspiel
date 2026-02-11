//
// Created by hoerz on 30.01.2026.
//

#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include <ctype.h>

#include "../header/cli.h"


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


status game_winner(const wchar_t *winning_player_name, const size_t winning_player_points,
                   const wchar_t *loosing_player_name, const size_t loosing_player_points) {
    if (wprintf(L"%ls gewinnt mit %llu Punkten! (%ls hatte %llu Punkte)\n", winning_player_name, winning_player_points,
                loosing_player_name, loosing_player_points) < 0)
        return PRINT_ERROR;
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

static void discard_line(void) {
    wint_t wc;
    // Rest der Zeile verwerfen (Wide-Variante)
    while ((wc = getwchar()) != L'\n' && wc != WEOF) {
    }
}

status read_single_digit(const wchar_t *input, void *out_void) {
    size_t *out = (size_t *) out_void;
    // exakt eine Ziffer [0-9]
    if (input[0] >= L'0' && input[0] <= L'9' && input[1] == L'\0') {
        *out = (size_t) (input[0] - L'0');
        return OK;
    }
    return USER_INPUT_ERROR;
}

status read_yes_no(const wchar_t *input, void *out_void) {
    boolean *out = (boolean *) out_void;
    if (input[0] != L'\0' && input[1] == L'\0') {
        wchar_t c = towlower(input[0]);
        if (c == L'j') {
            *out = TRUE;
            return OK;
        }
        if (c == L'n') {
            *out = FALSE;
            return OK;
        }
    }
    return USER_INPUT_ERROR;
}

status read_string(const wchar_t *input, void *out_void) {
    // Erwartet, dass out_void auf eine Struktur mit Puffer + Kapazität zeigt
    wbuf *wb = (wbuf *) out_void;

    if (!wb || !wb->ptr || wb->cap == 0) return USER_INPUT_ERROR;

    // Sichere Kopie mit Begrenzung und Nullterminator
    size_t i = 0;
    for (; input[i] && i + 1 < wb->cap; ++i) {
        wb->ptr[i] = input[i];
    }
    wb->ptr[i] = L'\0';

    // Wenn abgeschnitten wurde, ist das u. U. trotzdem OK – je nach Policy.
    // Falls „zu lang“ als Fehler gelten soll:
    if (input[i] != L'\0') return USER_INPUT_ERROR;

    return OK;
}

status read_with(const reader_fn reader, void *output) {
    // Puffergröße = INPUT_LENGTH (Nutzlänge) + 1 (Terminierung)
    wchar_t line[INPUT_LENGTH + 1];
    if (!fgetws(line, (int) (sizeof line / sizeof line[0]), stdin))
        return USER_INPUT_ERROR;

    // Newline entfernen, falls vorhanden
    for (size_t i = 0; i < INPUT_LENGTH + 1; ++i) {
        if (line[i] == L'\n') {
            line[i] = L'\0';
            break;
        }
        if (line[i] == L'\0') break;
    }

    // Wenn kein '\n' gefunden wurde und Puffer voll ist, Rest verwerfen
    const size_t len = wcslen(line);
    if (len == INPUT_LENGTH) {
        // Es könnte abgeschnitten sein → restliche Zeile verwerfen
        wint_t wc;
        do {
            wc = getwchar();
        } while (wc != L'\n' && wc != WEOF);
    }

    // Trim: führende und folgende Whitespaces
    wchar_t *start = line;
    while (*start && iswspace(*start)) ++start;

    wchar_t *end = start;
    while (*end) ++end;
    while (end > start && iswspace(end[-1])) --end;
    *end = L'\0';
    // Delegieren
    return reader(start, output);
}
