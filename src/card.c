//
// Created by hoerz on 15.12.2025.
//

#include "../header/card.h"

status print_card(const Card card, wchar_t *s) {
    if (card.rank == 10) {
        s[0] = rank[card.rank][0];
        s[1] = rank[card.rank][1];
        s[2] = suit[card.suit];
        s[3] = L'\0';
    } else {
        s[0] = rank[card.rank][0];
        s[1] = suit[card.suit];
        s[2] = L'\0';
    }
    return OK;
}
